// ConsoleClose.cpp

#include "StdAfx.h"

#include "../../UI/Console/ConsoleClose.h"	// パス変更

static int g_BreakCounter = 0;
static const int kBreakAbortThreshold = 2;

// namespace NConsoleClose {							// 削除

#if !defined(UNDER_CE) && defined(_WIN32)
//static BOOL WINAPI HandlerRoutine(DWORD ctrlType)		// 削除
BOOL HandlerRoutine()									// 追加
{
  /* 削除ここから
  if (ctrlType == CTRL_LOGOFF_EVENT)
  {
    // printf("\nCTRL_LOGOFF_EVENT\n");
    return TRUE;
  }
     削除ここまで */ 

  g_BreakCounter++;
  if (g_BreakCounter < kBreakAbortThreshold)
    return TRUE;
  return FALSE;
  /*
  switch(ctrlType)
  {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
      if (g_BreakCounter < kBreakAbortThreshold)
      return TRUE;
  }
  return FALSE;
  */
}
#endif

namespace NConsoleClose {			// 追加
bool TestBreakSignal()
{
  #ifdef UNDER_CE
  return false;
  #else
  /*
  if (g_BreakCounter > 0)
    return true;
  */
  return (g_BreakCounter > 0);
  #endif
}

void CheckCtrlBreak()
{
  if (TestBreakSignal())
    throw CCtrlBreakException();
}

CCtrlHandlerSetter::CCtrlHandlerSetter()
{
  #if !defined(UNDER_CE) && defined(_WIN32)
//  if(!SetConsoleCtrlHandler(HandlerRoutine, TRUE))	// 削除
//    throw "SetConsoleCtrlHandler fails";				// 削除
  #endif
}

CCtrlHandlerSetter::~CCtrlHandlerSetter()
{
  #if !defined(UNDER_CE) && defined(_WIN32)
//  if(!SetConsoleCtrlHandler(HandlerRoutine, FALSE))	// 削除
//    throw "SetConsoleCtrlHandler fails";				// 削除
	g_BreakCounter = 0;									// 追加
  #endif
}

}
