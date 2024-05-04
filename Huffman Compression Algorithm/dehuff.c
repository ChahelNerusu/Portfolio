#include "bitreader.h"
#include "node.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define STACK_SIZE 64
#define OPTIONS    "hi:o:"
void node_free_tree(Node *node);

void stack_push(Node **stack, Node *node, int *top) {
    if (*top < STACK_SIZE - 1) {
        (*top)++;
        stack[*top] = node;
    } else {
        fprintf(stderr, "Error: Stack overflow\n");
        exit(1);
    }
}

Node *stack_pop(Node **stack, int *top) {
    if (*top >= 0) {
        Node *node = stack[*top];
        (*top)--;
        return node;
    } else {
        fprintf(stderr, "Error: Stack underflow\n");
        exit(1);
    }
}

void dehuff_decompress_file(FILE *fout, BitReader *inbuf) {
    uint8_t type1 = bit_read_uint8(inbuf);
    uint8_t type2 = bit_read_uint8(inbuf);
    assert(type1 == 'H');
    assert(type2 == 'C');

    uint32_t filesize = bit_read_uint32(inbuf);
    uint16_t num_leaves = bit_read_uint16(inbuf);

    Node *stack[STACK_SIZE];
    int top = -1;

    Node *node;
    uint32_t num_nodes = 2 * num_leaves - 1;

    for (uint32_t i = 0; i < num_nodes; i++) {
        if (bit_read_bit(inbuf) == 1) {
            uint8_t symbol = bit_read_uint8(inbuf);
            node = node_create(symbol, 0);
        } else {
            node = node_create(0, 0);
            node->right = stack_pop(stack, &top);
            node->left = stack_pop(stack, &top);
        }
        stack_push(stack, node, &top);
    }
    Node *code_tree = stack_pop(stack, &top);
    for (uint32_t i = 0; i < filesize; i++) {
        node = code_tree;
        while (1) {
            uint8_t bit = bit_read_bit(inbuf);
            if (bit == 0) {
                node = node->left;
            } else {
                node = node->right;
            }
            if (node->left == NULL && node->right == NULL) {
                break;
            }
        }
        fputc(node->symbol, fout);
    }
    node_free_tree(code_tree);
}
void node_free_tree(Node *node) {
    if (node == NULL) {
        return;
    }
    node_free_tree(node->left);
    node_free_tree(node->right);
    free(node);
}
int main(int argc, char **argv) {
    int opt;
    const char *infile = NULL;
    const char *outfile = NULL;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = optarg; break;
        case 'o': outfile = optarg; break;
        case 'h':
        default:
            printf("Usage: huff -i infile -o outfile\n");
            printf("       huff -v -i infile -o outfile\n");
            printf("       huff -h\n");
            return 1;
        }
    }
    if (infile == NULL) {
        fprintf(stderr, "huff: -i option is required\n");
        printf("Usage: huff -i infile -o outfile\n");
        printf("       huff -v -i infile -o outfile\n");
        printf("       huff -h\n");
        return 1;
    }
    if (infile == NULL || outfile == NULL) {
        fprintf(stderr, "Usage: huff -i infile -o outfile\n");
        return 1;
    }
    FILE *ifile = fopen(infile, "rb");
    if (ifile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE *ofile = fopen(outfile, "wb");
    if (ofile == NULL) {
        perror("Error opening output file");
        fclose(ifile);
        return 1;
    }

    BitReader *inbuf = bit_read_open(infile);
    if (inbuf == NULL) {
        fprintf(stderr, "Error creating BitReader\n");
        fclose(ifile);
        fclose(ofile);
        return 1;
    }

    dehuff_decompress_file(ofile, inbuf);
    fclose(ifile);
    fclose(ofile);
    bit_read_close(&inbuf);

    return 0;
}
