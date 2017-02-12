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
// CaptionEvent.h : header file

#ifndef __CAPTIONEVENT_H__
#define __CAPTIONEVENT_H__

#define CAPTION_NONE 0   // Be carefull changing these numbers!
#define CAPTION_BMP  1   // They are saved in the registry
#define CAPTION_TXT  2

#define POS_LEFT     0
#define POS_CENTER   1
#define POS_RIGHT    2

#define POS_TOP      0
#define POS_CENTER   1
#define POS_BOTTOM   2

#include "DorgemEvent.h"

class CCaptionManager;

/////////////////////////////////////////////////////////////////////////////
//

class CCaptionEvent : public CDorgemEvent
{
public:
  CCaptionEvent();

  // Read from and write to the registry
  virtual BOOL ReadRegistry(CRegKey*);
  virtual BOOL WriteRegistry(CRegKey*);

  // Pure virtual function
  virtual BOOL Overlay(HDC, int, int) = 0;

  BOOL    GetAbsolute()           { return(m_bAbsolute);   };
  int     GetPosHor()             { return(m_iPosHor);     };
  int     GetPosVer()             { return(m_iPosVer);     };

  void SetAbsolute(BOOL b)        { m_bAbsolute = b;       };
  void SetPosHor(int i)           { m_iPosHor = i;         };
  void SetPosVer(int i)           { m_iPosVer = i;         };

protected:

  BOOL     m_bAbsolute;           // Is this position absolute
  int      m_iPosHor;             // Horizontal position
  int      m_iPosVer;             // Vertical position

};

void AFXAPI DDV_CaptionEventUnique(CDataExchange* pDX, CString const& strValue, CCaptionManager* pMngr);

#endif // __CAPTIONEVENT_H__
