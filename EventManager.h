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
// EventManager.h: interface for the CEventManager class.

#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDorgemEvent;

#ifndef CDorgemEventPtrArray
#define CDorgemEventPtrArray CTypedPtrArray<CObArray,CDorgemEvent*>
#endif

class CEventManager : public CObject
{
public:
  CEventManager();
  virtual ~CEventManager();

  BOOL Exist(CString);

  BOOL AddEvent(CDorgemEvent*);
  void RemoveAll();
  BOOL Remove(CString);

  CDorgemEvent* GetFirstEvent();
  CDorgemEvent* GetNextEvent();

  void WriteEvents();
  void Order();

  BOOL GetAutoSave()              { return(m_bAutoSave); };
  void SetAutoSave(BOOL b)        { m_bAutoSave = b;     };

protected:

  CDorgemEventPtrArray m_pEvent;      // The events
  int     m_iCount;                   // Counter for GetFirst-GetNext
  CString m_strRegPath;               // Where are the settings stored in the registry
  BOOL    m_bAutoSave;                // Automaticly save on destruct?

  virtual void ReadEvents() = 0;
};

#endif // __EVENTMANAGER_H__
