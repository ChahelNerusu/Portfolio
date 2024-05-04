#include "bitreader.h"

#include <stdio.h>
#include <stdlib.h>
struct BitReader {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

BitReader *bit_read_open(const char *filename) {
    BitReader *reader = (BitReader *) malloc(sizeof(BitReader));
    if (reader == NULL) {
        return NULL;
    }
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        free(reader);
        return NULL;
    }

    reader->underlying_stream = f;
    reader->byte = 0;
    reader->bit_position = 8;
    return reader;
}
uint8_t bit_read_bit(BitReader *buf) {
    if (buf->bit_position > 7) {
        int byte = fgetc(buf->underlying_stream);

        if (byte == EOF) {
            fprintf(stderr, "Fatal error\n");
            exit(1);
        }

        buf->byte = (uint8_t) byte;
        buf->bit_position = 0;
    }
    uint8_t bit_value = (buf->byte >> buf->bit_position) & 1;
    buf->bit_position += 1;

    return bit_value;
}

void bit_read_close(BitReader **pbuf) {
    if (*pbuf != NULL) {
        if (fclose((*pbuf)->underlying_stream) == EOF) {
            perror("Error closing");
            exit(1);
        }
        free(*pbuf);
        *pbuf = NULL;
    }
}
uint8_t bit_read_uint8(BitReader *buf) {
    uint8_t byte = 0x00;
    for (int i = 0; i < 8; i++) {
        uint8_t b = bit_read_bit(buf);
        byte |= (b & 1) << i;
    }
    return byte;
}
uint32_t bit_read_uint32(BitReader *buf) {
    uint32_t word = 0x00000000;
    for (int i = 0; i < 32; i++) {
        uint8_t b = bit_read_bit(buf);
        word |= ((uint32_t) (b & 1) << i);
    }
    return word;
}
uint16_t bit_read_uint16(BitReader *buf) {
    uint16_t word = 0x0000;
    for (int i = 0; i < 16; ++i) {
        uint8_t b = bit_read_bit(buf);
        word |= (uint16_t) (b & 1) << i;
    }

    return word;
}
