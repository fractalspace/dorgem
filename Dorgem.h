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
// Dorgem.h : main header file for the Dorgem application

#ifndef __DORGEM_H__
#define __DORGEM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
  #error include 'StdAfx.h' before including this file for PCH
#endif

#include "Resource.h"           // Resource symbols
#include "RegistryEntry.h"      // Registry-file entries

#define DEFAULT_WEB_PORT 8080   // Not the offical default http port, but the official alternate port number

#define WEB_SERVER_NONE     0
#define WEB_SERVER_STILL    1
#define WEB_SERVER_STREAM   2

class CDunManager;
class CStoreManager;
class CCaptionManager;
class CListenSocket;
class CTM;

class CDorgemApp : public CWinApp
{
public:
  CDorgemApp();
  ~CDorgemApp();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDorgemApp)
  public:
  virtual BOOL InitInstance();
  //}}AFX_VIRTUAL

// Implementation

  //{{AFX_MSG(CDorgemApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

public:

  CString GetTempFileName(LPCSTR =NULL);
  CString GetSystemError(DWORD);
  HCURSOR LoadHandCursor();
  BOOL SetConfig(LPCSTR);

  UINT GetProfileInt(LPCTSTR, LPCTSTR, int =0);
  CString GetProfileString(LPCTSTR, LPCTSTR, LPCTSTR =NULL);
  BOOL GetProfileBinary(LPCTSTR, LPCTSTR, LPBYTE*, UINT*);
  BOOL WriteProfileInt(LPCTSTR, LPCTSTR, int);
  BOOL WriteProfileString(LPCTSTR, LPCTSTR, LPCTSTR);
  BOOL WriteProfileBinary(LPCTSTR, LPCTSTR, LPBYTE, UINT);

  void Crypt(CString&);
  void DeCrypt(CString&);

  void StartWebServer(BOOL =TRUE);
  void StopWebServer();
  BOOL IsWebServerRunning()       { return(m_pWebServer != NULL); };

  CStoreManager*    m_pStoreMngr;
  CDunManager*      m_pDunMngr;
  CCaptionManager*  m_pCaptionMngr;
  CListenSocket*    m_pWebServer;

  CTM*    m_ptmBuildDate;       // BuildDate
  CString m_strRegKey;          // Start of the registry path
  CString m_strConfig;          // The current configuration
  WORD    m_wLang;              // Language of the program
  CString m_strAppPath;         // Path to the .exe
  BOOL    m_bWin2k;             // Are we running Win2000 or newer
  BOOL    m_bStartMinimized;    // Start minimized
};

/////////////////////////////////////////////////////////////////////////////
// Global typesafe acces

inline CDorgemApp*      GetApp()         { return((CDorgemApp*)AfxGetApp()); }
inline CStoreManager*   GetStoreMngr()   { return(GetApp()->m_pStoreMngr);   }
inline CDunManager*     GetDunMngr()     { return(GetApp()->m_pDunMngr);     }
inline CCaptionManager* GetCaptionMngr() { return(GetApp()->m_pCaptionMngr); }

/////////////////////////////////////////////////////////////////////////////
// Own extra data validation

void AFXAPI DDV_MinChars(CDataExchange*, CString const&, int);

#endif // __DORGEM_H__
