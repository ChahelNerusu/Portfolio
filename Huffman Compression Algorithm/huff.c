#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#define OPTIONS "hi:o:"
typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;
uint32_t fill_histogram(FILE *fin, uint32_t *histogram) {
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }

    uint32_t filesize = 0;
    int byte;
    while ((byte = fgetc(fin)) != EOF) {
        ++histogram[byte];
        ++filesize;
    }
    ++histogram[0x00];
    ++histogram[0xff];
    return filesize;
}

Node *create_tree(uint32_t *histogram, uint16_t *num_leaves) {
    PriorityQueue *pq = pq_create();
    for (int i = 0; i < 256; i++) {
        if (histogram[i] > 0) {
            Node *new_node = node_create((uint8_t) i, histogram[i]);
            enqueue(pq, new_node);
            (*num_leaves)++;
        }
    }
    while (!pq_size_is_1(pq)) {
        Node *left = dequeue(pq);
        Node *right = dequeue(pq);
        Node *new_node = node_create(0, left->weight + right->weight);
        new_node->left = left;
        new_node->right = right;
        enqueue(pq, new_node);
    }
    Node *root = dequeue(pq);
    pq_free(&pq);

    return root;
}

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    if (node->left != NULL && node->right != NULL) {
        fill_code_table(code_table, node->left, code, code_length + 1);
        code |= (uint64_t) 1 << code_length;
        fill_code_table(code_table, node->right, code, code_length + 1);
    } else {
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;
    }
}
void huff_write_tree(BitWriter *outbuf, Node *node) {
    if (node->left == NULL) {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    } else {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    }
}

void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table) {
    bit_write_uint8(outbuf, 'H');
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);
    int b;
    while (1) {
        b = fgetc(fin);
        if (b == EOF) {
            break;
        }

        uint64_t code = code_table[b].code;
        uint8_t code_length = code_table[b].code_length;

        for (int i = 0; i < code_length; i++) {
            bit_write_bit(outbuf, code & 1);
            code >>= 1;
        }
    }
}
void free_huffman_tree(Node *node) {
    if (node == NULL) {
        return;
    }
    free_huffman_tree(node->left);
    free_huffman_tree(node->right);
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
        default: printf("Usage: huff -i infile -o outfile\n"); return EXIT_FAILURE;
        }
    }
    if (infile == NULL) {
        fprintf(stderr, "huff: -i option is required\n");
        printf("Usage: huff -i infile -o outfile\n");
        printf("       huff -v -i infile -o outfile\n");
        printf("       huff -h\n");
        return 1;
    }
    FILE *ifile = fopen(infile, "rb");
    if (ifile == NULL) {
        perror("Error opening input");
        return 1;
    }

    BitWriter *outbuf = bit_write_open(outfile);
    if (outbuf == NULL) {
        perror("Error opening output");
        fclose(ifile);
        return 1;
    }

    uint32_t histogram[256] = { 0 };
    uint32_t filesize = fill_histogram(ifile, histogram);
    fseek(ifile, 0, SEEK_SET);

    uint16_t num_leaves = 0;
    Node *code_tree = create_tree(histogram, &num_leaves);
    Code code_table[256] = { 0 };
    fill_code_table(code_table, code_tree, 0, 0);

    huff_compress_file(outbuf, ifile, filesize, num_leaves, code_tree, code_table);
    free_huffman_tree(code_tree);
    fclose(ifile);
    bit_write_close(&outbuf);
    return 0;
}
