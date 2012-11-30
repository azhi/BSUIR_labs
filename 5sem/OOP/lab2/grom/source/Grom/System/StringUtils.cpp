// Free open-source Grom library

#include "PrecompiledHeader.h"

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string>
#include <errno.h>
#include "Exception.h"
#include "StringUtils.h"

namespace Sys
{

void ReplaceChar(char* str, char ch, char by)
{
  if (str != NULL)
  {
    char c = *str;
    while (c != 0)
    {
      if (c == ch)
        *str = by;
      ++str;
    }
  }
}

void ReplaceChar(std::string& str, char ch, char by)
{
  for (size_t i = 0; i < str.size(); ++i)
  {
    if (str.at(i) == ch)
      str.at(i) = by;
  }
}

std::string LowerCase(const char* str)
{
  std::string result;
  if (!IsEmpty(str))
  {
    result.resize(strlen(str));
    for (size_t i = 0; i < result.size(); i++)
      result[i] = tolower(str[i]);
  }
  return result;
}

std::string LowerCase(const std::string& str)
{
  std::string result;
  if (str.size() > 0)
  {
    result.resize(str.size());
    for (size_t i = 0; i < result.size(); i++)
      result[i] = tolower(str[i]);
  }
  return result;
}

std::string UpperCase(const char* str)
{
  std::string result;
  if (!IsEmpty(str))
  {
    result.resize(strlen(str));
    for (size_t i = 0; i < result.size(); i++)
      result[i] = toupper(str[i]);
  }
  return result;
}

std::string UpperCase(const std::string& str)
{
  std::string result;
  if (str.size() > 0)
  {
    result.resize(str.size());
    for (size_t i = 0; i < result.size(); i++)
      result[i] = toupper(str[i]);
  }
  return result;
}

std::string SentenceCase(const char* str)
{
  std::string result;
  if (!IsEmpty(str))
  {
    result.resize(strlen(str));
    result[0] = toupper(str[0]);
    for (size_t i = 1; i < result.size(); i++)
      result[i] = tolower(str[i]);
  }
  return result;
}

std::string SentenceCase(const std::string& str)
{
  std::string result;
  if (str.size() > 0)
  {
    result.resize(str.size());
    result[0] = toupper(str[0]);
    for (size_t i = 1; i < result.size(); i++)
      result[i] = tolower(str[i]);
  }
  return result;
}

int CompareStr(const char* str1, const char* str2)
{
  if (!IsEmpty(str1))
    if (!IsEmpty(str2))
      return strcoll(str1, str2);
    else
      return 1;
  else
    if (!IsEmpty(str2))
      return -1;
    else
      return 0;
}

int CompareText(const char* str1, const char* str2)
{
  if (!IsEmpty(str1))
    if (!IsEmpty(str2))
    {
      char buf1[2], buf2[2];
      buf1[1] = buf2[1] = '\0';
      int result;
      do
      {
        buf1[0] = tolower(*str1);
        buf2[0] = tolower(*str2++);
        result = strcoll(buf1, buf2);
      }
      while (*str1++ && (result == 0));
      return result;
    }
    else
      return 1;
  else
    if (!IsEmpty(str2))
      return -1;
    else
      return 0;
}

bool SameStr(const char* str1, const char* str2)
{
  if (!IsEmpty(str1))
    if (!IsEmpty(str2))
      return strcmp(str1, str2) == 0;
    else
      return IsEmpty(str1);
  else
    return IsEmpty(str2);
}

bool SameText(const char* str1, const char* str2)
{
  return CompareText(str1, str2) == 0;
}

bool ContainsChar(const char* str, char ch)
{
  if (str != NULL)
    return strchr(str, ch) != NULL;
  else
    return false;
}

char* CopyStr(const char* source, char* target, int targetSize)
{
  if (source != NULL)
  {
    strncpy(target, source, targetSize - 1);
    target[targetSize - 1] = '\0';
  }
  else
    target[0] = '\0';
  return target;
}

static bool IsSpace(char c)
{
  switch (c)
  {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      return true;
  }
  return false;
}

bool IsBlank(const char* str)
{
  if (IsEmpty(str))
    return true;
  while ( (*str) != '\0' && IsSpace(*str) )
    str++;
  if ( (*str) == '\0' )
    return true;
  else
    return false;
}

bool IsBlank(const char* str, int length)
{
  if (IsEmpty(str))
    return true;
  for (int i = 0; i < length; ++i)
    if ( !IsSpace(*(str+i)) )
      return false;
  return true;
}

std::string TrimSpaces(const char* str)
{
  std::string result;
  if (!IsEmpty(str))
  {
    while (IsSpace(*str))
      ++str;
    const char* end = str + strlen(str) - 1;
    while (IsSpace(*end))
      --end;
    if (end >= str)
      result.assign(str, end + 1);
  }
  return result;
}

const char* SkipChars(const char* str, const char* chars)
{
  if (!IsEmpty(str) && !IsEmpty(chars))
  {
    while (strchr(chars, *str) != NULL && *str != '\0')
      ++str;
  }
  return str;
}

const char* FindChars(const char* str, const char* chars)
{
  if (!IsEmpty(str))
  {
    while (strchr(chars, *str) == NULL && *str != '\0')
      ++str;
  }
  return str;
}

std::string GetNextToken(const char** str, const char* delimiters, const char* spaces)
{
  std::string result;
  if (str != NULL)
  {
    const char* s = SkipChars(*str, spaces);
    const char* t = FindChars(s, delimiters);
    const char* r = t;
    if (!IsEmpty(t))
    {
      while (t > s && ContainsChar(spaces, *(t - 1)))
      {
        --t;
      }
    }
    result.assign(s, t);
    *str = SkipChars(r, delimiters);
  }
  else
    throw CreateArgumentNullException();
  return result;
}

std::string StrFromInt(int value)
{
  return FormattedString("%d", value);
}

int IntFromStr(const char* value)
{
  int result = atoi(value);
  if (errno == ERANGE)
    throw CreateArgumentOutOfRangeException();
  return result;
}

int IntFromStrDef(const char* value, int defaultResult)
{
  int result = atoi(value);
  if (errno == ERANGE)
    result = defaultResult;
  return result;
}

#ifdef CFG_WIN32
std::string MultiByteStringFromWideString(const wchar_t* str)
{
  std::string result;
  if (str != NULL)
  {
#ifdef CFG_COMPILER_MSC_80_2005
    // Eliminate warning C4996: 'wcstombs' was declared deprecated
    size_t n = 0;
    if (wcstombs_s(&n, NULL, 0, str, 0) == 0 && n != -1 && n > 0)
    {
      result.resize(n);
      if (wcstombs_s(&n, &result.at(0), n, str, n) == 0)
        result.resize(n - 1); // truncate terminating NULL
      else
        result.clear();
    }
#else
    size_t n = wcstombs(NULL, str, 0);
    if (n != -1 && n > 0)
    {
      result.resize(n);
      wcstombs(&result.at(0), str, n);
    }
#endif
  }
  return result;
}

std::basic_string<wchar_t> WideStringFromMultiByteString(const char* str)
{
  std::basic_string<wchar_t> result;
  if (str != NULL)
  {
#ifdef CFG_COMPILER_MSC_80_2005
    // Eliminate warning C4996: 'mbstowcs' was declared deprecated
    size_t n = 0;
    if (mbstowcs_s(&n, NULL, 0, str, 0) == 0 && n != -1 && n > 0)
    {
      result.resize(n);
      if (mbstowcs_s(&n, &result.at(0), n, str, n) == 0)
        result.resize(n - 1); // truncate terminating NULL
      else
        result.clear();
    }
#else
    size_t n = mbstowcs(NULL, str, 0);
    if (n != -1 && n > 0)
    {
      result.resize(n);
      mbstowcs(&result.at(0), str, n);
    }
#endif
  }
  return result;
}
#endif // #ifdef CFG_WIN32

// FormattedString

FormattedString::FormattedString()
{
}

FormattedString::FormattedString(const char* format, ...)
{
  va_list argList;
  va_start(argList, format);
  DoFormatArgs(format, argList);
  va_end(argList);
}

void FormattedString::DoFormat(const char* format, ...)
{
  va_list argList;
  va_start(argList, format);
  DoFormatArgs(format, argList);
  va_end(argList);
}

inline
static int FormatArgs(char* buf, size_t size, const char* format, va_list argList)
{
#ifdef CFG_WIN32
#  ifdef CFG_COMPILER_MSC_60_1998
  int res = _vsnprintf(buf, size, format, argList);
  return (res >= 0) ? res : (2 * res);
#  endif
#  ifdef CFG_COMPILER_MSC_71_2003
  int res = _vsnprintf(buf, size, format, argList);
  return (res >= 0) ? res : _vscprintf(format, argList);
#  endif
#  ifdef CFG_COMPILER_MSC_80_2005
  int res = vsnprintf_s(buf, size, _TRUNCATE, format, argList);
  return (res >= 0) ? res : _vscprintf(format, argList);
#  endif
#else
  return vsnprintf(buf, size, format, argList);
#endif
}

void FormattedString::DoFormatArgs(const char* format, va_list argList)
{
  size_t size = 256;
  char* buf = (char*)malloc(size);
  if (buf != NULL)
  {
    try
    {
      size_t n = FormatArgs(buf, size, format, argList);
      while (n >= size)
      {
        size = n + 1;
        buf = (char*)realloc(buf, size);
        if (buf != NULL)
          n = FormatArgs(buf, size, format, argList);
        else
          break;
      }
      if (buf != NULL)
        assign(buf, n);
    }
    catch (...)
    {
      if (buf != NULL)
        free(buf);
      throw;
    }
    if (buf != NULL)
      free(buf);
  }
}

// MultiString

MultiString::MultiString()
{
}

MultiString::MultiString(const MultiString& a) :
  m_words(a.m_words)
{
  assign(a);
}

MultiString::MultiString(const char* str, const char* delimiters, const char* spaces)
{
  size_t a = 0;
  size_t len = strlen(str);
  assign(str, len + 1); // including null-terminator
  while (a < len)
  {
    if (spaces != NULL)
      while (a < len && strchr(spaces, at(a)) != NULL)
        ++a; // skip spaces

    size_t delim = a;
    if (delimiters != NULL)
    {
      while (delim < len && strchr(delimiters, at(delim)) == NULL)
        ++delim; // find delimiter
    }
    else
      delim = len;

    size_t b = delim;
    if (spaces != NULL)
      while (b > a && strchr(spaces, at(b - 1)) != NULL)
        --b; // skip spaces before delimiter

    m_words.push_back(a);
    if (b < len)
      at(b) = '\0';

    a = delim + 1; // next word
  }
}

int MultiString::word_count() const
{
  return (int)m_words.size();
}

const char* MultiString::word(int index) const
{
  return &at(m_words[index]);
}

} // namespace Sys
