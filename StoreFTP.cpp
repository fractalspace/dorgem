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
// StoreFTP.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "StoreEvent.h"
#include "StoreFTP.h"
#include "FtpClient.h"

#include "Tm.h"
#include "RegKey.h"
#include "RegistryEntry.h"
#include "DunManager.h"
#include "DorgemDlg.h"

#ifdef _DEBUG
//#define __NO_FTP__     // Useful for testing without an active Internet connection
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

CStoreFTP::CStoreFTP()
{
  m_iType = STORE_FTP;
  m_dwPort = INTERNET_DEFAULT_FTP_PORT;
  m_bUseRAS = FALSE;
  m_bKeepOpen = FALSE;
  m_pConnection = NULL;
  m_bPassiveFTP = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//

CStoreFTP::~CStoreFTP()
{
  CloseConnection(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Create a session object to initialize WinINet library. Default parameters
// mean the access method in the registry (that is, set by the "Internet"
// icon in the Control Panel) will be used.

BOOL CStoreFTP::OpenConnection()
{
  BOOL bRet = TRUE;
  if (m_pConnection == NULL)
  {
    GetMainWnd()->SetStatusBar("Opening FTP connection to " + m_strHost);

    m_pConnection = new CFtpClient;
    ASSERT(m_pConnection != NULL);
    bRet = m_pConnection->LogOnToServer(m_strHost, m_strUser, m_strPassword, m_dwPort);

    if (!bRet)
    {
      m_strErrorMsg = m_pConnection->m_strRetMsg;
      Log(LOG_ERROR, m_strErrorMsg);
      GetMainWnd()->SetStatusBar("Error connecting to " + m_strHost);
    }
  }
  return(bRet);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFTP::CloseConnection(BOOL bAlwaysClose)
{
  if (m_bKeepOpen && !bAlwaysClose)
    return;

  // If the connection is open, close it
  if (m_pConnection != NULL)
  {
    m_pConnection->LogOffServer();
    delete m_pConnection;
    m_pConnection = NULL;
  }
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreFTP::Store()
{
  TRACE("[CStoreFTP]Store(%s)\n", m_strName);

  if (!m_bEnable || m_bBusy)
    return(TRUE);
  m_bBusy = TRUE;

  ASSERT(!m_strHost.IsEmpty());
  ASSERT(!m_strUser.IsEmpty());
  ASSERT(!m_strPassword.IsEmpty());
  ASSERT(!m_strOrigFile.IsEmpty());
  ASSERT(!m_strFile.IsEmpty());

#ifdef __NO_FTP__
  BOOL bRet = TRUE;  // Pretend we stored OK
#else
  BOOL bRet = FALSE;

  // Need to check for active RAS connection?
  if (m_bUseRAS)
  {
    CDunManager* pDun = GetDunMngr();
    pDun->SetConnection(m_strConnection);
    pDun->SetUseRas(m_bUseRAS);
    if (!pDun->IsConnected())
    {
      Log(LOG_INFO, m_strConnection + " not connected");
      if (m_bResetTime)
      {
        CTM tmNow;
        m_tLastUpdate = tmNow;
      }
      m_bBusy = FALSE;
      return(TRUE);
    }
  }

  // Do we need to open the connection
  if (!OpenConnection())
    return(FALSE);

  if (m_pConnection != NULL)
  {
    GetMainWnd()->SetStatusBar("Putting image with FTP on " + m_strHost);

    CTM tmNow;
    CString strFile = tmNow.Format(m_strFile);

    // Contains changes made by Aaron Kondziela - aaronk@pscomputing.com
    CString strFileTemp = strFile + ".temp"; // Temporary file name
    CString strFileTemp2 = strFile + ".old"; // This is for the original file when swapping

    // Upload image to a temp file
    if (m_pConnection->TransferFile(strFileTemp, m_strOrigFile, FALSE, m_bPassiveFTP))
    {
      // Quickly swap in the new image where the old one was
      // Don't catch errors here because these files may not exist
      m_pConnection->RenameRemoteFile(strFile, strFileTemp2);  // Rename the existing file if any
      m_pConnection->RenameRemoteFile(strFileTemp, strFile);   // Rename the temporary file to the real name (may want to catch errors here)
      m_pConnection->DeleteRemoteFile(strFileTemp2);           // Delete the actual filename if it already exists

      bRet = TRUE;
    }
    else
    {
      m_strErrorMsg = m_pConnection->m_strRetMsg;
      Log(LOG_ERROR, m_strErrorMsg);
      GetMainWnd()->SetStatusBar("Error putting image with FTP on " + m_strHost);
    }
  }

  CloseConnection(FALSE);

#endif

  if (bRet || m_bResetTime)
  {
    CTM tmNow;
    m_tLastUpdate = tmNow;
  }

  if (bRet)
  {
    CString strTmp = "Stored succesfully";
    Log(LOG_INFO, strTmp);
    strTmp += " with FTP on ";
    strTmp += m_strHost;
    GetMainWnd()->SetStatusBar(strTmp);
  }

  m_bBusy = FALSE;
  return(bRet);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreFTP::ReadRegistry(CRegKey* pKey)
{
  ASSERT(pKey != NULL);

  CStoreEvent::ReadRegistry(pKey);

  Log(LOG_DEBUG, "Reading from registry");

  m_strFile = pKey->GetStringValue(szFile);
  m_strHost = pKey->GetStringValue(szHost);
  m_strUser = pKey->GetStringValue(szUser);
  m_strPassword = pKey->GetStringValue(szPassword);
  m_dwPort = pKey->GetDWORDValue(szPort, INTERNET_DEFAULT_FTP_PORT);
  m_bKeepOpen = pKey->GetDWORDValue(szKeepOpen);
  m_bUseRAS = pKey->GetDWORDValue(szUseRas);
  m_bPassiveFTP = pKey->GetDWORDValue(szPassiveFTP, FALSE);
  m_strConnection = pKey->GetStringValue(szRasConn);

  GetApp()->DeCrypt(m_strPassword);
  Log(LOG_DEBUG, "Ready reading from registry");
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreFTP::WriteRegistry(CRegKey* pKey)
{
  CStoreEvent::WriteRegistry(pKey);

  Log(LOG_DEBUG, "Writing to registry");

  // Store the information
  CRegKey reg;
  reg.Create(*pKey, m_strName);
  reg.SetStringValue(szHost, m_strHost);
  reg.SetStringValue(szUser, m_strUser);
  reg.SetDWORDValue(szPort, m_dwPort);
  reg.SetStringValue(szFile, m_strFile);
  reg.SetDWORDValue(szInterval, m_tInterval);
  reg.SetDWORDValue(szKeepOpen, m_bKeepOpen);
  reg.SetDWORDValue(szUseRas, m_bUseRAS);
  reg.SetDWORDValue(szPassiveFTP, m_bPassiveFTP);
  reg.SetStringValue(szRasConn, m_strConnection);

  CString strPassword = m_strPassword;
  GetApp()->Crypt(strPassword);
  reg.SetStringValue(szPassword, strPassword);

  reg.Close();

  Log(LOG_DEBUG, "Ready writing to registry");
  return(TRUE);
}
