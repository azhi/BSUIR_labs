// Free open-source Grom library

#include "PrecompiledHeader.h"

#include <errno.h>
#include <time.h>
#include "ErrorMessages.h"
#include "StringUtils.h"
#include "Exception.h"

namespace Sys
{

// Exception

Exception::Exception(const std::string& message) :
  m_Message(message), m_InnerException(NULL)
{
}

Exception::Exception(const std::string& message, Exception* innerException) :
  m_Message(message), m_InnerException(innerException)
{
}

Exception::~Exception() throw()
{
  if (m_InnerException != NULL)
    delete m_InnerException;
}

const char* Exception::what() const throw()
{
  return m_Message.c_str();
}

// SystemException

SystemException::SystemException(const std::string& message) :
  Exception(message)
{
}

SystemException::SystemException(const std::string& message, Exception* innerException) :
  Exception(message, innerException)
{
}

// RtlException

RtlException::RtlException(int errNo, const std::string& message) :
  m_ErrNo(errNo), SystemException(message)
{
}

RtlException::RtlException(int errNo, const std::string& message, Exception* innerException) :
  m_ErrNo(errNo), SystemException(message, innerException)
{
}

// ApplicationException

ApplicationException::ApplicationException(int errorCode, const std::string& message) :
  Exception(message), m_ErrorCode(errorCode)
{
}

ApplicationException::ApplicationException(int errorCode, const std::string& message, Exception* innerException) :
  Exception(message, innerException), m_ErrorCode(errorCode)
{
}

// InvalidCastException

InvalidCastException::InvalidCastException(const std::string& message) :
  SystemException(message)
{
}

InvalidCastException::InvalidCastException(const std::string& message, Exception* innerException) :
  SystemException(message, innerException)
{
}

// ArgumentException

ArgumentException::ArgumentException(const std::string& message) :
  SystemException(message)
{
}

ArgumentException::ArgumentException(const std::string& message, Exception* innerException) :
  SystemException(message, innerException)
{
}

// IOException

IOException::IOException(const std::string& message) :
  SystemException(message)
{
}

IOException::IOException(const std::string& message, Exception* innerException) :
  SystemException(message, innerException)
{
}

// ExternalException

ExternalException::ExternalException(const std::string& message) :
  SystemException(message)
{
}

ExternalException::ExternalException(const std::string& message, Exception* innerException) :
  SystemException(message, innerException)
{
}

// ExceptionString

ExceptionString::ExceptionString(const Exception* e, const char* delimiter) : FormattedString()
{
  DoFormat(e, delimiter);
}

void ExceptionString::DoFormat(const Exception* e, const char* delimiter)
{
  try
  {
    assign(e->Message());
    while (e->InnerException() != NULL)
    {
      e = e->InnerException();
      append(delimiter);
      append(e->Message());
    }
  }
  catch (...)
  {
    erase();
  }
}

// Utilities

SystemException* CreateNotImplementedException()
{
  return new SystemException("the called function is not implemented");
}

inline
void strerror_x(int errNo, std::string* result)
{
#ifdef CFG_COMPILER_MSC_80_2005
  // Eliminate warning C4996: 'strerror' was declared deprecated
  result->assign(128, '\0');
  if (strerror_s(&result->at(0), result->size() - 1, errNo) != 0)
      result->assign("unknown error (cannot get error message)");
#else
  *result = strerror(errNo);
#endif
}

RtlException* CreateRtlException(const char* hint)
{
  std::string msg;
  strerror_x(errno, &msg);
  if (!IsEmpty(hint))
    return new RtlException(errno, FormattedString("%s: %s", hint, msg.c_str()));
  else
    return new RtlException(errno, FormattedString("%s", msg.c_str()));
}

#ifdef CFG_WIN32
void ThrowWin32FileException(const char* filePath)
{
  DWORD err = GetLastError();
  if (ERROR_SUCCESS != err)
  {
    switch (err)
    {
      case ERROR_FILE_NOT_FOUND:
      case ERROR_PATH_NOT_FOUND:
        throw new IOException(FormattedString(ERRMSG_FILE_NOT_FOUND, filePath));
      case ERROR_ACCESS_DENIED:
        throw new IOException(FormattedString(ERRMSG_CANNOT_ACCESS_FILE, filePath));
      case ERROR_DISK_FULL:
        throw new IOException(FormattedString(ERRMSG_NOT_ENOUGH_DISK_SPACE, filePath));
      case ERROR_BAD_PATHNAME:
        throw new IOException(FormattedString(ERRMSG_INVALID_FILE_NAME, filePath));
      default:
        throw new IOException(FormattedString(ERRMSG_CANNOT_ACCESS_FILE, filePath));
    }
  }
}
#endif

InvalidCastException* CreateInvalidCastException(const char* from, const char* to)
{
  return new InvalidCastException(FormattedString("can't cast %s to %s", from, to));
}

ArgumentException* CreateIndexOutOfBoundsException(int index)
{
  return new ArgumentException(FormattedString("specified index %d is out of array bounds", index));
}

ArgumentException* CreateRangeOutOfBoundsException(int index, int count)
{
  return new ArgumentException(FormattedString("specified range (index=%d, count=%d) is out of array bounds", index, count));
}

ArgumentException* CreateArgumentNullException()
{
  return new ArgumentException("argument cannot be null");
}

ArgumentException* CreateArgumentOutOfRangeException()
{
  return new ArgumentException("specified argument was out of the range of valid values");
}

ArgumentException* CreateItemCountNegativeException(int value)
{
  return new ArgumentException(FormattedString("item count cannot be negative (%d)", value));
}

HRESULT CheckSafeCallResult(HRESULT resultCode)
{
#ifdef CFG_WIN32
  if (resultCode < 0)
    throw CreateExternalException();
#endif // CFG_WIN32
  return resultCode;
}

// On Windows platform converts exception to external error info. Exception
// object passed as an argument is deleted before return. Returns
// error code, which should be returned by a calling procedure.
// On Unix platform the function simply re-throws exception.
HRESULT ConvertExceptionToComError(Exception* e, const char* source)
{
#ifdef CFG_WIN32
  //int errCode = e->ErrCode();
  int errCode = EXTERNAL_ERROR;
  std::string msg = ExceptionString(e);
#ifdef _DEBUG
  if (!IsEmpty(source))
    msg += FormattedString("\nSource: %s", source);
#endif // DEBUG
  CreateComErrorInfo(msg.c_str(), source, NULL, 0);
  delete e;
  return errCode;
#endif // CFG_WIN32
#ifdef API_POSIX
  throw e;
#endif // API_POSIX
}

#ifdef CFG_WIN32

// The CreateExternalException function uses GetErrorInfo function to get 
// COM error information and then creates Exception with the error description.
ExternalException* CreateExternalException()
{
  IErrorInfo* errInfo;
  BSTR descriptionBStr;
  std::string descriptionStr;
  if (GetErrorInfo(0, &errInfo) == S_OK)
  {
    errInfo->GetDescription(&descriptionBStr);
    descriptionStr = MultiByteStringFromWideString(descriptionBStr);
    ::SysFreeString(descriptionBStr);
  }
  return new ExternalException(descriptionStr);
}

// The CreateComErrorInfo function uses CreateErrorInfo to create error
// object, which is then filled with the error information and passed to
// the acceptor by a call to SetErrorInfo.
void CreateComErrorInfo(const char* description, const char* source,
  const char* helpFile, UInt32 helpContext)
{
  ICreateErrorInfo* createErrInfo;
  IErrorInfo* errInfo;
  std::basic_string<wchar_t> buf;
  if (CreateErrorInfo(&createErrInfo) == S_OK)
  {
    createErrInfo->SetGUID(IID_IErrorInfo);
    if (description != NULL)
    {
      buf = WideStringFromMultiByteString(description);
      createErrInfo->SetDescription(const_cast<wchar_t*>(buf.c_str()));
    }
    if (source != NULL)
    {
      buf = WideStringFromMultiByteString(source);
      createErrInfo->SetSource(const_cast<wchar_t*>(buf.c_str()));
    }
    if (helpFile != NULL)
    {
      buf = WideStringFromMultiByteString(helpFile);
      createErrInfo->SetHelpFile(const_cast<wchar_t*>(buf.c_str()));
    }
    createErrInfo->SetHelpContext(helpContext);
    if (createErrInfo->QueryInterface(IID_IErrorInfo, (void**)(&errInfo)) == S_OK)
    {
      SetErrorInfo(0, errInfo);
      errInfo->Release();
    }
    createErrInfo->Release();
  }
}

#endif // CFG_WIN32

} // namespace Sys
