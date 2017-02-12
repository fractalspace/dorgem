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
// StoreProgram.h : header file

#ifndef __STOREPROGRAM_H__
#define __STOREPROGRAM_H__

/////////////////////////////////////////////////////////////////////////////
//

class CStoreProgram : public CStoreEvent
{
public:
  CStoreProgram();

  BOOL Store();
  BOOL ReadRegistry(CRegKey*);
  BOOL WriteRegistry(CRegKey*);

  LPCSTR GetCommand()          { return(m_strCommand); };
  void SetCommand(LPCSTR sz)   { m_strCommand = sz;    };

protected:
  CString   m_strCommand;

  DWORD StartProgram(LPCSTR, BOOL);

};

#endif // __STOREPROGRAM_H__
