import numpy as np
import pandas as pd
import yfinance as yf
from sklearn.preprocessing import MinMaxScaler
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense
from datetime import datetime, timedelta


# Function to fetch the stock data
def fetch_stock_data(symbol, start_date, end_date):
    ticker = yf.Ticker(symbol)
    try:
        stock_data = ticker.history(start=start_date, end=end_date)
        print("Data fetched successfully.")
        return stock_data[['High', 'Low']]
    except Exception as e:
        print(f"Failed to fetch data: {e}")
        return pd.DataFrame()  # Return empty DataFrame on failure


# Function to prepare data for LSTM
def prepare_data(data_df, look_back):
    scaler_high = MinMaxScaler(feature_range=(0, 1))
    scaler_low = MinMaxScaler(feature_range=(0, 1))

    scaled_high = scaler_high.fit_transform(data_df[['High']])
    scaled_low = scaler_low.fit_transform(data_df[['Low']])

    X, Y_high, Y_low = [], [], []
    for i in range(len(scaled_high) - look_back):
        a = np.hstack((scaled_high[i:i + look_back], scaled_low[i:i + look_back]))
        X.append(a)
        Y_high.append(scaled_high[i + look_back])
        Y_low.append(scaled_low[i + look_back])
    return np.array(X), np.array(Y_high), np.array(Y_low), scaler_high, scaler_low


# Function to build the LSTM model
def build_model(look_back):
    model = Sequential([
        LSTM(50, return_sequences=True, input_shape=(look_back, 2)),  # 2 features: High and Low
        LSTM(50),
        Dense(25),
        Dense(1)
    ])
    model.compile(optimizer='adam', loss='mean_squared_error')
    return model


# Main function for backtesting
def backtest_model():
    symbol = 'TSLA'
    start_date = '2023-01-01'
    end_date = (datetime.now() - timedelta(days=1)).strftime('%Y-%m-%d')  # until yesterday
    data = fetch_stock_data(symbol, start_date, end_date)

    look_back = 60
    X, y_high, y_low, scaler_high, scaler_low = prepare_data(data, look_back)

    # Split data - the last 5 days for testing
    train_X, test_X = X[:-5], X[-5:]
    train_y_high, test_y_high = y_high[:-5], y_high[-5:]
    train_y_low, test_y_low = y_low[:-5], y_low[-5:]

    # Create and train models for high and low predictions
    model_high = build_model(look_back)
    model_low = build_model(look_back)

    model_high.fit(train_X, train_y_high, epochs=10, batch_size=1, verbose=1)
    model_low.fit(train_X, train_y_low, epochs=10, batch_size=1, verbose=1)

    # Predictions
    predicted_highs = model_high.predict(test_X)
    predicted_lows = model_low.predict(test_X)

    # Inverse transform to get actual values
    predicted_highs = scaler_high.inverse_transform(predicted_highs)
    predicted_lows = scaler_low.inverse_transform(predicted_lows)

    # Ensure the test labels are correctly reshaped for inverse transformation
    # If `test_y_high` is a one-dimensional array, reshape it to two dimensions
    actual_highs = scaler_high.inverse_transform(test_y_high.reshape(-1, 1))
    actual_lows = scaler_low.inverse_transform(test_y_low.reshape(-1, 1))

    # Output predictions and actuals for comparison
    for i in range(len(predicted_highs)):
        print(f"Day {i + 1}: Predicted High: {predicted_highs[i][0]}, Actual High: {actual_highs[i][0]}")
        print(f"Day {i + 1}: Predicted Low: {predicted_lows[i][0]}, Actual Low: {actual_lows[i][0]}")


if __name__ == '__main__':
    backtest_model()
