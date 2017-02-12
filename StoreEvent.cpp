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
// StoreEvent.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "StoreEvent.h"

#include "StoreManager.h"
#include "CaptionManager.h"
#include "Tm.h"
#include "RegKey.h"
#include "RegistryEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Initialize the members

CStoreEvent::CStoreEvent() : CDorgemEvent()
{
  CTM tmNow;

  m_iType = STORE_NONE;
  m_bBusy = FALSE;
  m_tLastUpdate = tmNow;
  m_bResetTime = TRUE;
  m_tInterval = DEFAULT_INTERVAL;
  m_uLogLevel = 0;
  m_pCaptions = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//

CStoreEvent::~CStoreEvent()
{
  // Close the log-file
  if (m_fileLog.m_hFile != CFile::hFileNull)
    m_fileLog.Close();

  // Save the captions
  if (m_pCaptions != NULL)
  {
    m_pCaptions->WriteEvents();
    delete m_pCaptions;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Does this event need to be stored?

BOOL CStoreEvent::NeedStore()
{
  if (!m_bEnable)
    return(FALSE);

  CTM tmNow;
  return(m_tLastUpdate + m_tInterval <= (time_t) tmNow);
}

/////////////////////////////////////////////////////////////////////////////
// Reset the timer, just like a Store() has occurred

void CStoreEvent::ResetTimer()
{
  if (m_bResetTime)
  {
    CTM tmNow;
    m_tLastUpdate = tmNow;
  }
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreEvent::ReadRegistry(CRegKey* pKey)
{
  ASSERT(pKey != NULL);

  // Set the logfile?
  CString strFile = pKey->GetStringValue(szLogFile);
  if (!strFile.IsEmpty())
  {
    SetLogFile(strFile);
    m_uLogLevel = pKey->GetDWORDValue(szLogLevel, TRUE);
  }

  Log(LOG_DEBUG, "Base class: Reading from registry");

  m_bEnable = pKey->GetDWORDValue(szEnable, TRUE);
  m_iOrder = pKey->GetDWORDValue(szOrder, 0);
  m_tInterval = pKey->GetDWORDValue(szInterval, DEFAULT_INTERVAL);
  m_bResetTime = pKey->GetDWORDValue(szResetTime, TRUE);

  // Read the captions for this store event (if any)
  CRegKey reg;
  if (reg.Open(*pKey, szCaptionEvents))
  {
    reg.Close();
    m_pCaptions = new CCaptionManager(m_strRegCaption);
    ASSERT(m_pCaptions != NULL);
  }

  Log(LOG_DEBUG, "Base class: Ready reading from registry");
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreEvent::WriteRegistry(CRegKey* pKey)
{
  ASSERT(pKey != NULL);

  Log(LOG_DEBUG, "Base class: Writing to registry");

  // Store the information
  CRegKey reg;
  reg.Create(*pKey, m_strName);
  reg.SetDWORDValue(szType, m_iType);
  reg.SetDWORDValue(szEnable, m_bEnable);
  reg.SetDWORDValue(szOrder, m_iOrder);
  reg.SetDWORDValue(szInterval, m_tInterval);
  reg.SetDWORDValue(szResetTime, m_bResetTime);
  reg.SetDWORDValue(szLogLevel, m_uLogLevel);
  reg.SetStringValue(szLogFile, m_strLogFile);
  reg.Close();

  // Store the captions (if any)
  if (m_pCaptions != NULL)
    m_pCaptions->WriteEvents();

  Log(LOG_DEBUG, "Base class: Ready writing to registry");
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreEvent::Log(UINT uLevel, LPCSTR szLog)
{
  // Are we in the current log level?
  if (uLevel > m_uLogLevel)
    return;

  // Is the logfile opened?
  if (!OpenLogFile(TRUE))
    return;

  // Construct the time stamp
  CTM tmNow;
  CString str = tmNow.Format("%Y-%0m-%0d %0H:%0M:%0S ");

  // Add the type
  switch (m_iType)
  {
    case STORE_FILE:    str += "File Event";              break;
    case STORE_FTP:     str += "FTP Event";               break;
    case STORE_EMAIL:   str += "EMail Event";             break;
    case STORE_PROGRAM: str += "External Program Event";  break;
    case STORE_HTTP:    str += "HTTP Event";              break;
    default: ASSERT(FALSE);
  }

  // Add the name and text to log
  str += "(" + m_strName + "): " + szLog;

  // Finally do the real logging
  m_fileLog.WriteString(str + "\n");
  m_fileLog.Flush();
}

////////////////////////////////////////////////////////////////////////////
// Open or close the log file

void CStoreEvent::SetLogFile(CString str, BOOL bNew)
{
  if (str.IsEmpty())
  {
    m_fileLog.Close();
    m_strLogFile.Empty();
  }
  else
  {
    // If the path doesn't contain a '\' there is no full path given.
    // So add the app-path to the log file.
    if (str.Find("\\") == -1)
      m_strLogFile = GetApp()->m_strAppPath + str;
    else
      m_strLogFile = str;
  }
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreEvent::OpenLogFile(BOOL bNew)
{
  // Is the logfile name set
  if (m_strLogFile.IsEmpty())
    return(FALSE);

  // Is the file already opened?
  if (m_fileLog.m_hFile != CFile::hFileNull)
    return(TRUE);

  // Create the proper flag
  UINT uFlags = CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone;
  if (!bNew)
    uFlags |= CFile::modeNoTruncate;

  // Make the filename time related and open the file
  CTM tmNow;
  m_fileLog.Open(tmNow.Format(m_strLogFile), uFlags);

  if (!bNew)
    m_fileLog.SeekToEnd();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreEvent::GetInterval(time_t& tInterval, int& iBetween)
{
  // First try to find out if the interval is a whole hour or minute
  CTM tm;
  time_t tBetween;
  for (int i = BETWEEN_HOURS; i < BETWEEN_SECS; i++)
  {
    tBetween = tm.GetBetweenOffset(i);
    if (m_tInterval % tBetween == 0)
    {
      tInterval = m_tInterval / tBetween;
      iBetween = i;
      return;
    }
  }

  // return the seconds
  tInterval = m_tInterval;
  iBetween = BETWEEN_SECS;
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreEvent::SetInterval(int iInterval, int iBetween)
{
  CTM tm;
  m_tInterval = iInterval;
  m_tInterval *= tm.GetBetweenOffset(iBetween);
}

/////////////////////////////////////////////////////////////////////////////
//

CCaptionManager* CStoreEvent::GetCaptions()
{
  return(HasCaptions() ? m_pCaptions : GetCaptionMngr());
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreEvent::DeleteCaptions()
{
  if (!HasCaptions())
    return;

  m_pCaptions->SetAutoSave(FALSE);
  delete m_pCaptions;
  m_pCaptions = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// A DDV check to see if the event is unique

void AFXAPI DDV_StoreEventUnique(CDataExchange* pDX, CString const& strValue)
{
  if (!pDX->m_bSaveAndValidate)
    return;

  if (GetStoreMngr()->Exist(strValue))
  {
    MsgBoxEx("An event with this name already exists!");
    pDX->Fail();
  }
}
