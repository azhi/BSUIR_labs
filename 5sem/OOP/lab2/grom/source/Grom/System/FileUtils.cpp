// Free open-source Grom library

#include "PrecompiledHeader.h"

#include <string>
#include <assert.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#ifdef CFG_WIN32
#  include <direct.h>
#endif
#ifdef CFG_POSIX
#  include <dirent.h>
#  include <unistd.h>
#endif
#include "ValueArray.h"
#include "StringUtils.h"
#include "Utils.h"
#include "FileUtils.h"

namespace Sys
{

#ifdef CFG_WIN32
  const char* FILE_PATH_DELIMITERS = "\\/";
#endif
#ifdef CFG_POSIX
  const char* FILE_PATH_DELIMITERS = "/";
#endif

const char* URL_PATH_DELIMITERS = "/";

static bool IsServicePath(const char* path)
{
  if (path != NULL)
  {
    if (::strcmp(path, ".") == 0 || ::strcmp(path, "..") == 0)
      return true;
  }
  return false;
}

std::string TrimPathDelimiter(const char* path)
{
  std::string result;
  if (!IsEmpty(path))
  {
    const char* last = path + strlen(path) - 1;
    while (last > path && IsPathDelimiter(*last))
      --last;
    result.assign(path, last - path + 1);
  }
  return result;
}

inline
char* getcwd_x(char* buf, size_t size)
{
#ifdef CFG_COMPILER_MSC_80_2005
  // Eliminate warning C4996: 'getcwd' was declared deprecated
  return ::_getcwd(buf, (int)size);
#else
  return ::getcwd(buf, (int)size);
#endif
}

std::string CurrentDir()
{
  std::string buf;
  buf.resize(32);
  char* t = getcwd_x(&buf[0], buf.size());
  while (t == NULL && errno == ERANGE)
  {
    buf.resize(buf.size() * 2);
    t = getcwd_x(&buf[0], buf.size());
  }
  buf.resize(strlen(buf.c_str()));
  return buf;
}

inline
int chdir_x(const char* dir)
{
#ifdef CFG_COMPILER_MSC_80_2005
  // Eliminate warning C4996: 'chdir' was declared deprecated
  return ::_chdir(dir);
#else
  return ::chdir(dir);
#endif
}

void SetCurrentDir(const char* dir)
{
  if (chdir_x(dir) != 0)
    throw CreateRtlException(dir);
}

bool DirExists(const char* dir)
{
  std::string s = TrimPathDelimiter(dir);
  struct stat t;
  return (stat(s.c_str(), &t) == 0) && (t.st_mode & S_IFDIR);
}

static int mkdir_x(const char* dir)
{
#ifdef CFG_WIN32
#  ifdef CFG_COMPILER_MSC_80_2005
  // Eliminate warning C4996: 'mkdir' was declared deprecated
  return ::_mkdir(dir);
#  else
  return ::mkdir(dir);
#  endif
#endif
#ifdef CFG_POSIX
  return ::mkdir(dir, S_IRUSR | S_IWUSR | S_IROTH |
    S_IRGRP | S_IXOTH | S_IXUSR | S_IXGRP);
#endif
}

void CreateDir(const char* dir)
{
  if (dir != NULL)
  {
    if (mkdir_x(dir) != 0)
      throw CreateRtlException(dir);
  }
  else
    throw CreateArgumentNullException();
}

void ForceDir(const char* dir)
{
  if (dir != NULL)
  {
    const char* startDir = dir;
#if defined(CFG_OS_WINDOWS)
    // Skip "X:" (if dir is Windows file path)
    if (startDir[0] != '\0' && startDir[1] == ':')
      startDir += 2;
#endif
    // Skip \\ and // (if dir is network path)
    while (IsPathDelimiter(*startDir))
      ++startDir;
    // Split path into parts and add server name if dir is network path
    MultiString dirs(startDir, FILE_PATH_DELIMITERS, NULL);
    std::string path(dir, startDir - dir);
    int i = 0;
    if (startDir != dir && dirs.word_count() > 0) // if dir is network path
    {
      path.append(dirs.word(0));
      i = 1;
    }
    // Create directories
    while (i < dirs.word_count())
    {
      path = JoinPath(path.c_str(), dirs.word(i));
      if (!DirExists(path.c_str()))
        CreateDir(path.c_str());
      ++i;
    }
  }
  else
    throw CreateArgumentNullException();
}

static int rmdir_x(const char* dir)
{
#ifdef CFG_COMPILER_MSC_80_2005
  // Eliminate warning C4996: 'mkdir' was declared deprecated
  return ::_rmdir(dir);
#else
  return ::rmdir(dir);
#endif
}

bool TryDeleteDir(const char* dir, bool recursively)
{
  if (recursively)
  {
    std::string t = RealPath(dir);
    TValueArray<std::string> files;
    GetFilesAndDirs(t.c_str(), false, &files);
    for (int i = 0; i < files.Count(); ++i)
    {
      const char* t = files[i].c_str();
      if (IsDir(t))
        DeleteDir(t, true);
      else
        DeleteFile(t);
    }
  }
  return rmdir_x(dir) == 0;
}

void DeleteDir(const char* dir, bool recursively)
{
  if (!TryDeleteDir(dir, recursively))
    throw CreateRtlException(dir);
}

bool IsDir(const char* path)
{
  struct stat t;
  if (stat(path, &t) == 0)
    return (t.st_mode & S_IFDIR) != 0;
  else
    return false;
}

bool IsAbsolutePath(const char* path)
{
  bool result = false;
  if (path != NULL)
  {
    result = IsPathDelimiter(path[0]);
#if defined(CFG_OS_WINDOWS)
    result = result ||
      (path[0] != '\0' && path[1] == ':' && IsPathDelimiter(path[2]));
#endif
  }
  return result;
}

bool IsNetworkPath(const char* path)
{
  return path != NULL && IsPathDelimiter(path[0]) && IsPathDelimiter(path[1]);
}

std::string JoinPath(const char* head, const char* tail)
{
  std::string result;
  if (IsAbsolutePath(tail))
    result.assign(tail);
  else
  {
    if (!IsEmpty(head))
    {
      result.assign(head);
      if (!IsPathDelimiter(result.c_str()[result.size()]))
        result.append(FILE_PATH_DELIMITERS, 1); // use first delimiter from FILE_PATH_DELIMITERS
      if (!IsEmpty(tail))
        result.append(tail);
    }
    else if (!IsEmpty(tail))
      result.assign(tail);
  }
  return result;
}

std::string RealPath(const char* path)
{
  std::string s = Sys::JoinPath(CurrentDir().c_str(), path);
  return NormalizePath(s.c_str());
}

static bool ExtractUnixLikePathPrefix(const char* path, std::string* prefix)
{
  // Find leading slashes
  const char* t = path;
  if (IsPathDelimiter(*t)) // "/", "\"
  {
    ++t;
    if (IsPathDelimiter(*t)) // "//", "\\"
      ++t;
    prefix->assign(path, t - path);
    if (t - path > 1) // if "//" or "\\" then find and append server name
    {
      const char* p = t;
      std::string server = GetNextToken(&p, FILE_PATH_DELIMITERS, FILE_PATH_DELIMITERS);
      prefix->append(server);
      if (t[server.size()] != '\0')
        prefix->append(t + server.size(), 1);
    }
  }
  else
    prefix->clear();
  return t != path;
}

bool ExtractPathPrefix(const char* path, std::string* prefix)
{
#if defined(CFG_OS_WINDOWS)
  bool result;
  const char* t = path;
  if (t[0] != '\0' && t[1] == ':') // "C:"
  {
    t += 2;
    if (IsPathDelimiter(*t)) // "C:/", "C:\"
      ++t;
    prefix->assign(path, t - path);
    result = true;
  }
  else
    result = ExtractUnixLikePathPrefix(path, prefix);
  return result;
#else
  return ExtractUnixLikePathPrefix(path, prefix);
#endif
}

std::string NormalizePath(const char* path)
{
  std::string result;
  const char* t = path;
  // Handle path prefixes: C:, C:/, /, //server/
  if (ExtractPathPrefix(path, &result))
  {
    t += result.size();
#ifdef CFG_OS_WINDOWS
    ReplaceChar(result, '/', '\\');
#endif
  }
  // Split path into elements
  std::string token;
  TValueArray<std::string> elements;
  MultiString ms(t, FILE_PATH_DELIMITERS, NULL);
  for (int i = 0; i < ms.word_count(); ++i)
  {
    token = ms.word(i);
    if (!token.empty() && token != ".")
    {
      if (token == "..")
      {
        if (elements.Count() > 0 && elements[elements.Count() - 1] != "..")
          elements.SetCount(elements.Count() - 1);
        else if (result.empty())
          elements.Add(token);
        else
          throw new ArgumentException(FormattedString(
            "path '%s' cannot be normalized", path));
      }
      else
        elements.Add(token);
    }
  }
  // Join remaining elements
  if (elements.Count() > 0)
  {
    result.append(elements.Item(0));
    for (int i = 1; i < elements.Count(); ++i)
    {
      result.append(1, FILE_PATH_DELIMITERS[0]);
      result.append(elements.Item(i));
    }
  }
  return result;
}

void ExtractDir(const char* path, std::string* result)
{
  result->clear();
  if (path != NULL)
  {
    const char* p = path;
    const char* q = path;
    while (*p != '\0')
    {
      if (IsPathDelimiter(*p))
        q = p;
      ++p;
    }
    result->assign(path, q - path);
  }
}

void ExtractFileName(const char* path, std::string* result)
{
  result->clear();
  if (path != NULL)
  {
    MultiString s(path, FILE_PATH_DELIMITERS, NULL);
    int n = s.word_count();
    if (n > 0)
      *result = s.word(n - 1);
  }
}

bool FileExists(const char* path)
{
  struct stat t;
  return (stat(path, &t) == 0) && (t.st_mode & S_IFREG);
}

time_t FileAge(const char* path)
{
  struct stat t;
  if (stat(path, &t) == 0)
    return t.st_mtime;
  else
    return -1;
}

void DeleteFile(const char* path)
{
  if (remove(path) != 0)
    throw CreateRtlException(path);
}

void RenameFile(const char* oldPath, const char* newPath)
{
    if (rename(oldPath, newPath) != 0)
    {
        std::string hint = Sys::FormattedString("'%s' -> '%s'", oldPath, newPath);
        throw CreateRtlException(hint.c_str());
    }
}

void GetFiles(const char* dir, bool recursively, TValueArray<std::string>* result)
{
  FileEnumerator fe(dir, recursively);
  result->Clear();
  while (fe.MoveNext())
    if (!IsDir(fe.CurrentPath()))
      result->Add(std::string(fe.CurrentPath()));
}

void GetDirs(const char* dir, bool recursively, TValueArray<std::string>* result)
{
  FileEnumerator fe(dir, recursively);
  result->Clear();
  while (fe.MoveNext())
    if (IsDir(fe.CurrentPath()))
      result->Add(std::string(fe.CurrentPath()));
}

void GetFilesAndDirs(const char* dir, bool recursively, TValueArray<std::string>* result)
{
  FileEnumerator fe(dir, recursively);
  result->Clear();
  while (fe.MoveNext())
    result->Add(std::string(fe.CurrentPath()));
}

// NonRecursiveFileEnumerator

class NonRecursiveFileEnumerator
{
public:
  NonRecursiveFileEnumerator(NonRecursiveFileEnumerator* parent, const char* startupDir);
  ~NonRecursiveFileEnumerator();

