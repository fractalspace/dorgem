/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2002-2005 Frank Fesevur                                  //
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
// ListenSocket.cpp : implementation of the CListenSocket class
//
// This class is based upon the HttpSrv sample of Microsoft.
//

#include "StdAfx.h"
#include "Dorgem.h"
#include "ListenSocket.h"

#include "RequestSocket.h"
#include "CaptionManager.h"
#include "RegKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CListenSocket, CAsyncSocket)

/////////////////////////////////////////////////////////////////////////////
//

CListenSocket::CListenSocket()
{
  m_pCaptions = NULL;

  // Read the captions for the web server
  CString strReg = GetApp()->m_strRegKey + szGeneralWebServer;
  CString strCap = strReg;
  strCap += "\\";
  strCap += szCaptionEvents;

  CRegKey reg;
  if (reg.Open(HKEY_CURRENT_USER, strCap))
  {
    reg.Close();
    m_pCaptions = new CCaptionManager(strReg);
    ASSERT(m_pCaptions != NULL);
    m_pCaptions->SetAutoSave(FALSE);
  }
}

/////////////////////////////////////////////////////////////////////////////
//

CListenSocket::~CListenSocket()
{
  if (m_pCaptions != NULL)
    delete m_pCaptions;
}

/////////////////////////////////////////////////////////////////////////////
//

void CListenSocket::OnAccept(int iErrorCode)
{
  UNREFERENCED_PARAMETER(iErrorCode);

  CCaptionManager* pMngr = m_pCaptions;
  if (pMngr == NULL)
    pMngr = GetCaptionMngr();

  CRequestSocket* pRequestSocket = new CRequestSocket;
  pRequestSocket->SetCaptionMngr(pMngr);
  if (Accept(*pRequestSocket))
    pRequestSocket->AsyncSelect(FD_READ | FD_CLOSE);
}
