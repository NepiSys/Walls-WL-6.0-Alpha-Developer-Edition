// OpenCallbackConsole.cpp

#include "StdAfx.h"

#include "../../UI/Console/OpenCallbackConsole.h"	// �p�X�ύX

#include "../../UI/Console/ConsoleClose.h"			// �p�X�ύX
//#include "UserInputUtils.h"						// �폜

#include "resource.h"								// �ǉ�

HRESULT COpenCallbackConsole::Open_CheckBreak()
{
  if (NConsoleClose::TestBreakSignal())
    return E_ABORT;
  return S_OK;
}

HRESULT COpenCallbackConsole::Open_SetTotal(const UInt64 *, const UInt64 *)
{
  return Open_CheckBreak();
}

HRESULT COpenCallbackConsole::Open_SetCompleted(const UInt64 *, const UInt64 *)
{
  return Open_CheckBreak();
}
 
#ifndef _NO_CRYPTO

HRESULT COpenCallbackConsole::Open_CryptoGetTextPassword(BSTR *password)
{
  PasswordWasAsked = true;
  RINOK(Open_CheckBreak());
  if (!PasswordIsDefined)
  {
    RINOK(GetPassword(Password, IDS_DECRYPT));		// �ύX
    PasswordIsDefined = true;
  }
  return StringToBstr(Password, password);
}

HRESULT COpenCallbackConsole::Open_GetPasswordIfAny(UString &password)
{
  if (PasswordIsDefined)
    password = Password;
  return S_OK;
}

bool COpenCallbackConsole::Open_WasPasswordAsked()
{
  return PasswordWasAsked;
}

void COpenCallbackConsole::Open_ClearPasswordWasAskedFlag()
{
  PasswordWasAsked = false;
}

#endif
