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
// DorgemEvent.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "DorgemEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Initialize the members

CDorgemEvent::CDorgemEvent()
{
  m_iType = 0;
  m_bEnable = FALSE;
  m_iOrder = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDorgemEvent diagnostics

#ifdef _DEBUG
void CDorgemEvent::AssertValid() const
{
  CObject::AssertValid();
  ASSERT(!m_strName.IsEmpty());
}

void CDorgemEvent::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
  dc << "Type = " << m_iType;
  dc << "Name = " << m_strName;
  dc << "Enabled = " << m_bEnable;
  dc << "Order = " << m_iOrder;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
//

CString CDorgemEvent::GetLBName()
{
  CString str = m_strName;
  if (m_bEnable)
    str += " *";
  return(str);
}
