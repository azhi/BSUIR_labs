#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "common.h"

struct {
  uint16_t c[2];
  uint8_t value;
} ht[511];

union {
  uint64_t num;
  uint8_t arr[8];
} length;

uint16_t last_free = 1;

uint8_t r = 0, pos = 0;

void read_node(FILE *fin, unsigned short node)
{
  uint8_t term = r & (1 << (7 - pos));
  ++pos;

  if (pos == 8) {
    r = fgetc(fin);
    pos = 0;
  }

  if (term) {
    if (!pos) {
      ht[node].value = r;
      r = fgetc(fin);
    } else {
      ht[node].value = r << pos;
      r = fgetc(fin);
      ht[node].value ^= r >> (8 - pos);
    }
#ifdef DEBUG_TREE_DUMP
    printf("LEAF %.2x, r: %.2x, pos: %d\n", ht[node].value, r, pos);
#endif
  } else {
#ifdef DEBUG_TREE_DUMP
    printf("NODE, r: %.2x, pos: %d\n", r, pos);
#endif
    ht[node].c[0] = last_free++;
    ht[node].c[1] = last_free++;
    read_node(fin, ht[node].c[0]);
    read_node(fin, ht[node].c[1]);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 3)
    die("Sorry, wrong number of arguments.\n"
        "Correct use is:\t%s archive filename\n", argv[0]);

  FILE *fin = fopen(argv[1], "rb");
  if (fin == NULL)
    die("Sorry, cannot open file \"%s.\"\n", argv[1])

  FILE *fout = fopen(argv[2], "wb");
  if (fout == NULL)
    die("Sorry, cannot open file \"%s.\"\n", argv[2])

  // Getting length
  for (uint8_t i = 0; i < 8; ++i)
    length.arr[i] = fgetc(fin);

  // Handling an (almost) empty file
  if (length.num <= 1L) {
    if (length.num > 0L)
      fputc(fgetc(fin), fout);

    fclose(fout);
    fclose(fin);
    exit(0);
  }

  // Reading the tree
  r = fgetc(fin);
  pos = 0;
  read_node(fin, 0);

  // Decoding
  while (length.num > 0) {
    uint16_t htn = 0;
    while (ht[htn].c[0]) {
      if (pos == 8) {
        pos = 0;
        r = fgetc(fin);
      }

      uint8_t term = r & (1 << (7 - pos));
      ++pos;
      if (term)
        htn = ht[htn].c[1];
      else
        htn = ht[htn].c[0];
    }
    fputc(ht[htn].value, fout);
    --length.num;
  }

  fclose(fout);
  fclose(fin);

  exit(0);
}
