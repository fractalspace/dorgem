/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2002 Frank Fesevur                                       //
//                                                                         //
//  This program is free software; you can redistribute it and/or modify   //
//  it under the terms of the GNU General Public License as published by   //
//  the Free Software Foundation; either version 2 of the License, or      //
//  (at your option) any later version.                                    //
//                                                                         //
//  This program is distributed in the hope that it will be useful,        //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           //
//  GNU General Public License for more details.                           //
//                                                                         //
//  You should have received a copy of the GNU General Public License      //
//  along with this program; if not, write to the Free Software            //
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
// StoreProgram.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "StoreEvent.h"
#include "StoreProgram.h"

#include "Tm.h"
#include "RegKey.h"
#include "RegistryEntry.h"
#include "DorgemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

CStoreProgram::CStoreProgram()
{
  m_iType = STORE_PROGRAM;
}

////////////////////////////////////////////////////////////////////////////
// Sort of generic StartProgram()

DWORD CStoreProgram::StartProgram(LPCSTR szCmdLine, BOOL bWaitFinish)
{
  TCHAR szCmd[_MAX_PATH];
  lstrcpy(szCmd, szCmdLine);

  TCHAR szPath[_MAX_PATH];
  GetCurrentDirectory(_MAX_PATH, szPath);

  STARTUPINFO si;
  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.dwFlags = STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;
  si.cb = sizeof(STARTUPINFO);

  PROCESS_INFORMATION pi;
  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

  DWORD dwReturn = NOERROR;
  if (CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, szPath, &si, &pi))
  {
    if (bWaitFinish)
      WaitForSingleObject(pi.hProcess, INFINITE);
  }
  else
    dwReturn = GetLastError();

  return(dwReturn);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreProgram::Store()
{
  TRACE("[CStoreProgram]Store(%s)\n", m_strName);

  if (!m_bEnable || m_bBusy)
    return(TRUE);
  m_bBusy = TRUE;

  BOOL bRet = TRUE;
  CTM tmNow;
  CString strTmp = m_strCommand;
  if (strTmp.Replace("%s", m_strOrigFile) != 1)
  {
    CString str = "No '%s' in command of Program Store Event '";
    str += m_strName;
    str += "'";
    Log(LOG_ERROR, str);
    GetMainWnd()->SetStatusBar(str);
    bRet = FALSE;
  }
  else
  {
    GetMainWnd()->SetStatusBar("Storing using program " + m_strName);
    bRet = (StartProgram(tmNow.Format(strTmp), TRUE) == 0);
  }

  if (bRet || m_bResetTime)
  {
    tmNow.GetSystemTime();
    m_tLastUpdate = tmNow;
  }

  if (bRet)
  {
    CString strTmp = "Stored succesfully";
    Log(LOG_INFO, strTmp);
    strTmp += " using program ";
    strTmp += m_strName;
    GetMainWnd()->SetStatusBar(strTmp);
  }

  m_bBusy = FALSE;
  return(bRet);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreProgram::ReadRegistry(CRegKey* pKey)
{
  ASSERT(pKey != NULL);

  Log(LOG_DEBUG, "Reading from registry");

  CStoreEvent::ReadRegistry(pKey);

  m_strCommand = pKey->GetStringValue(szCommand);

  Log(LOG_DEBUG, "Ready reading from registry");
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreProgram::WriteRegistry(CRegKey* pKey)
{
  Log(LOG_DEBUG, "Writing to registry");

  CStoreEvent::WriteRegistry(pKey);

  // Store the information
  CRegKey reg;
  reg.Create(*pKey, m_strName);
  reg.SetStringValue(szCommand, m_strCommand);
  reg.Close();

  Log(LOG_DEBUG, "Ready writing to registry");
  return(TRUE);
}
