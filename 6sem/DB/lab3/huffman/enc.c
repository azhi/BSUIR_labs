#include <stdio.h>
#include <stdlib.h>

#include "common.h"

// Note: Global arrays are 0 initialized.
struct {
  unsigned long count;
  unsigned char p;
  unsigned char code;
} ht[511];

union {
  unsigned long long num;
  unsigned char arr[8];
} length;

unsigned short get_child(unsigned char p, unsigned char code)
{
  for (unsigned short i = 0; i < 512; ++i)
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
  int ch = 0;
  while ((ch = fgetc(fin)) != EOF) {
    ht[ch].count++;
    length.num++;
  }

  // Finding the alphabet size.
  unsigned short al_sz = 0;
  for (unsigned short i = 0; i < 256; ++i)
    if (ht[i].count)
      ++al_sz;

  // Building the tree.
  for (unsigned short i = 256; i - 255 < al_sz; ++i)
  {
    // Finding 2 rarest chars / nodes
    short mi0 = -1, mi1 = -1;
    for (unsigned short j = 0; j < i; ++j)
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
  unsigned char r = 0, pos = 0; // DEBUG

  // Size of the input file
  for (unsigned char i = 0; i < 8; ++i)
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
  unsigned char tn = al_sz-1, skip_0 = 0;
  if (tn) {
    r = 0;
    pos = 1;
    while (tn) {
      unsigned short chld;

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
        unsigned char l;
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
    short s = ch - 255;
    unsigned short tc = 0;
    unsigned char cs = 0;

    while (ht[s + 255].p) {
      tc <<= 1; tc ^= ht[s + 255].code; ++cs;
      s = ht[s + 255].p;
    }

    // Mirror the char code
    for (unsigned char i = 0; i < cs/2; ++i) {
      tc ^= (tc & (1 << (cs - i - 1))) >> (cs - 2 * i - 1);
      tc ^= (tc & (1 << i))            << (cs - 2 * i - 1);
      tc ^= (tc & (1 << (cs - i - 1))) >> (cs - 2 * i - 1);
    }

#ifdef DEBUG_WRITE
    printf("0 tc: %.2x, cs: %2d r: %.2x, pos: %2d\n", tc, cs, r, pos);
#endif

    while (cs + pos >= 8) {
      r <<= (8 - pos);
      r ^= tc >> (cs + pos - 8);
      tc &= (1 << (cs + pos - 8)) - 1;
      cs -= 8 - pos;

#ifdef DEBUG_WRITE
      printf("Writing: %.2x\n", r);
#endif
      fputc(r, fout);

      r = 0; // DEBUG
      pos = 0;
    }

    r <<= cs;
    r ^= tc;
    pos += cs;
  }

  if (pos != 0)
    fputc(r << (8 - pos), fout);

  fclose(fout);
  fclose(fin);

  exit(0);
}
