#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

// Note: Global arrays are 0 initialized.
struct {
  uint64_t count;
  uint8_t p;
  uint8_t code;
} ht[511];

typedef struct {
  uint8_t v[32];
} my_uint256_t;

union {
  uint64_t num;
  uint8_t arr[8];
} length;

uint16_t get_child(uint8_t p, uint8_t code)
{
  for (uint16_t i = 0; i < 512; ++i)
    if (ht[i].p == p && ht[i].code == code)
      return i;
  die("Sorry, error while dumping the tree.\n"
      "Something went wrong. Please contact authors.\n")
}

int main(int argc, char *argv[])
{
  if (argc != 3)
    die("Sorry, wrong number of arguments.\n"
        "Correct use is:\t%s filename archive\n", argv[0]);

  FILE *fin = fopen(argv[1], "rb");
  if (fin == NULL)
    die("Sorry, cannot open file \"%s.\"\n", argv[1])

  // Collencting character statistics.
  int16_t ch = 0;
  while ((ch = fgetc(fin)) != EOF) {
    ht[ch].count++;
    length.num++;
  }

  // Finding the alphabet size.
  uint16_t al_sz = 0;
  for (uint16_t i = 0; i < 256; ++i)
    if (ht[i].count)
      ++al_sz;

  // Building the tree.
  for (uint16_t i = 256; i - 255 < al_sz; ++i)
  {
    // Finding 2 rarest chars / nodes
    int16_t mi0 = -1, mi1 = -1;
    for (uint16_t j = 0; j < i; ++j)
      if (ht[j].count != 0L && ht[j].p == 0) {
        if (mi0 == -1 || ht[j].count < ht[mi0].count) {
          mi1 = mi0;
          mi0 = j;
        } else if (mi1 == -1 || ht[j].count < ht[mi1].count) {
          mi1 = j;
        }
      }

    // ht[i] is a parent of ht[mi0] and ht[mi1]
    ht[i].count = ht[mi0].count + ht[mi1].count;
    ht[mi1].code = 1;
    ht[mi0].p = ht[mi1].p = i - 255;
  }

  FILE *fout = fopen(argv[2], "wb");
  if (fout == NULL)
    die("Sorry, cannot open file \"%s.\"\n", argv[2]);

  // r - output buffer
  uint8_t r = 0, pos = 0; // DEBUG

  // Size of the input file
  for (uint8_t i = 0; i < 8; ++i)
    fputc(length.arr[i], fout);

  if (length.num <= 1L) {
    if (length.num > 0L)
      fputc(fgetc(fin), fout);

    fclose(fout);
    fclose(fin);
    exit(0);
  } else {
    rewind(fin);
  }

  // Dumping huffman tree
  uint8_t tn = al_sz-1, skip_0 = 0;
  if (tn) {
    r = 0;
    pos = 1;
    while (tn) {
      uint16_t chld;

      if (!skip_0)
        chld = get_child(tn, 0);
      else
        chld = get_child(tn, 1);

#ifdef DEBUG_TREE_DUMP
      printf("%d %.2x %d, r: %.2x, pos: %d\n", tn, chld, skip_0, r, pos);
#endif

      if (chld < 256) {
        r <<= 1;
        r ^= 1;
        ++pos;

        if (pos == 8) {
          fputc(r, fout);
          fputc(chld, fout);
          pos = 0;
          r = 0;
        } else {
          r <<= 8 - pos;
          r ^= chld >> pos;
          fputc(r, fout);
          r = chld;
        }

      } else {
        r <<= 1;
        ++pos;

        if (pos == 8) {
          fputc(r, fout);
          pos = 0;
          r = 0;
        }

        tn = chld - 255;
        skip_0 = 0;
        continue;
      }

      if (skip_0) {
        uint8_t l;
        do {
          l = tn;
          tn = ht[255 + tn].p;
        } while (tn && ht[255 + l].code);
      }
      skip_0 = 1;
    }
  } else {
    ch = 0;
    while (!ht[ch].count)
      ++ch;
    fputc(0x80 | (ch >> 1), fout);
    r = ch;
    pos = 1;
  }

  // Encoding
  while ((ch = fgetc(fin)) != EOF) {
    int16_t s = ch - 255;
    my_uint256_t tc;
    memset(&tc, '\0', sizeof tc); // DEBUG
    uint16_t cs = 0;

    while (ht[s + 255].p) {
      uint8_t lc = ht[s + 255].code;
      for (uint8_t i = 0; i < 32; ++i) {
        uint8_t llc = (tc.v[i] & 0x80) >> 7;
        tc.v[i] <<= 1;
        tc.v[i] |= lc;
        lc = llc;
      }
      ++cs;
      s = ht[s + 255].p;
    }

    //printf("%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x (%d)\n",
    //       tc.v[7], tc.v[6], tc.v[5], tc.v[4], tc.v[3], tc.v[2], tc.v[1], tc.v[0], cs);

    // Mirror the char code
    for (uint8_t i = 0; i < cs/2; ++i) {
      tc.v[i/8]        ^= (tc.v[(cs-i-1)/8] & (1 << ((cs-i-1)%8))) >> ((cs - 2*i - 1) % 8);
      tc.v[(cs-i-1)/8] ^= (tc.v[i/8]        & (1 << i%8))          << ((cs - 2*i - 1) % 8);
      tc.v[i/8]        ^= (tc.v[(cs-i-1)/8] & (1 << ((cs-i-1)%8))) >> ((cs - 2*i - 1) % 8);
    }

#ifdef DEBUG_WRITE
    printf("tc: %.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x (%d)\n",
           tc.v[7], tc.v[6], tc.v[5], tc.v[4], tc.v[3], tc.v[2], tc.v[1], tc.v[0], cs);
#endif

    while (cs + pos >= 8) {
#ifdef DEBUG_WRITE
      printf("Writing. tc[%d]=%.2x, cs=%3d, r=%.2x, pos=%d\n", (cs-1)/8, tc.v[(cs-1)/8], cs, r, pos);
#endif
      if (cs%8 + pos >= 8 || cs%8 == 0) {
        r <<= (8 - pos);
        r ^= tc.v[(cs-1)/8] >> ((cs + pos) % 8);
        tc.v[(cs-1)/8] &= (1 << ((cs + pos) % 8)) - 1;
      } else {
        r <<= cs%8;
        r ^= tc.v[cs/8];
        tc.v[cs/8] = 0; // DEBUG
        pos += cs%8;
        cs -= cs%8;
        continue;
      }

#ifdef DEBUG_WRITE
      printf("Writing: %.2x\n", r);
#endif
      fputc(r, fout);

      cs -= 8 - pos;
      r = 0; // DEBUG
      pos = 0;
    }

    r <<= cs;
    r ^= tc.v[0];
    pos += cs;
  }

  if (pos != 0)
    fputc(r << (8 - pos), fout);

  fflush(fout);
  fclose(fout);
  fclose(fin);

  exit(0);
}
