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
// StoreManager.cpp: implementation of the CStoreManager class.

#include "StdAfx.h"
#include "Dorgem.h"
#include "StoreManager.h"

#include "StoreEvent.h"
#include "StoreFile.h"
#include "StoreFTP.h"
#include "StoreHTTP.h"
#include "StoreProgram.h"
#include "RegKey.h"
#include "RegistryEntry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction

CStoreManager::CStoreManager()
{
  m_strRegPath = GetApp()->m_strRegKey + szStoreEvents;
  ReadEvents();
}

/////////////////////////////////////////////////////////////////////////////
// Read the events from the registry

void CStoreManager::ReadEvents()
{
  TRACE("[CStoreManager]ReadEvents()\n");

  CRegKey reg, reg2;
  if (!reg.Open(HKEY_CURRENT_USER, m_strRegPath))
    return;

  CString strEvent;
  BOOL bCont = reg.FindFirstSubKey(strEvent);
  while (bCont)
  {
    TRACE("StoreEvent: '%s'\n", strEvent);

    // Find out what type it is
    CRegKey reg2;
    if (reg2.Open(reg, strEvent))
    {
      DWORD dwType = reg2.GetDWORDValue(szType, 0);
      CStoreEvent* pEvent = NULL;

      // Create the right class
      switch(dwType)
      {
        case STORE_FILE:    pEvent = new CStoreFile;    break;
        case STORE_FTP:     pEvent = new CStoreFTP;     break;
        //case STORE_EMAIL:   pEvent = new CStoreEMail;   break;
        case STORE_PROGRAM: pEvent = new CStoreProgram; break;
        case STORE_HTTP:    pEvent = new CStoreHTTP;    break;
      }

      // Get the information of that class and add it to the list
      if (pEvent != NULL)
      {
        pEvent->SetName(strEvent);
        pEvent->SetRegCaption(m_strRegPath + "\\" + strEvent);
        pEvent->ReadRegistry(&reg2);
        AddEvent(pEvent);
      }

      reg2.Close();
    }
    bCont = reg.FindNextSubKey(strEvent);
  }
  Order();
}
