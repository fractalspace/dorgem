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
// DorgemEvent.h : header file

#ifndef __DORGEMEVENT_H__
#define __DORGEMEVENT_H__

class CRegKey;

/////////////////////////////////////////////////////////////////////////////
// The CaptionEvent and StoreEvent are derived from this class.
// The EventManager is based upon this class.

class CDorgemEvent : public CObject
{
public:
  CDorgemEvent();

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Read from and write to the registry
  virtual BOOL ReadRegistry(CRegKey*) = 0;
  virtual BOOL WriteRegistry(CRegKey*) = 0;

  LPCSTR  GetName()               { return(m_strName);     };
  BOOL    IsEnabled()             { return(m_bEnable);     };
  int     GetOrder()              { return(m_iOrder);      };
  int     GetType()               { return(m_iType);       };
  CString GetLBName();

  void SetName(LPCSTR sz)         { m_strName = sz;        };
  void SetEnabled(BOOL b)         { m_bEnable = b;         };
  void SetOrder(int i)            { m_iOrder = i;          };

protected:

  int      m_iType;               // Type is the event
  CString  m_strName;             // The name of the event
  BOOL     m_bEnable;             // Is this event enabled
  int      m_iOrder;              // Order of the event

};

#endif // __DORGEMEVENT_H__
