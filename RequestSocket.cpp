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
// RequestSocket.cpp : implementation of the CRequestSocket class
//
// This class is based upon the HttpSrv sample of Microsoft.
//

#include "StdAfx.h"
#include "Dorgem.h"
#include "RequestSocket.h"
#include "Request.h"

#include "DorgemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define READ_SIZE   2048
#define CRLF        "\x0d\x0a"

IMPLEMENT_DYNCREATE(CRequestSocket, CAsyncSocket)

/////////////////////////////////////////////////////////////////////////////
//

CRequestSocket::CRequestSocket()
{
  m_bKilled = FALSE;
  m_iRefs = 1;
  m_reqStatus = REQ_REQUEST;
  m_buf.SetSize(1024);
  m_cbOut = 0;
  m_pRequest = NULL;
  m_dwTotalReceived = 0;
  m_uStillRefreshRate = GetApp()->GetProfileInt(szGeneralWebServer, szRefreshRate, 0);
  m_pCaptions = NULL;

  // If the path doesn't contain a '\' there is no full path given.
  // So add the app-path to the log file.
  CString str = GetApp()->GetProfileString(szGeneralWebServer, szAccessLog);
  if (str.Find("\\") == -1)
    m_strLogFile = GetApp()->m_strAppPath + str;
  else
    m_strLogFile = str;
}

/////////////////////////////////////////////////////////////////////////////
//

CRequestSocket::~CRequestSocket()
{
  if (m_pRequest)
  {
    // Release our hold on the request object....
    m_pRequest->m_bDone = TRUE;
    m_pRequest->Release();
  }
}

/////////////////////////////////////////////////////////////////////////////
//

void CRequestSocket::SetCaptionMngr(CCaptionManager* pMngr)
{
  m_pCaptions = pMngr;
}

/////////////////////////////////////////////////////////////////////////////
//

void CRequestSocket::OnReceive(int iErrorCode)
{
  TRACE("[CRequestSocket]OnReceive()\n");

  UNREFERENCED_PARAMETER(iErrorCode);

  if (m_pRequest == NULL)
  {
    // New request....
    m_pRequest = new CRequest;
    m_bKeepOpen = m_bWantKeepOpen = FALSE;
  }

  if (m_pRequest)
  {
    // Get the bytes....
    int iBytes = Receive(m_buf.GetData(), m_buf.GetSize());
    if (iBytes != SOCKET_ERROR)
    {
      // If the client has sent more than 10KB assume this is a DoS attack and close connection
      m_dwTotalReceived += iBytes;
      if (m_dwTotalReceived > 10240)
      {
        ShutDown(both);
        m_bKilled = TRUE;
        Release();
        return;
      }

      int iIndex = 0;
      switch (m_reqStatus)
      {
        case REQ_REQUEST:
        case REQ_HEADER:
          while (GetLine(m_buf, iBytes, iIndex) == TRUE)
          {
            if (!m_strLine.IsEmpty())
              ProcessLine();
            else
            {
              m_reqStatus = REQ_BODY;
              break;
            }
          }
          // Break if we're not looking for the body....
          if (m_reqStatus != REQ_BODY)
            break;
          // Stop if no body sent....
          if (!BodySent())
          {
            m_reqStatus = REQ_DONE;
            break;
          }
          // fall through....
        case REQ_BODY:
          AddToBody(iBytes, iIndex);
          break;
      }
      if (m_reqStatus == REQ_DONE)
      {
        m_buf.SetSize(0);
        if (!StartResponse())
          AsyncSelect(FD_WRITE | FD_CLOSE);
      }
    }
    else
      iBytes = GetLastError();
  }
  else
  {
    // Couldn't allocate request object....
    ShutDown(both);
    m_bKilled = TRUE;
    Release();
  }
}

/////////////////////////////////////////////////////////////////////////////
//

