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
// EventManager.cpp: implementation of the CEventManager class.

#include "StdAfx.h"
#include "Dorgem.h"
#include "EventManager.h"

#include "DorgemEvent.h"
#include "RegKey.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction

CEventManager::CEventManager()
{
  m_iCount = 0;
  m_bAutoSave = TRUE;
}

CEventManager::~CEventManager()
{
  if (m_bAutoSave)
    WriteEvents();

  RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// Get the first available event

CDorgemEvent* CEventManager::GetFirstEvent()
{
  m_iCount = 0;
  return(GetNextEvent());
}

/////////////////////////////////////////////////////////////////////////////
// Get the next available event. If there are no more events NULL

CDorgemEvent* CEventManager::GetNextEvent()
{
  if (m_iCount >= m_pEvent.GetSize())
    return(NULL);
  return(m_pEvent[m_iCount++]);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CEventManager::Exist(CString strEvent)
{
  CDorgemEvent* pLoop = GetFirstEvent();
  while (pLoop != NULL)
  {
    if (pLoop->GetName() == strEvent)
      return(TRUE);
    pLoop = GetNextEvent();
  }

  return(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// Add the event to the array

BOOL CEventManager::AddEvent(CDorgemEvent* pEvent)
{
  ASSERT(pEvent != NULL);

  // Check to see if an event with this name already exists?
  if (Exist(pEvent->GetName()))
    return(FALSE);

  // Add the event to the list
  m_pEvent.Add(pEvent);
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Clean up the array

void CEventManager::RemoveAll()
{
  TRACE("[CEventManager]RemoveAll()\n");

  for (int i = 0; i < m_pEvent.GetSize(); i++)
    delete m_pEvent[i];
  m_pEvent.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CEventManager::Remove(CString strName)
{
  for (int i = 0; i < m_pEvent.GetSize(); i++)
  {
    TRACE("Event = '%s'\n", m_pEvent[i]->GetName());
    if (m_pEvent[i]->GetName() == strName)
    {
      delete m_pEvent[i];
      m_pEvent.RemoveAt(i);
      return(TRUE);
    }
  }
  return(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// Sort array, using good old bubble-sort since speed is not really needed

void CEventManager::Order()
{
  CDorgemEvent* pEvent;
  int i, j;
  for (i = 0; i < m_pEvent.GetSize(); i++)
  {
    for (j = i; j < m_pEvent.GetSize(); j++)
    {
      if (m_pEvent[i]->GetOrder() > m_pEvent[j]->GetOrder())
      {
        pEvent = m_pEvent[i];
        m_pEvent.SetAt(i, m_pEvent[j]);
        m_pEvent.SetAt(j, pEvent);
      }
    }
  }

  // Renumber the order numbers
  for (i = 0; i < m_pEvent.GetSize(); i++)
    m_pEvent[i]->SetOrder(i + 1);
}

/////////////////////////////////////////////////////////////////////////////
// Write the events to the registry

void CEventManager::WriteEvents()
{
  TRACE("[CEventManager]WriteEvents()\n");
  ASSERT(!m_strRegPath.IsEmpty());

  CRegKey reg;
  if (reg.Open(HKEY_CURRENT_USER, m_strRegPath))
    reg.DeleteTree();
  else
    reg.Create(HKEY_CURRENT_USER, m_strRegPath);

  Order();
  for (int i = 0; i < m_pEvent.GetSize(); i++)
    m_pEvent[i]->WriteRegistry(&reg);

  reg.Close();
}