  NonRecursiveFileEnumerator* Parent();
  const char* StartupDir();
  const char* CurrentFileName();
  bool MoveNext();
  bool Bof();
  bool Eof();

private:
  bool DoMoveNext();
  void Finalize();

  NonRecursiveFileEnumerator* m_Parent;
  std::string m_StartupDir;
#ifdef CFG_WIN32
  WIN32_FIND_DATA* m_Data;
  HANDLE m_Handle;
#endif
#ifdef CFG_POSIX
  dirent* m_Data;
  DIR* m_Handle;
#endif
};

#ifdef CFG_WIN32
static const HANDLE NULL_HANDLE = INVALID_HANDLE_VALUE;
#endif
#ifdef CFG_POSIX
static DIR* const NULL_HANDLE = NULL;
#endif

NonRecursiveFileEnumerator::NonRecursiveFileEnumerator(NonRecursiveFileEnumerator* parent, const char* startupDir) :
  m_Parent(parent), m_Data(NULL), m_Handle(NULL_HANDLE)
{
  m_StartupDir = startupDir;
#ifdef CFG_WIN32
  m_Data = new WIN32_FIND_DATA;
  // Actual opening occurs in the MoveNext method
#endif
#ifdef CFG_POSIX
  m_Handle = opendir(startupDir);
#endif
}

NonRecursiveFileEnumerator::~NonRecursiveFileEnumerator()
{
  Finalize();
}

void NonRecursiveFileEnumerator::Finalize()
{
  if (m_Handle != NULL_HANDLE)
  {
#ifdef CFG_WIN32
    FindClose(m_Handle);
#endif
#ifdef CFG_POSIX
    closedir(m_Handle);
#endif
  }
  m_Handle = NULL_HANDLE;

#ifdef CFG_WIN32
  delete m_Data;
  m_Data = NULL;
#endif
}

inline
NonRecursiveFileEnumerator* NonRecursiveFileEnumerator::Parent()
{
  return m_Parent;
}

const char* NonRecursiveFileEnumerator::StartupDir()
{
  return m_StartupDir.c_str();
}

const char* NonRecursiveFileEnumerator::CurrentFileName()
{
  assert(!Bof());
  if (m_Data != NULL)
  {
#ifdef CFG_WIN32
    return m_Data->cFileName;
#endif
#ifdef CFG_POSIX
    return m_Data->d_name;
#endif
  }
  else
     return "";
}

bool NonRecursiveFileEnumerator::MoveNext()
{
  bool result = DoMoveNext();
  // skip "." and ".." entries
  while (result && IsServicePath(CurrentFileName()))
  {
    result = DoMoveNext();
  }
  return result;
}

bool NonRecursiveFileEnumerator::DoMoveNext()
{
  bool result = false;
#ifdef CFG_WIN32
  if (m_Data != NULL)
  {
    if (m_Handle == NULL_HANDLE)
    {
      std::string t = JoinPath(m_StartupDir.c_str(), "*"); // Windows always expects wildcard
      m_Handle = FindFirstFile(t.c_str(), m_Data);
      result = m_Handle != NULL_HANDLE;
    }
    else
      result = FindNextFile(m_Handle, m_Data) == TRUE;
  }
#endif
#ifdef CFG_POSIX
  if (m_Handle != NULL_HANDLE)
  {
    m_Data = readdir(m_Handle);
    result = m_Data != NULL;
  }
#endif
  if (!result)
    Finalize();
  return result;
}

bool NonRecursiveFileEnumerator::Bof()
{
#ifdef CFG_WIN32
  return m_Data != NULL && m_Handle == NULL_HANDLE;
#endif
#ifdef CFG_POSIX
  return m_Data == NULL && m_Handle != NULL_HANDLE;
#endif
}

bool NonRecursiveFileEnumerator::Eof()
{
  return m_Data == NULL && m_Handle == NULL_HANDLE;
}

#define DIRENUM_DEFAULT_FLAGS  0
#define DIRENUM_RECURSIVE_MODE 1
#define DIRENUM_CURRENT_CACHED 2

// FileEnumerator

FileEnumerator::FileEnumerator() : 
  m_StartupDir(), m_CurrentNonRecursiveEnumerator(NULL),
  m_Flags(DIRENUM_DEFAULT_FLAGS), m_Current()
{
}

FileEnumerator::FileEnumerator(const char* path, bool recursive) : 
  m_StartupDir(path), m_CurrentNonRecursiveEnumerator(NULL),
  m_Flags(DIRENUM_DEFAULT_FLAGS), m_Current()
{
  SetRecursive(recursive);
}

FileEnumerator::~FileEnumerator()
{
  Reset();
}

bool FileEnumerator::Recursive()
{
  return (m_Flags & DIRENUM_RECURSIVE_MODE) != 0;
}

void FileEnumerator::SetRecursive(bool value)
{
  if (value)
    m_Flags |= DIRENUM_RECURSIVE_MODE;
  else
    m_Flags &= ~DIRENUM_RECURSIVE_MODE;
}

void FileEnumerator::SetStartupDir(const char* value)
{
  Reset();
  m_StartupDir = value;
}

void FileEnumerator::Reset()
{
  while (m_CurrentNonRecursiveEnumerator != NULL)
  {
    NonRecursiveFileEnumerator* p = m_CurrentNonRecursiveEnumerator;
    m_CurrentNonRecursiveEnumerator = p->Parent();
    delete p;
  }
  m_Flags &= ~DIRENUM_CURRENT_CACHED;
  m_Current.clear();
}

bool FileEnumerator::MoveNext()
{
  bool result = false;
  if (!m_StartupDir.empty())
  {
    if (m_CurrentNonRecursiveEnumerator == NULL)
      m_CurrentNonRecursiveEnumerator = new NonRecursiveFileEnumerator(NULL, m_StartupDir.c_str());
    // Enter directory if mode is recursive
    if ((m_Flags & DIRENUM_RECURSIVE_MODE) != 0 && 
      !m_CurrentNonRecursiveEnumerator->Bof() && 
      !m_CurrentNonRecursiveEnumerator->Eof() && 
      IsDir(CurrentPath()))
    {
      //std::string s = m_EntriesEnumerator->StartupDir();
      std::string s = JoinPath(CurrentDir(), m_CurrentNonRecursiveEnumerator->CurrentFileName());
      m_CurrentNonRecursiveEnumerator = new NonRecursiveFileEnumerator(m_CurrentNonRecursiveEnumerator, s.c_str());
    }
    // Find next entry
    result = m_CurrentNonRecursiveEnumerator->MoveNext();
    while (!result && m_CurrentNonRecursiveEnumerator->Parent() != NULL)
    {
      // Go to the parent directory
      NonRecursiveFileEnumerator* p = m_CurrentNonRecursiveEnumerator;
      m_CurrentNonRecursiveEnumerator = p->Parent();
      delete p;
      result = m_CurrentNonRecursiveEnumerator->MoveNext();
    }
    m_Flags &= ~DIRENUM_CURRENT_CACHED;
  }
  return result;
}

const char* FileEnumerator::CurrentPath()
{
  if ((m_Flags & DIRENUM_CURRENT_CACHED) == 0)
  {
    m_Current = JoinPath(CurrentDir(), CurrentFileName());
    m_Flags |= DIRENUM_CURRENT_CACHED;
  }
  return m_Current.c_str();
}

const char* FileEnumerator::CurrentFileName()
{
  Sys::RunTimeAssert(m_CurrentNonRecursiveEnumerator != NULL);
  if (m_CurrentNonRecursiveEnumerator != NULL)
    return m_CurrentNonRecursiveEnumerator->CurrentFileName();
  else
    return "";
}

const char* FileEnumerator::CurrentDir()
{
  Sys::RunTimeAssert(m_CurrentNonRecursiveEnumerator != NULL);
  if (m_CurrentNonRecursiveEnumerator != NULL)
    return m_CurrentNonRecursiveEnumerator->StartupDir();
  else
    return "";
}

// TempCurrentDir

TempCurrentDir::TempCurrentDir(const char* dir)
{
  if (!IsEmpty(dir))
  {
    m_SavedDir = CurrentDir();
    SetCurrentDir(dir);
  }
}

TempCurrentDir::~TempCurrentDir()
{
  if (!m_SavedDir.empty())
    SetCurrentDir(m_SavedDir.c_str());
}

} // namespace Sys
