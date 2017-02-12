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
// Request.cpp : implementation of the CRequest class
//
// This class is based upon the HttpSrv sample of Microsoft.
//

#include "StdAfx.h"
#include "Request.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRequest, CObject)

/////////////////////////////////////////////////////////////////////////////
//

CRequest::CRequest()
{
  m_nRefs = 1;
  m_bDone = FALSE;
  m_dwExecute = 0;
  m_cbBody = 0;
  m_uStatus = 0;
  m_cbSent = 0;
  m_dwContentLength = 0;
  m_tmRequest.SetLanguage(LANG_ENGLISH);
}

/////////////////////////////////////////////////////////////////////////////
//

CString CRequest::GetHeaderValue(CString strName)
{
  CString strValue;
  strName.MakeLower();
  m_mapHeaders.Lookup(strName, strValue);
  return(strValue);
}

/////////////////////////////////////////////////////////////////////////////
//

int CRequest::AddRef()
{
  return(++m_nRefs);
}

/////////////////////////////////////////////////////////////////////////////
//

int CRequest::Release()
{
  int nRefs = --m_nRefs;
  if (nRefs == 0)
    delete this;
  return(nRefs);
}
