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
// CaptionManager.cpp: implementation of the CCaptionManager class.

#include "StdAfx.h"
#include "Dorgem.h"
#include "CaptionManager.h"

#include "CaptionEvent.h"
#include "CaptionBitmap.h"
#include "CaptionText.h"
#include "RegKey.h"
#include "RegistryEntry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction

CCaptionManager::CCaptionManager(LPCSTR szRegPath)
{
  m_strRegPath = szRegPath;
  m_strRegPath += "\\";
  m_strRegPath += szCaptionEvents;
  ReadEvents();
}

CCaptionManager::CCaptionManager()
{
  m_strRegPath = GetApp()->m_strRegKey + szCaptionEvents;
  ReadEvents();
}

/////////////////////////////////////////////////////////////////////////////
// Read the events from the registry

void CCaptionManager::ReadEvents()
{
  TRACE("[CCaptionManager]ReadEvents()\n");

  CRegKey reg;
  if (!reg.Open(HKEY_CURRENT_USER, m_strRegPath))
    return;

  CString strName;
  BOOL bCont = reg.FindFirstSubKey(strName);
  while (bCont)
  {
    TRACE("CaptionEvent: '%s'\n", strName);

    // Find out what type it is
    CRegKey reg2;
    if (reg2.Open(reg, strName))
    {
      DWORD dwType = reg2.GetDWORDValue(szType, 0);
      CCaptionEvent* pEvent = NULL;

      // Create the right class
      switch(dwType)
      {
        case CAPTION_BMP:  pEvent = new CCaptionBitmap;  break;
        case CAPTION_TXT:  pEvent = new CCaptionText;    break;
      }

      // Get the information of that class and add it to the list
      if (pEvent != NULL)
      {
        pEvent->SetName(strName);
        pEvent->ReadRegistry(&reg2);
        AddEvent(pEvent);
      }

      reg2.Close();
    }
    bCont = reg.FindNextSubKey(strName);
  }
  Order();
}
