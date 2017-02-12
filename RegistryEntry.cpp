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
// Registry entries

#include "StdAfx.h"  // Just to prevent precompiled headers compilation error

#include "RegistryEntry.h"

// Path to store entries in registry
TCHAR szRegKey[]              = _T("Software\\Dorgem\\");
TCHAR szConfig[]              = _T("Config-");

// General settings
TCHAR szGeneral[]             = _T("General");
TCHAR szDevice[]              = _T("Device");
TCHAR szMainWndPos[]          = _T("MainWndPos");
TCHAR szPreviewPos[]          = _T("PreviewPos");
TCHAR szPreview[]             = _T("Preview");
TCHAR szTrayIcon[]            = _T("TrayIcon");
TCHAR szCompress[]            = _T("Compress");
TCHAR szCapture[]             = _T("Capture");
TCHAR szShowMsgBoxes[]        = _T("ShowMsgBoxes");

// Motion Detection settings
TCHAR szMotionDetection[]     = _T("MotionDetection");
TCHAR szReconnect[]           = _T("Reconnect");
TCHAR szReconnectTime[]       = _T("ReconnectTime");

// AVI settings
TCHAR szUseAVI[]              = _T("UseAVI");
TCHAR szArchiveAVI[]          = _T("ArchiveAVI");
TCHAR szAVIChangeInterval[]   = _T("AVIChangeInterval");
TCHAR szFramesPerSecond[]     = _T("FramesPerSecond");

// Web Server settings
TCHAR szGeneralWebServer[]    = _T("General\\WebServer");
TCHAR szWebServerType[]       = _T("WebServerType");
TCHAR szRefreshRate[]         = _T("RefreshRate");
TCHAR szAccessLog[]           = _T("AccessLog");

// Store settings
TCHAR szEnable[]              = _T("Enable");
TCHAR szInterval[]            = _T("Interval");
TCHAR szStoreEvents[]         = _T("StoreEvents");
TCHAR szType[]                = _T("Type");
TCHAR szLogFile[]             = _T("LogFile");
TCHAR szLogLevel[]            = _T("LogLevel");

TCHAR szCommand[]             = _T("Command");
TCHAR szFile[]                = _T("File");             // Also used by the text caption
TCHAR szHost[]                = _T("Host");
TCHAR szPassword[]            = _T("Password");
TCHAR szPort[]                = _T("Port");             // Also used by the web server
TCHAR szRasConn[]             = _T("RasConn");
TCHAR szResetTime[]           = _T("ResetTime");
TCHAR szUser[]                = _T("User");
TCHAR szUseRas[]              = _T("UseRas");
TCHAR szPassiveFTP[]          = _T("PassiveFTP");
TCHAR szKeepOpen[]            = _T("KeepOpen");
TCHAR szSecure[]              = _T("Secure");
TCHAR szCreateDirs[]          = _T("CreateDirs");

// Caption settings
TCHAR szCaptionEvents[]       = _T("CaptionEvents");
TCHAR szOrder[]               = _T("Order");
TCHAR szAbsolute[]            = _T("Absolute");
TCHAR szPosHor[]              = _T("PosHor");
TCHAR szPosVer[]              = _T("PosVer");

TCHAR szText[]                = _T("Text");
TCHAR szFont[]                = _T("Font");
TCHAR szLanguage[]            = _T("Language");
TCHAR szBackColor[]           = _T("BackColor");
TCHAR szForeColor[]           = _T("ForeColor");
TCHAR szTransparent[]         = _T("Transparent");
TCHAR szShadow[]              = _T("Shadow");
TCHAR szRotate[]              = _T("Rotate");
TCHAR szMaxLength[]           = _T("MaxLength");
TCHAR szOverlayType[]         = _T("OverlayType");
