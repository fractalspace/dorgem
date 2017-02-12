/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2003 Frank Fesevur                                       //
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
// StoreHTTP.h : header file

#ifndef __STOREHTTP_H__
#define __STOREHTTP_H__

/////////////////////////////////////////////////////////////////////////////
//

class CStoreHTTP : public CStoreEvent
{
public:
  CStoreHTTP();

  BOOL Store();
  BOOL ReadRegistry(CRegKey*);
  BOOL WriteRegistry(CRegKey*);

  LPCSTR GetFile()            { return(m_strFile);       };
  LPCSTR GetHost()            { return(m_strHost);       };
  LPCSTR GetUser()            { return(m_strUser);       };
  LPCSTR GetPassword()        { return(m_strPassword);   };
  LPCSTR GetConnection()      { return(m_strConnection); };
  BOOL   UseRAS()             { return(m_bUseRAS);       };
  BOOL   Secure()             { return(m_bSecure);       };

  void SetFile(LPCSTR sz)       { m_strFile = sz;       };
  void SetHost(LPCSTR sz)       { m_strHost = sz;       };
  void SetUser(LPCSTR sz)       { m_strUser = sz;       };
  void SetPassword(LPCSTR sz)   { m_strPassword = sz;   };
  void SetConnection(LPCSTR sz) { m_strConnection = sz; };
  void UseRAS(BOOL b)           { m_bUseRAS = b;        };
  void Secure(BOOL b)           { m_bSecure = b;        };

protected:
  BOOL    m_bSecure;            // Use secure HTTP, aka HTTPS
  CString m_strHost;
  CString m_strUser;
  CString m_strPassword;
  DWORD   m_dwPort;
  CString m_strFile;            //
  BOOL    m_bUseRAS;            // Check RAS for active connections
  CString m_strConnection;

};

#endif // __STOREHTTP_H__
