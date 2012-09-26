#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include <boost/regex.hpp>

using namespace std;
using namespace boost;

const float common_freq[26] = { 0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.0228, 0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.0236, 0.0015, 0.01974, 0.00074 };
const char alphabet[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

FILE* input;
int max_lgramm_length = 10, min_lgramm_length = 3;

char* readKey(const char* src)
{
  char* res = (char*) malloc(sizeof(char)*(strlen(src) - 5));
  res = strcpy(res, src + 6); 
  return res;
}

char* encodeCaesar(char* str, int k)
{
  char* res = (char*) malloc( sizeof(char) * (strlen(str) + 1) );
  for (int i = 0; i < strlen(str); i++)
    if ( str[i] >= 'A' && str[i] <= 'Z' ) 
      res[i] = alphabet[ (str[i] - 'A' + k) % 26 ];
  res[strlen(str)] = '\0';
  return res;
}

char* decodeCaesar(char* str, int k)
{
  char* res = (char*) malloc( sizeof(char) * (strlen(str) + 1) );
  for (int i = 0; i < strlen(str); i++)
    if ( str[i] >= 'A' && str[i] <= 'Z' ) 
      res[i] = alphabet[ (str[i] - 'A' + 26 - k) % 26 ];
  res[strlen(str)] = '\0';
  return res;
}

int hackCaesarKey(char* str, int* key_p)
{
  float calc_freq[26];
  for (int i = 0; i < 26; i++)
    calc_freq[i] = 0.0f;
  for (int i = 0; i < strlen(str); i++)
    calc_freq[ str[i] - 'A' ]++;
  for (int i = 0; i < 26; i++)
  {
    calc_freq[i] /= strlen(str);
    /* printf("%c - %.3f\n", i + 'A', calc_freq[i]); */
  }

  float min_divergence = 999999999.9f;
  int key = 0;
  for (int i = 0; i < 26; i++)
  {
    float divergence = 0.0f;
    for (int j = 0; j < 26; j++)
      divergence += fabs( common_freq[j] - calc_freq[ (j + i) % 26 ] );
    if ( divergence < min_divergence )
    {
      min_divergence = divergence;
      key = i;
    }
  }

  if ( key_p != NULL )
    *key_p = key;
  return key;
}

char* hackCaesar(char* str, int* key_p)
{
  int key = hackCaesarKey(str, key_p);
  return decodeCaesar(str, key);
}

char** splitToBlocks(char* str, int length)
{
  char** res;
  res = (char**) malloc( sizeof(char*) * length );
  int count = 0;
  for (int i = 0; i < length; i++)
    res[i] = (char*) malloc( sizeof(char) * ((int) strlen(str) / length + 2 ) );
  for (int i = 0; i < strlen(str); i++)
  {
    res[ i % length ][count] = str[i];
    if ( i % length == length - 1 )
      count++;
  }
  for (int i = 0; i < ((int) strlen(str) % length); i++)
    res[i][count+1] = '\0';
  for (int i = strlen(str) % length; i < length; i++)
  {
    res[i][count] = '\0'; 
    res[i][count+1] = '\0'; 
  }
  return res;
}

char* uniteBlocks(char** blocks, int length)
{
  int text_length = 0;
  for (int i = 0; i < length; i++)
    text_length += strlen(blocks[i]);
  text_length++;
  char* res = (char*) malloc( sizeof(char) * (text_length + 1) );
  int count = 0;
  for (int i = 0; i < text_length; i++)
  {
    bool end = false;
    for (int j = 0; j < length; j++)
      if ( blocks[j][i] == '\0' )
      {
        end = true;
        break;
      }
      else
        res[count++] = blocks[j][i];

    if ( end )
      break;
  }
  res[count] = '\0';
  return res;
}

char* encodeVigenere(char* str, char* key)
{
  char** blocks = splitToBlocks(str, strlen(key));
  for (int i = 0; i < strlen(key); i++)
    blocks[i] = encodeCaesar(blocks[i], key[i] - 'A');
  char* res = uniteBlocks(blocks, strlen(key));
  return res;
}

char* decodeVigenere(char* str, char* key)
{
  char** blocks = splitToBlocks(str, strlen(key));
  for (int i = 0; i < strlen(key); i++)
    blocks[i] = decodeCaesar(blocks[i], key[i] - 'A');
  char* res = uniteBlocks(blocks, strlen(key));
  return res;
}

int gcd(int a, int b)
{
  while ( b != 0 )
  {
    int tmp = b;
    b = a % b;
    a = tmp;
  }
  return a;
}
int hackKasiskiKeyLength(char* str)
{
  string sstr(str);

  int length[200];
  int count = 0;
  bool* counted = (bool*) malloc( sizeof(bool) * strlen(str) );
  char* pattern = (char*) malloc( sizeof(char) * 20 );
  for (int i = max_lgramm_length; i >= min_lgramm_length; i--)
  {
    printf("finding %d-length l-gramms\n", i);
    sprintf(pattern, "(.{%d}).*?\\1", i);

    string::const_iterator start, end;
    start = sstr.begin();
    end = sstr.end();
    string regex_pattern(pattern);
    regex expression(regex_pattern);
    match_results<string::const_iterator> what;
    match_flag_type flags = boost::match_default;
    while (regex_search(start, end, what, expression, flags))
    {
      if ( !counted[what[0].first - sstr.begin()] )
      {
        length[count++] = what[0].length() - i;
        cout << string(what[0].first, what[0].second) << " - " << length[count-1] << endl;
        for (int j = 0; j < i; j++)
          counted[what[0].first - sstr.begin() + j] = true;
      }

      start = what[0].first + 1;
      flags |= boost::match_prev_avail;
      flags |= boost::match_not_bob;
    }
    printf("----------------------------------------------------------------------------------------------------\n", i);
  }

  // int keylength = 0;
  // if ( count > 0 )
  //   keylength = length[0];
  // for (int i = 1; i < count; i++)
  // {
  //   int backup_keylength = keylength;
  //   keylength = gcd(keylength, length[i]);
  //   if ( keylength == 1 )
  //     keylength = backup_keylength;
  // }
  int max_count = 0;
  int keylength = 1;
  for (int i = 2; i < 100; i++)
  {
    int incl_count = 0;
    for (int j = 1; j < count; j++)
    {
      if ( length[j] % i == 0 )
        incl_count++;
    }

    if ( incl_count * pow(i, 0.5) > max_count * pow(keylength, 0.5) )
    {
      max_count = incl_count;
      keylength = i;
    }
  }

  printf("keylength=%d\n", keylength);
  return keylength;
}

char* hackVigenere(char* str)
{
  int keylength = hackKasiskiKeyLength(str);
  if ( keylength == 0 )
  {
    printf("Couldn't find key length. Terminating\n");
    exit(EXIT_FAILURE);
  }
  char** blocks = splitToBlocks(str, keylength);
  char* key = (char*) malloc( sizeof(char) * (keylength + 1) );
  int lk = 0;
  for (int i = 0; i < keylength; i++)
  {
    int tmp;
    blocks[i] = hackCaesar(blocks[i], &tmp);
    key[lk++] = tmp + 'A';
  }
  key[lk] = '\0';
  char* res = uniteBlocks(blocks, keylength);
  printf("key=%s\n", key);
  return res;
}

int main(int argc, const char *argv[])
{
  if ( argc < 3 )
  {
    fprintf(stderr, "Too few arguments.\nUsage: %s --caesar/vigenere --decode/encode/hack [--key=<key>] [<down_lgramm_limit> <up_lgramm_limit>] infile\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  input = fopen(argv[argc-1], "r");
  if ( input == NULL )
  {
    fprintf(stderr, "%s can't open file: %s\n", argv[0], argv[argc-1]);
    exit(EXIT_FAILURE);
  }

  char str[10024];
  fgets(str, 10024, input);
 
  char* key;

  if ( strcmp(argv[1],"--caesar") == 0 )
  {
    if ( strcmp(argv[2],"--decode") == 0 )
    {
      key = readKey(argv[3]);
      printf("Message=%s\n", decodeCaesar(str, atoi(key)));
    }
    else if ( strcmp(argv[2], "--encode") == 0 )
    {
      key = readKey(argv[3]);
      printf("%s\n", encodeCaesar(str, atoi(key)));
    }
    else if ( strcmp(argv[2], "--hack") == 0 )
    {
      int key;
      char* msg = hackCaesar(str, &key);
      printf("key=%d\n", key);
      printf("Message=%s\n", msg);
    }
  }
  else if ( strcmp(argv[1],"--vigenere") == 0 )
  {
    if ( strcmp(argv[2],"--decode") == 0 )
    {
      key = readKey(argv[3]);
      printf("Message=%s\n", decodeVigenere(str, key));
    }
    else if ( strcmp(argv[2], "--encode") == 0 )
    {
      key = readKey(argv[3]);
      printf("%s\n", encodeVigenere(str, key));
    }
    else if ( strcmp(argv[2], "--hack") == 0 )
    {
      min_lgramm_length = atoi(argv[3]);
      max_lgramm_length = atoi(argv[4]);
      printf("Message=%s\n", hackVigenere(str));
    }
  }

  return 0;
}
