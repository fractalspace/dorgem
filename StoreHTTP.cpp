/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2003 Frank Fesevur                                       //
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
// StoreHTTP.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "StoreEvent.h"
#include "StoreHTTP.h"

#include "Tm.h"
#include "RegKey.h"
#include "RegistryEntry.h"
#include "DunManager.h"
#include "DorgemDlg.h"

#ifdef _DEBUG
//#define __NO_HTTP__     // Useful for testing without an active Internet connection
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

CStoreHTTP::CStoreHTTP()
{
  m_iType = STORE_HTTP;
  m_dwPort = INTERNET_DEFAULT_HTTP_PORT;
  m_bUseRAS = FALSE;
  m_bSecure = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreHTTP::Store()
{
  TRACE("[CStoreHTTP]Store(%s)\n", m_strName);

  if (!m_bEnable || m_bBusy)
    return(TRUE);
  m_bBusy = TRUE;

  ASSERT(!m_strHost.IsEmpty());
  ASSERT(!m_strUser.IsEmpty());
  ASSERT(!m_strPassword.IsEmpty());
  ASSERT(!m_strOrigFile.IsEmpty());
  ASSERT(!m_strFile.IsEmpty());

#ifdef __NO_HTTP__
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

  // Create a session object to initialize WinINet library. Default parameters
  // mean the access method in the registry (that is, set by the "Internet"
  // icon in the Control Panel) will be used.

  CString strTmp;
  strTmp.LoadString(IDS_PRODUCT_NAME);

  CInternetSession sess(strTmp);
  CHttpConnection* pHTTP  = NULL;
  CHttpFile* pHttpFile    = NULL;

  DWORD   dwInternetFlag  = INTERNET_FLAG_EXISTING_CONNECT;   // flags for opening http request
  DWORD   dwFileSize      = 0;                                // file size
  LPVOID  lpBuffer        = NULL;                             // buffer for sending file

  try
  {
    if (m_bSecure)    // Enable security flags  - CJB
    {
      m_dwPort = INTERNET_DEFAULT_HTTPS_PORT;
      dwInternetFlag = INTERNET_FLAG_SECURE;
    }


    GetMainWnd()->SetStatusBar("Opening HTTP connection to " + m_strHost);
    pHTTP = sess.GetHttpConnection(m_strHost, (INTERNET_PORT) m_dwPort, m_strUser, m_strPassword);
    if (pHTTP != NULL)
    {
      CTM tmNow;
      CString strFile = tmNow.Format(m_strFile);
      GetMainWnd()->SetStatusBar("Putting image with HTTP on " + m_strHost);

      // Read capture into buffer
      CFile fileImg;
      if (fileImg.Open(m_strOrigFile,CFile::modeRead | CFile::shareDenyWrite))
      {
        dwFileSize = (DWORD) fileImg.GetLength();       // get file length
        lpBuffer = malloc(dwFileSize);                  // create buffer
        dwFileSize = fileImg.Read(lpBuffer,dwFileSize); // read into buffer (returns bytes read)
        fileImg.Close();                                // close file

        // Create HTTP request (PUT)
        pHttpFile = pHTTP->OpenRequest(CHttpConnection::HTTP_VERB_PUT,
                                        strFile, NULL, 1, NULL, NULL, dwInternetFlag);

        if (pHttpFile->SendRequest(NULL, 0, lpBuffer, dwFileSize))
          bRet = TRUE;
        else
        {
          m_strErrorMsg = GetApp()->GetSystemError(GetLastError());
          Log(LOG_ERROR, m_strErrorMsg);
        }
      }
      else
      {
        m_strErrorMsg = GetApp()->GetSystemError(GetLastError());
        Log(LOG_ERROR, m_strErrorMsg);
      }
    }
  }
  catch (CInternetException* pEx)
  {
    TCHAR szErr[1024];
    pEx->GetErrorMessage(szErr, 1024);
    TRACE("[CStoreHTTP]Store ERROR! %s\n", szErr);
    m_strErrorMsg = szErr;
    Log(LOG_ERROR, m_strErrorMsg);
    pEx->Delete();
  }

  // If the request is open, close it
  if (pHttpFile != NULL)
  {
    pHttpFile->Close();
    delete pHttpFile;
  }

  // If the buffer was created, destroy it
  if (lpBuffer != NULL)
    free(lpBuffer);

  // If the connection is open, close it
  if (pHTTP != NULL)
  {
    pHTTP->Close();
    delete pHTTP;
  }

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
    strTmp += " with HTTP on ";
    strTmp += m_strHost;
    GetMainWnd()->SetStatusBar(strTmp);
  }
  else
    GetMainWnd()->SetStatusBar("Error putting image with HTTP on " + m_strHost);

  m_bBusy = FALSE;
  return(bRet);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreHTTP::ReadRegistry(CRegKey* pKey)
{
  ASSERT(pKey != NULL);

  Log(LOG_DEBUG, "Reading from registry");

  CStoreEvent::ReadRegistry(pKey);

  m_strFile = pKey->GetStringValue(szFile);
  m_bSecure = pKey->GetDWORDValue(szSecure);
  m_strHost = pKey->GetStringValue(szHost);
  m_strUser = pKey->GetStringValue(szUser);
  m_strPassword = pKey->GetStringValue(szPassword);
  m_dwPort = pKey->GetDWORDValue(szPort, INTERNET_DEFAULT_HTTP_PORT);
  m_bUseRAS = pKey->GetDWORDValue(szUseRas);
  m_strConnection = pKey->GetStringValue(szRasConn);

  GetApp()->DeCrypt(m_strPassword);
  Log(LOG_DEBUG, "Ready reading from registry");
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreHTTP::WriteRegistry(CRegKey* pKey)
{
  Log(LOG_DEBUG, "Writing to registry");

  CStoreEvent::WriteRegistry(pKey);

  // Store the information
  CRegKey reg;
  reg.Create(*pKey, m_strName);
  reg.SetDWORDValue(szSecure, m_bSecure);
  reg.SetStringValue(szHost, m_strHost);
  reg.SetStringValue(szUser, m_strUser);
  reg.SetDWORDValue(szPort, m_dwPort);
  reg.SetStringValue(szFile, m_strFile);
  reg.SetDWORDValue(szInterval, m_tInterval);
  reg.SetDWORDValue(szUseRas, m_bUseRAS);
  reg.SetStringValue(szRasConn, m_strConnection);

  CString strPassword = m_strPassword;
  GetApp()->Crypt(strPassword);
  reg.SetStringValue(szPassword, strPassword);

  reg.Close();

  Log(LOG_DEBUG, "Ready writing to registry");
  return(TRUE);
}
