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
// StoreFTP.h : header file

#ifndef __STOREFTP_H__
#define __STOREFTP_H__

class CFtpClient;

/////////////////////////////////////////////////////////////////////////////
//

class CStoreFTP : public CStoreEvent
{
public:
  CStoreFTP();
  ~CStoreFTP();

  BOOL Store();
  BOOL ReadRegistry(CRegKey*);
  BOOL WriteRegistry(CRegKey*);

  LPCSTR GetFile()              { return(m_strFile);        };
  LPCSTR GetHost()              { return(m_strHost);        };
  DWORD  GetPort()              { return(m_dwPort);         };
  LPCSTR GetUser()              { return(m_strUser);        };
  LPCSTR GetPassword()          { return(m_strPassword);    };
  LPCSTR GetConnection()        { return(m_strConnection);  };
  BOOL   KeepOpen()             { return(m_bKeepOpen);      };
  BOOL   UseRAS()               { return(m_bUseRAS);        };
  BOOL   UsePassiveFTP()        { return(m_bPassiveFTP);    };

  void SetFile(LPCSTR sz)       { m_strFile = sz;           };
  void SetHost(LPCSTR sz)       { m_strHost = sz;           };
  void SetPort(DWORD dw)        { m_dwPort = dw;            };
  void SetUser(LPCSTR sz)       { m_strUser = sz;           };
  void SetPassword(LPCSTR sz)   { m_strPassword = sz;       };
  void SetConnection(LPCSTR sz) { m_strConnection = sz;     };
  void UseRAS(BOOL b)           { m_bUseRAS = b;            };
  void UsePassiveFTP(BOOL b)    { m_bPassiveFTP = b;        };
  void KeepOpen(BOOL b)         { m_bKeepOpen = b;          };

protected:

  CFtpClient*       m_pConnection;    // The current FTP connection
  CString           m_strHost;        // FTP host to store the image
  CString           m_strUser;        // Username on the FTP host
  CString           m_strPassword;    // Password on the FTP host
  DWORD             m_dwPort;         // Port to talk to the FTP host
  CString           m_strFile;        // File on the FTP host
  BOOL              m_bKeepOpen;      // Try to keep the connection open between stores?
  BOOL              m_bUseRAS;        // Check RAS for active connections
  BOOL              m_bPassiveFTP;    // Use active or passive FTP?
  CString           m_strConnection;  // The RAS connection to check for

  BOOL OpenConnection();
  void CloseConnection(BOOL = FALSE);

};

#endif // __STOREFTP_H__
