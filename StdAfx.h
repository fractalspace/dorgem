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
// StdAfx.h: include file for standard system include files, or project
// specific include files that are used frequently, but are changed
// infrequently

#ifndef __STDAFX_H__
#define __STDAFX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN    // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>     // MFC core and standard components
#include <afxext.h>     // MFC extensions

#include <windowsx.h>   // Macro APIs, window message crackers, and control APIs
#include <ras.h>        // Remote Access API
#include <raserror.h>   // RAS Error handling
#include <vfw.h>        // Video for Windows
#include <mmreg.h>      // Multimedia Registration
#include <math.h>       // Mathematics support
#include <htmlhelp.h>   // HTML Help support

#include <afxmt.h>      // MFC Multithreaded Extensions
#include <afxtempl.h>   // MFC Template support
//#include <afxdtctl.h>   // MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>     // MFC support for Windows Common Controls
#include <afxinet.h>    // MFC Internet support
#include <afxsock.h>    // MFC support for Windows Sockets

// Useful macros for easy access to the different types of standard message boxes
#define MsgBoxEx(s)           AfxMessageBox(s, MB_OK | MB_ICONEXCLAMATION)
#define MsgBoxStop(s)         AfxMessageBox(s, MB_OK | MB_ICONSTOP)
#define MsgBoxError(s)        AfxMessageBox(s, MB_OK | MB_ICONERROR)
#define MsgBoxInfo(s)         AfxMessageBox(s, MB_OK | MB_ICONINFORMATION)
#define MsgBoxYesNo(s)       (AfxMessageBox(s, MB_YESNO | MB_ICONQUESTION) == IDYES)
#define MsgBoxYesNoCancel(s)  AfxMessageBox(s, MB_YESNOCANCEL | MB_ICONQUESTION)

#endif // __STDAFX_H__