void CRequestSocket::OnSend(int iErrorCode)
{
  TRACE("[CRequestSocket]OnSend()\n");

  UNREFERENCED_PARAMETER(iErrorCode);

  int iBytes = Send(m_buf.GetData(), m_cbOut);
  if (iBytes == SOCKET_ERROR)
  {
    if (GetLastError() != WSAEWOULDBLOCK)
    {
      ShutDown(both);
      m_bKilled = TRUE;
      Release();
    }
    else
      AsyncSelect(FD_WRITE | FD_CLOSE);
  }
  else
  {
    if (iBytes < m_cbOut)
    {
      // Still got some left....
      m_buf.RemoveAt(0, iBytes);
      m_cbOut -= iBytes;

      // Adjust the bytes-sent value for the request....
      m_pRequest->m_cbSent += iBytes;

      // Set up for next write....
      AsyncSelect(FD_WRITE | FD_CLOSE);
    }
    else
    {
      // Adjust the bytes-sent value for the request....
      m_pRequest->m_cbSent += iBytes;

      // Eh, we're outta here....
      ShutDown(both);
      m_bKilled = TRUE;
      Release();
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//

void CRequestSocket::OnClose(int iErrorCode)
{
  TRACE("[CRequestSocket]OnClose()\n");

  UNREFERENCED_PARAMETER(iErrorCode);

  m_bKilled = TRUE;
  Release();
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CRequestSocket::GetLine(const CByteArray& bytes, int iBytes, int& iIndex)
{
  TRACE("[CRequestSocket]GetLine()\n");

  BOOL bLine = FALSE;
  while (bLine == FALSE && iIndex < iBytes)
  {
    char ch = (char)(bytes.GetAt(iIndex));
    switch(ch)
    {
      case '\r': // ignore
        break;
      case '\n': // end-of-line
        bLine = TRUE;
        break;
      default:   // other....
        m_strLine += ch;
        break;
    }
    iIndex++;
  }
  return(bLine);
}

/////////////////////////////////////////////////////////////////////////////
//

void CRequestSocket::ProcessLine()
{
  TRACE("[CRequestSocket]ProcessLine()\n");

  int iIndex;
  switch (m_reqStatus)
  {
    case REQ_REQUEST:
      m_pRequest->m_strRequest = m_strLine;
      iIndex = m_strLine.Find(' ');
      if (iIndex != -1)
      {
        m_pRequest->m_strMethod = m_strLine.Left(iIndex);
        m_strLine = m_strLine.Mid(iIndex+1);
        m_strLine.TrimLeft();
        iIndex = m_strLine.Find(' ');
        if (iIndex == -1)
        {
          m_pRequest->m_strURL = m_strLine;
          m_pRequest->m_strURL.TrimRight();
          m_reqStatus = REQ_SIMPLE;
        }
        else
        {
          m_pRequest->m_strURL = m_strLine.Left(iIndex);
          m_pRequest->m_strVersion = m_strLine.Mid(iIndex+1);
          m_pRequest->m_strVersion.TrimLeft();
        }
        // Check for execution arguments....
        iIndex = m_pRequest->m_strURL.Find('?');
        if (iIndex != -1)
        {
          // Yup; save the args....
          m_pRequest->m_strArgs = m_pRequest->m_strURL.Mid(iIndex+1);

          // Strip from file name....
          m_pRequest->m_strURL = m_pRequest->m_strURL.Left(iIndex);
          m_pRequest->m_dwExecute = CRequest::APP_EXECUTE;
        }

        // Change any "%xx"s to the appropriate char....
        m_pRequest->m_strURL = Decode(m_pRequest->m_strURL);
      }
      m_reqStatus = REQ_HEADER;
      break;

    case REQ_HEADER:
      iIndex = m_strLine.Find(':');
      if (iIndex != -1)
      {
        CString strName = m_strLine.Left(iIndex);
        CString strValue = m_strLine.Mid(iIndex+1);
        strName.MakeLower();
        strValue.TrimLeft();
        m_pRequest->m_mapHeaders.SetAt(strName, strValue);
      }
      break;
  }
  m_strLine.Empty();
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CRequestSocket::BodySent()
{
  TRACE("[CRequestSocket]BodySent()\n");

  BOOL bSent = FALSE;
  CString strValue = m_pRequest->GetHeaderValue("Content-Length");
  if (!strValue.IsEmpty())
  {
    m_pRequest->m_cbBody = atoi(strValue);
    bSent = TRUE;
  }
  return bSent;
}

/////////////////////////////////////////////////////////////////////////////
//

void CRequestSocket::AddToBody(int iBytes, int iIndex)
{
  TRACE("CRequestSocket]AddToBody()\n");

  // Save the buffer size....
  int iOldSize = m_buf.GetSize();

  // Get rid of old stuff; append to body data....
  m_buf.RemoveAt(0, iIndex);
  m_buf.SetSize(iBytes - iIndex);
  m_pRequest->m_baBody.Append(m_buf);

  // Restore the buffer size....
  m_buf.SetSize(iOldSize);

  // See if we're done....
  if (m_pRequest->m_baBody.GetSize() >= m_pRequest->m_cbBody)
  {
    m_pRequest->m_baBody.SetSize(m_pRequest->m_cbBody);
    m_reqStatus = REQ_DONE;
  }
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CRequestSocket::StartResponse()
{
  TRACE("[CRequestSocket]StartResponse()\n");

  // Switch on the method....
  if (m_pRequest->m_cbBody == 0 && m_pRequest->m_strMethod.CompareNoCase("GET") == 0)
  {
    if (m_pRequest->m_uStatus == 0)
    {
      // Get the information from the client
      UINT uPort;
      GetPeerName(m_pRequest->m_strHost, uPort);
      m_pRequest->m_strRefer = m_pRequest->GetHeaderValue("Referer");
      m_pRequest->m_strUserAgent = m_pRequest->GetHeaderValue("User-Agent");

      // Adjust the status bar
      GetMainWnd()->SetStatusBar("WebServer connection from " + m_pRequest->m_strHost);

      // Capture the current image (incl overlays) in a temporary JPEG file
      CString strJpegFile = GetApp()->GetTempFileName();
      if (!GetMainWnd()->DoCapture(strJpegFile, TRUE, m_pCaptions))
        return(FALSE);

      // Open that JPEG file
      CFile file;
      if (!file.Open(strJpegFile, CFile::modeRead))
        return(FALSE);
      m_pRequest->m_dwContentLength = (DWORD) file.GetLength();

      // Send the HTTP headers
      StuffStatus(200, "OK");
      StuffHeader("Content-Type", "image/jpeg");
      StuffHeader("Content-Length", m_pRequest->m_dwContentLength);
      StuffHeader("Last-Modified", GetHttpDate());
      StuffHeader("Cache-Control", "no-cache");         // HTTP 1.1
      StuffHeader("Cache-Control", "private");          // HTTP 1.1
      StuffHeader("Cache-Control", "must-revalidate");  // HTTP 1.1
      StuffHeader("Pragma", "no-cache");                // HTTP 1.0
      StuffHeader("Expires", 0);
      if (m_uStillRefreshRate != 0)
        StuffHeader("Refresh", m_uStillRefreshRate);
      StuffString(CRLF);

      // Read the content of the file
      BYTE bytes[READ_SIZE];
      DWORD dwRead = file.Read(bytes, READ_SIZE);
      while (dwRead > 0)
      {
        StuffRaw(bytes, dwRead);
        dwRead = file.Read(bytes, READ_SIZE);
      }
      StuffString(CRLF);

      // Clean up the mess
      file.Close();
      DeleteFile(strJpegFile);

      // Do Logging Stuff
      LogAccess(m_pRequest);
      GetMainWnd()->SetStatusBar();
    }
  }
  else
    StuffError(501, "Not Implemented");

  return(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//

int CRequestSocket::StuffRaw(LPBYTE bytes, int iLen)
{
  // Make sure there's enough room....
  if (m_cbOut + iLen > m_buf.GetSize())
  {
    int nChunks = iLen / 1024 + 1;
    m_buf.SetSize(m_cbOut + nChunks * 1024);
  }

  // Copy the data....
  MoveMemory(m_buf.GetData() + m_cbOut, bytes, iLen);
  m_cbOut += iLen;

  // Return amount of space left....
  return((m_buf.GetSize() - m_cbOut));
}

/////////////////////////////////////////////////////////////////////////////
//

int CRequestSocket::StuffString(LPCSTR szData)
{
  return(StuffRaw((LPBYTE) szData, strlen(szData)));
}

/////////////////////////////////////////////////////////////////////////////
//

int CRequestSocket::StuffStatus(UINT uStatus, LPCSTR szStatus)
{
  // Stuff the status
  CString strTmp;
  strTmp.Format("HTTP/1.0 %d %s%s", uStatus, szStatus, CRLF);
  StuffString(strTmp);
  m_pRequest->m_uStatus = uStatus;

  // Stuff the server name
  CString strServer;
  strServer.LoadString(IDS_PRODUCT_NAME);
  strServer += "/";
  strTmp.LoadString(IDS_PRODUCT_VERSION);
  strServer += strTmp;
  StuffHeader("Server", strServer);

  // Stuff the date
  return(StuffHeader("Date", GetHttpDate()));
}

/////////////////////////////////////////////////////////////////////////////
//

int CRequestSocket::StuffError(UINT uError, LPCSTR szError)
{
  StuffStatus(uError, szError);
  return(StuffString(CRLF));
}

/////////////////////////////////////////////////////////////////////////////
//

int CRequestSocket::StuffHeader(LPCSTR szName, LPCSTR szValue)
{
  StuffString(szName);
  StuffString(": ");
  StuffString(szValue);
  return(StuffString(CRLF));
}

/////////////////////////////////////////////////////////////////////////////
//

int CRequestSocket::StuffHeader(LPCSTR szName, int iValue)
{
  CString strValue;
  strValue.Format("%d", iValue);

  return(StuffHeader(szName, strValue));
}

/////////////////////////////////////////////////////////////////////////////
//

CString CRequestSocket::GetHttpDate()
{
  TRACE("[CRequestSocket]GetHttpDate()\n");

  CTM tmNow;
  tmNow.ToUTC();
  tmNow.SetLanguage(LANG_ENGLISH);

  return(tmNow.Format("%a, %d %o %Y %H:%0M:%0S GMT"));
}

/////////////////////////////////////////////////////////////////////////////
//

int CRequestSocket::AddRef()
{
  TRACE("[CRequestSocket]AddRef()\n");

  return(++m_iRefs);
}

/////////////////////////////////////////////////////////////////////////////
//

int CRequestSocket::Release()
{
  TRACE("[CRequestSocket]Release()\n");

  int iRefs = --m_iRefs;
  if (iRefs == 0)
    delete this;
  return(iRefs);
}

/////////////////////////////////////////////////////////////////////////////
//

#pragma warning(disable: 4244)  // conversion from 'xxx' to 'yyy', possible loss of data

CString Decode(const CString& str, BOOL bQuery)
{
  TRACE("Decode()\n");

  int iIndex;
  CString strDecoded = str;
  // special processing or query strings....
  if (bQuery)
  {
    // change all '+' to ' '....
    while ((iIndex = strDecoded.Find('+')) != -1)
      strDecoded = strDecoded.Left(iIndex) + ' ' + strDecoded.Mid(iIndex+1);
  }

  // first see if there are any %s to decode....
  if (strDecoded.Find('%') != -1)
  {
    // iterate through the string, changing %dd to special char....
    for (iIndex = 0; iIndex < strDecoded.GetLength(); iIndex++)
    {
      char ch = strDecoded.GetAt(iIndex);
      if (ch == '%')
      {
        if (strDecoded.GetAt(iIndex+1) == '%')
        {
          // wanna keep one percent sign....
          strDecoded = strDecoded.Left(iIndex) + strDecoded.Mid(iIndex+1);
        }
        else
        {
          // assume we have a hex value....
          char ch1 = strDecoded.GetAt(iIndex+1);
          char ch2 = strDecoded.GetAt(iIndex+2);
          ch1 = ch1 >= 'A' ? (ch1&0xdf)-'A' : ch1-'0';
          ch2 = ch2 >= 'A' ? (ch2&0xdf)-'A' : ch2-'0';
          // replace the escape sequence with the char....
          strDecoded = strDecoded.Left(iIndex)
            + (char)(ch1*16 + ch2)
            + strDecoded.Mid(iIndex+3);
        }
      }
    }
  }
  return(strDecoded);
}

#pragma warning(default: 4244)  // conversion from 'xxx' to 'yyy', possible loss of data

/////////////////////////////////////////////////////////////////////////////
//

void CRequestSocket::SendRawFrame()
{
  TRACE("[CRequestSocket]SendRawFrame()\n");

  if (m_pRequest == NULL)
  {
    // New request....
    m_pRequest = new CRequest;
    m_bKeepOpen = m_bWantKeepOpen = FALSE;
  }

  if (m_pRequest)
  {
    // Capture the image
    //m_pRequest->m_dwContentLength = m_pView->GetFrame(&m_frameData, &response);

    UINT uPeerPort;
    GetPeerName(m_pRequest->m_strHost, uPeerPort);
    m_pRequest->m_strRequest = "RAW WRITE";
    LogAccess(m_pRequest);

    //StartTargetStuff();
    AsyncSelect(FD_WRITE | FD_CLOSE);
  }
  else
  {
    // Couldn't allocate request object....
    ShutDown(both);
    m_bKilled = TRUE;
    Release();
  }
}

/////////////////////////////////////////////////////////////////////////////
//

void CRequestSocket::LogAccess(CRequest* pRequest)
{
  if (m_strLogFile.IsEmpty())
    return;

  CTM tmNow;
  FILE* fp = fopen(tmNow.Format(m_strLogFile), "a");
  if (fp == NULL)
    return;

  fprintf(fp, "%s - - [%s] \"%s\" %d %d \"%s\" \"%s\"\n",
      pRequest->m_strHost,
      pRequest->m_tmRequest.Format("%0d/%o/%Y:%0H:%0M:%0S %z"),
      pRequest->m_strRequest,
      pRequest->m_uStatus,
      pRequest->m_dwContentLength,
     (pRequest->m_strRefer.GetLength() ? pRequest->m_strRefer : "-"),
     (pRequest->m_strUserAgent.GetLength() ? pRequest->m_strUserAgent : "-"));
  fclose(fp);
}
