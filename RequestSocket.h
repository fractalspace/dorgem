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
// RequestSocket.h : interface of the CRequestSocket class
//
// This class is based upon the HttpSrv sample of Microsoft.
//

#ifndef __REQUESTSOCKET_H__
#define __REQUESTSOCKET_H__

class CRequest;
class CCaptionManager;

class CRequestSocket : public CAsyncSocket
{
public:
  DECLARE_DYNCREATE(CRequestSocket)

  CRequestSocket();
  ~CRequestSocket();

  virtual int AddRef();
  virtual int Release();
  void SetCaptionMngr(CCaptionManager*);

protected:
  enum REQSTATUS
  {
    REQ_REQUEST=0, REQ_HEADER, REQ_BODY, REQ_SIMPLE, REQ_DONE
  };

  BOOL        m_bKeepOpen;            //
  BOOL        m_bWantKeepOpen;        //
  CRequest*   m_pRequest;             // Request object
  CByteArray  m_buf;                  //
  int         m_cbOut;                //
  CString     m_strLine;              //
  REQSTATUS   m_reqStatus;            //
  int         m_iRefs;                //
  BOOL        m_bKilled;              //
  UINT        m_uStillRefreshRate;    // The Still server refresh rate (0 = no refresh header)
  DWORD       m_dwTotalReceived;      // Total bytes received
  CString     m_strLogFile;           // The filename of the access log file
  CCaptionManager* m_pCaptions;       //

  CString GetHttpDate();
  BOOL GetLine(const CByteArray&, int, int&);
  void ProcessLine();
  BOOL BodySent();
  void AddToBody(int, int);
  BOOL StartResponse();
  void LogAccess(CRequest*);

  void SendRawFrame();
  int  StuffStatus(UINT, LPCSTR);
  int  StuffString(LPCSTR);
  int  StuffError(UINT, LPCSTR);
  int  StuffHeader(LPCSTR, LPCSTR);
  int  StuffHeader(LPCSTR, int);
  int  StuffRaw(LPBYTE, int);

  virtual void OnReceive(int);
  virtual void OnSend(int);
  virtual void OnClose(int);
};

CString Decode(const CString& str, BOOL bQuery = FALSE);

#endif // __REQUESTSOCKET_H__
