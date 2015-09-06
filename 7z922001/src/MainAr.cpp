// MainAr.cpp

#include "StdAfx.h"
#include "MainAr.h"									// �ǉ�

#include "Common/MyException.h"
#include "Common/StdOutStream.h"

#include "Windows/Error.h"
#include "Windows/NtCheck.h"

#include "../../UI/Common/ArchiveCommandLine.h"	// �p�X�ύX
#include "../../UI/Common/ExitCode.h"				// �p�X�ύX

#include "../../UI/Console/ConsoleClose.h"			// �p�X�ύX

using namespace NWindows;

CStdOutStream *g_StdStream = 0;

extern int Main2(
  #ifndef _WIN32
  int numArgs, const char *args[]
  #endif
);

static const char *kExceptionErrorMessage = "\n\nError:\n";
static const char *kUserBreak  = "\nBreak signaled\n";
static const char *kMemoryExceptionMessage = "\n\nERROR: Can't allocate required memory!\n";
static const char *kUnknownExceptionMessage = "\n\nUnknown Error\n";
static const char *kInternalExceptionMessage = "\n\nInternal Error #";

#define NT_CHECK_FAIL_ACTION (*g_StdStream) << "Unsupported Windows version"; return NExitCode::kFatalError;

/* �ǉ��������� */
UINT _stdcall Main(LPVOID lpParameter)
{
	// �R�}���h���C���𕪊����ă��C�������֓n��
	BOOL bCodePage = ::AreFileApisANSI();
	int result = g_StdOut.SetLastError(SendCommand7zip());
	g_StdOut.CloseProgressDialog();
	if (bCodePage)
		SetFileApisToANSI();

	return result;
}
/* �ǉ������܂� */

/* �폜��������
int MY_CDECL main
(
  #ifndef _WIN32
  int numArgs, const char *args[]
  #endif
)
   �폜�����܂� */
int SendCommand7zip()	// �ǉ�
{
  g_StdStream = &g_StdOut;

  NT_CHECK

  NConsoleClose::CCtrlHandlerSetter ctrlHandlerSetter;
  int res = 0;
  try
  {
    res = Main2(
    #ifndef _WIN32
    numArgs, args
    #endif
    );
  }
  catch(const CNewException &)
  {
    (*g_StdStream) << kMemoryExceptionMessage;
    return (NExitCode::kMemoryError);
  }
  catch(const NConsoleClose::CCtrlBreakException &)
  {
    (*g_StdStream) << endl << kUserBreak;
    return (NExitCode::kUserBreak);
  }
  catch(const CArchiveCommandLineException &e)
  {
    (*g_StdStream) << kExceptionErrorMessage << e << endl;
    return (NExitCode::kUserError);
  }
  catch(const CSystemException &systemError)
  {
    if (systemError.ErrorCode == E_OUTOFMEMORY)
    {
      (*g_StdStream) << kMemoryExceptionMessage;
      return (NExitCode::kMemoryError);
    }
    if (systemError.ErrorCode == E_ABORT)
    {
      (*g_StdStream) << endl << kUserBreak;
      return (NExitCode::kUserBreak);
    }
	/* �ǉ��������� */
    if (systemError.ErrorCode == ERROR_PASSWORD_FILE)
    {
      (*g_StdStream) << endl << "Password Error\n";
      return (ERROR_PASSWORD_FILE);
    }
	/* �ǉ������܂� */
    (*g_StdStream) << endl << endl << "System error:" << endl <<
        NError::MyFormatMessageW(systemError.ErrorCode) << endl;
    return (NExitCode::kFatalError);
  }
  catch(NExitCode::EEnum &exitCode)
  {
    (*g_StdStream) << kInternalExceptionMessage << exitCode << endl;
    return (exitCode);
  }
  /*
  catch(const NExitCode::CMultipleErrors &multipleErrors)
  {
    (*g_StdStream) << endl << multipleErrors.NumErrors << " errors" << endl;
    return (NExitCode::kFatalError);
  }
  */
  catch(const UString &s)
  {
    (*g_StdStream) << kExceptionErrorMessage << s << endl;
    return (NExitCode::kFatalError);
  }
  catch(const AString &s)
  {
    (*g_StdStream) << kExceptionErrorMessage << s << endl;
    return (NExitCode::kFatalError);
  }
  catch(const char *s)
  {
    (*g_StdStream) << kExceptionErrorMessage << s << endl;
    return (NExitCode::kFatalError);
  }
  catch(int t)
  {
    (*g_StdStream) << kInternalExceptionMessage << t << endl;
    return (NExitCode::kFatalError);
  }
  catch(...)
  {
    (*g_StdStream) << kUnknownExceptionMessage;
    return (NExitCode::kFatalError);
  }
  return  res;
}
