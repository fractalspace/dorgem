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
// Request.h : interface of the CRequest class
//
// This class is based upon the HttpSrv sample of Microsoft.
//

#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "Tm.h"

class CRequest : public CObject
{
  DECLARE_DYNCREATE(CRequest)

public:
  // Construction....
  CRequest();

  // Attributes....
  CString             m_strHost;            // host's address
  CString             m_strMethod;          // GET, HEAD or POST
  CString             m_strURL;             //
  CString             m_strVersion;         // HTTP/1.0
  CMapStringToString  m_mapHeaders;         //
  CByteArray          m_baBody;             //
  int                 m_cbBody;             //
  DWORD               m_dwExecute;          // executable type
  CString             m_strArgs;            // string after '?'
  UINT                m_uStatus;            // The status of this request
  BOOL                m_bDone;              // Are we done with this request
  UINT                m_cbSent;             //
  CString             m_strRequest;         // The request as it was send to the server
  CTM                 m_tmRequest;          // time of request
  CString             m_strRefer;           //
  CString             m_strUserAgent;       // Browser
  DWORD               m_dwContentLength;    // Length of the content (aka the image)

  // Access routines....
  CString GetHeaderValue(CString strName);
  int     AddRef();
  int     Release();

  enum AppFlags
  {
    APP_NONE    = 0x0000,
    APP_EXECUTE = 0x0001,
    APP_CGI     = 0x0002,
    APP_ISAPI   = 0x0004
  };

protected:
  int     m_nRefs;
};

#endif // __REQUEST_H__
