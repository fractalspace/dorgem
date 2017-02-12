/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2001 Frank Fesevur                                       //
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
// StoreFile.cpp : implementation file

#include "StdAfx.h"
#include "StoreEvent.h"
#include "StoreFile.h"
#include "Dorgem.h"
#include "DorgemDlg.h"

#include "Tm.h"
#include "RegKey.h"
#include "RegistryEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

CStoreFile::CStoreFile()
{
  m_iType = STORE_FILE;
  m_bCreateDirs = FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Check if 'szDir' is really a directory

BOOL CStoreFile::IsDirectory(LPCSTR szDir)
{
  // Does 'szDir' exist
  WIN32_FIND_DATA fdRec;
  HANDLE hFind = FindFirstFile(szDir, &fdRec);
  if (hFind == INVALID_HANDLE_VALUE)
    return(FALSE);
  FindClose(hFind);

  // 'szDir' exists, but is it a directory?
  if (!(fdRec.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    return(FALSE);

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreFile::Store()
{
  TRACE("[CStoreFile]Store(%s)\n", m_strName);

  if (!m_bEnable || m_bBusy)
    return(TRUE);
  m_bBusy = TRUE;

  CTM tmNow;
  CString strFile = tmNow.Format(m_strFile);

  GetMainWnd()->SetStatusBar("Storing to file: " + strFile);

  // Do we need to create the directory
  if (m_bCreateDirs)
  {
    int iIndex = strFile.Find('\\');
    while (iIndex >= 0)
    {
      CString strTmp = strFile.Left(iIndex);
      if (!IsDirectory(strTmp))
      {
        if (CreateDirectory(strTmp, NULL))
        {
          TRACE("[CStoreFile]Store: Created '%s'\n", strTmp);
        }
      }
      iIndex = strFile.Find('\\', iIndex + 1);
    }
  }

  BOOL bRet = CopyFile(m_strOrigFile, strFile, FALSE);
  if (!bRet)
  {
    m_strErrorMsg.Format("Failed to save '%s': %s", strFile,
                                   GetApp()->GetSystemError(GetLastError()));
    TRACE("Error: '%s'\n", m_strErrorMsg);
    Log(LOG_ERROR, m_strErrorMsg);
    GetMainWnd()->SetStatusBar("Error saving image " + strFile);
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
    strTmp += " to file ";
    strTmp += strFile;
    GetMainWnd()->SetStatusBar(strTmp);
  }

  m_bBusy = FALSE;
  return(bRet);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreFile::ReadRegistry(CRegKey* pKey)
{
  ASSERT(pKey != NULL);

  Log(LOG_DEBUG, "Reading from registry");

  CStoreEvent::ReadRegistry(pKey);
  m_strFile = pKey->GetStringValue(szFile);
  m_bCreateDirs = pKey->GetDWORDValue(szCreateDirs, FALSE);

  Log(LOG_DEBUG, "Ready reading from registry");
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreFile::WriteRegistry(CRegKey* pKey)
{
  Log(LOG_DEBUG, "Writing to registry");

  CStoreEvent::WriteRegistry(pKey);

  // Store the information
  CRegKey reg;
  reg.Create(*pKey, m_strName);
  reg.SetStringValue(szFile, m_strFile);
  reg.SetDWORDValue(szCreateDirs, m_bCreateDirs);
  reg.Close();

  Log(LOG_DEBUG, "Ready writing to registry");
  return(TRUE);
}
