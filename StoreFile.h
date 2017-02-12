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
// StoreFile.h : header file

#ifndef __STOREFILE_H__
#define __STOREFILE_H__

/////////////////////////////////////////////////////////////////////////////
//

class CStoreFile : public CStoreEvent
{
public:
  CStoreFile();

  BOOL Store();
  BOOL ReadRegistry(CRegKey*);
  BOOL WriteRegistry(CRegKey*);

  LPCSTR GetFile()          { return(m_strFile);      };
  BOOL   CreateDirs()       { return(m_bCreateDirs);  };

  void SetFile(LPCSTR sz)   { m_strFile = sz;         };
  void CreateDirs(BOOL b)   { m_bCreateDirs = b;      };

protected:
  CString m_strFile;
  BOOL    m_bCreateDirs;

  BOOL IsDirectory(LPCSTR);
};

#endif // __STOREFILE_H__
