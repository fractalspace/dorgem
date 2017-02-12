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
// CaptionEvent.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "CaptionEvent.h"

#include "RegKey.h"
#include "RegistryEntry.h"
#include "CaptionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Initialize the members

CCaptionEvent::CCaptionEvent() : CDorgemEvent()
{
  m_iType = CAPTION_NONE;
  m_bAbsolute = FALSE;
  m_iPosHor = POS_LEFT;
  m_iPosVer = POS_TOP;
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionEvent::ReadRegistry(CRegKey* pKey)
{
  ASSERT(pKey != NULL);

  TRACE("[CCaptionEvent]ReadRegistry()\n");

  m_bEnable = pKey->GetDWORDValue(szEnable, TRUE);
  m_iOrder = pKey->GetDWORDValue(szOrder, 0);
  m_bAbsolute = pKey->GetDWORDValue(szAbsolute, FALSE);
  m_iPosHor = pKey->GetDWORDValue(szPosHor, POS_LEFT);
  m_iPosVer = pKey->GetDWORDValue(szPosVer, POS_TOP);

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionEvent::WriteRegistry(CRegKey* pKey)
{
  TRACE("[CCaptionEvent]WriteRegistry()\n");

  // Store the information
  CRegKey reg;
  reg.Create(*pKey, m_strName);
  reg.SetDWORDValue(szType, m_iType);
  reg.SetDWORDValue(szEnable, m_bEnable);
  reg.SetDWORDValue(szOrder, m_iOrder);
  reg.SetDWORDValue(szAbsolute, m_bAbsolute);
  reg.SetDWORDValue(szPosHor, m_iPosHor);
  reg.SetDWORDValue(szPosVer, m_iPosVer);
  reg.Close();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// A DDV check to see if the event is unique

void AFXAPI DDV_CaptionEventUnique(CDataExchange* pDX, CString const& strValue, CCaptionManager* pMngr)
{
  if (!pDX->m_bSaveAndValidate)
    return;

  if (pMngr->Exist(strValue))
  {
    MsgBoxEx("An event with this name already exists!");
    pDX->Fail();
  }
}
