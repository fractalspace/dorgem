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
// Dorgem.cpp : Defines the class behaviors for the application.

#include "StdAfx.h"
#include "Dorgem.h"
#include "DorgemDlg.h"

#include "DunManager.h"
#include "StoreManager.h"
#include "CaptionManager.h"
#include "Tm.h"
#include "RegKey.h"
#include "SInstance.h"
#include "CmdLine.h"
#include "ListenSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDorgemApp

BEGIN_MESSAGE_MAP(CDorgemApp, CWinApp)
  //{{AFX_MSG_MAP(CDorgemApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDorgemApp construction

CDorgemApp::CDorgemApp()
{
  m_wLang = LANG_ENGLISH;
  m_pWebServer = NULL;
  m_strRegKey = szRegKey;
  m_bStartMinimized = FALSE;
}

CDorgemApp::~CDorgemApp()
{
  // These ones are always created
  delete m_pDunMngr;
  delete m_pStoreMngr;
  delete m_ptmBuildDate;
  delete m_pCaptionMngr;

  StopWebServer();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDorgemApp object

CDorgemApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDorgemApp initialization

BOOL CDorgemApp::InitInstance()
{
  // Parse the command line
  CCmdLine CmdInfo;
  ParseCommandLine(CmdInfo);

  // Initialize the main dialog
  CDorgemDlg dlg;
  m_pMainWnd = &dlg;

  // Check to see if there is a previous instance?
  dlg.m_pInstanceChecker->ActivateChecker();
  if (dlg.m_pInstanceChecker->PreviousInstanceRunning())
  {
    //MsgBoxInfo("Dorgem is already running, it will now be restored.");
    dlg.m_pInstanceChecker->ActivatePreviousInstance();
    m_pMainWnd = NULL;
    return(FALSE);
  }

  // Get the *path* to the .exe
  TCHAR szPath[_MAX_PATH];
  DWORD dwSize = _MAX_PATH;
  if (GetModuleFileName(m_hInstance, szPath, dwSize) == 0)
  {
    TRACE("GetModuleFileName failed\n");
    return(FALSE);
  }
  m_strAppPath = szPath;
  int iIndex = m_strAppPath.ReverseFind('\\');
  ASSERT(iIndex != -1);
  m_strAppPath = m_strAppPath.Left(iIndex + 1);
  TRACE("AppPath: '%s'\n", m_strAppPath);

  // Construct the full path name for the .chm
  CString strTmp;
  strTmp.LoadString(IDS_PRODUCT_NAME);
  CString strHelpPath = m_strAppPath + strTmp;
  strHelpPath += ".chm";
  free((void*) m_pszHelpFilePath);
  m_pszHelpFilePath = _tcsdup(strHelpPath);

  // Check if we use Windows 2000 or newer?
  OSVERSIONINFO os;
  os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&os);
  m_bWin2k = (os.dwPlatformId == VER_PLATFORM_WIN32_NT && os.dwMajorVersion >= 5);

  // Create the event manager
  m_pStoreMngr = new CStoreManager;
  ASSERT(m_pStoreMngr != NULL);

  // Create the DUN manager
  m_pDunMngr = new CDunManager;
  ASSERT(m_pDunMngr != NULL);

  // Create the Caption manager
  m_pCaptionMngr = new CCaptionManager;
  ASSERT(m_pCaptionMngr != NULL);

  // Create the CTM object and decode the build date
  m_ptmBuildDate = new CTM;
  ASSERT(m_ptmBuildDate != NULL);

  char szMonth[10];
  int iDay, iMonth, iYear;
  sscanf(__DATE__, "%s %d %d", szMonth, &iDay, &iYear);
  for (iMonth = 0; iMonth < MAXMONTH; iMonth++)
  {
    strTmp = m_ptmBuildDate->MonthName(iMonth, LANG_ENGLISH, FALSE);
    if (strTmp == szMonth)
      break;
  }
  m_ptmBuildDate->SetDate(iDay, iMonth, iYear - 1900);
  m_ptmBuildDate->SetLanguage(m_wLang);
  TRACE("Built on %s\n", m_ptmBuildDate->Format("%d %O %Y"));

  // Initialise WinSock
  if (!AfxSocketInit())
  {
    MsgBoxStop("Failed to initialize WinSock");
    return(FALSE);
  }

  // Start the webserver
  if (GetProfileInt(szGeneralWebServer, szWebServerType, WEB_SERVER_NONE) != WEB_SERVER_NONE)
    StartWebServer();

  // Finally open the dialog
  dlg.DoModal();

  return(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// Set the configuration name

BOOL CDorgemApp::SetConfig(LPCSTR szCfg)
{
  // Check if all chars in the config name are valid
  LPCSTR p = szCfg;
  while (*p)
  {
    if (!isalnum(*p))
      return(FALSE);
    p++;
  }

  // Set the member properly
  m_strConfig = szCfg;

  m_strRegKey = szRegKey;
  m_strRegKey += szConfig;
  m_strRegKey += szCfg;
  m_strRegKey += '\\';
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Overload of CWinApp::GetProfileInt(). Don't want the 'Company'.

UINT CDorgemApp::GetProfileInt(LPCTSTR szSection, LPCTSTR szEntry, int nDefault)
{
  ASSERT(szSection != NULL);
  ASSERT(szEntry != NULL);

  CRegKey reg;
  if (!reg.Open(HKEY_CURRENT_USER, m_strRegKey + szSection))
    return(nDefault);
  return((UINT) reg.GetDWORDValue(szEntry, nDefault));
}

/////////////////////////////////////////////////////////////////////////////
// Overload of CWinApp::GetProfileString(). Don't want the 'Company'.

CString CDorgemApp::GetProfileString(LPCTSTR szSection,
                                      LPCTSTR szEntry, LPCTSTR szDefault)
{
  ASSERT(szSection != NULL);
  ASSERT(szEntry != NULL);

  CRegKey reg;
  if (!reg.Open(HKEY_CURRENT_USER, m_strRegKey + szSection))
    return(szDefault);
  return(reg.GetStringValue(szEntry, szDefault));
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CDorgemApp::GetProfileBinary(LPCTSTR szSection, LPCTSTR szEntry, LPBYTE* ppData, UINT* pBytes)
{
  ASSERT(szSection != NULL);
  ASSERT(szEntry != NULL);

  CRegKey reg;
  if (!reg.Open(HKEY_CURRENT_USER, m_strRegKey + szSection))
    return(FALSE);
  return(reg.GetBinaryValue(szEntry, ppData, pBytes));
}

/////////////////////////////////////////////////////////////////////////////
// Overload of CWinApp::WriteProfileInt(). Don't want the 'Company'.

BOOL CDorgemApp::WriteProfileInt(LPCTSTR szSection, LPCTSTR szEntry, int nValue)
{
  ASSERT(szSection != NULL);
  ASSERT(szEntry != NULL);

  CRegKey reg;
  if (!reg.Create(HKEY_CURRENT_USER, m_strRegKey + szSection))
    return(FALSE);
  return(reg.SetDWORDValue(szEntry, nValue));
}

/////////////////////////////////////////////////////////////////////////////
// Overload of CWinApp::WriteProfileString(). Don't want the 'Company'.

BOOL CDorgemApp::WriteProfileString(LPCTSTR szSection, LPCTSTR szEntry, LPCTSTR szValue)
{
  ASSERT(szSection != NULL);
  ASSERT(szEntry != NULL);

  CRegKey reg;
  if (!reg.Create(HKEY_CURRENT_USER, m_strRegKey + szSection))
    return(FALSE);
  return(reg.SetStringValue(szEntry, szValue));
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CDorgemApp::WriteProfileBinary(LPCTSTR szSection, LPCTSTR szEntry, LPBYTE pData, UINT nBytes)
{
  ASSERT(szSection != NULL);
  ASSERT(szEntry != NULL);

  CRegKey reg;
  if (!reg.Create(HKEY_CURRENT_USER, m_strRegKey + szSection))
    return(FALSE);
  return(reg.SetBinaryValue(szEntry, pData, nBytes));
}

//////////////////////////////////////////////////////////////////////
//

CString CDorgemApp::GetTempFileName(LPCSTR szPattern)
{
  if (szPattern == NULL)
  {
    if (m_strConfig.IsEmpty())
      szPattern = "cap";
    else
      szPattern = m_strConfig;
  }

  TCHAR szTmpDir[MAX_PATH];
  if (::GetTempPath(MAX_PATH, szTmpDir))
  {
    TCHAR szTmpFile[MAX_PATH];
    if (::GetTempFileName(szTmpDir, szPattern, 0, szTmpFile))
      return(szTmpFile);
  }

  CString str = szPattern;
  str += ".tmp";
  return(str);
}

//////////////////////////////////////////////////////////////////////
//

CString CDorgemApp::GetSystemError(DWORD dwError)
{
  char szBuf[128];
  if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError,
                          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          szBuf, sizeof(szBuf), NULL))
  {
    return(szBuf);
  }

  CString strTmp;
  strTmp.Format("Error %d", dwError);
  return(strTmp);
}

//////////////////////////////////////////////////////////////////////////
// Load the hand cursor. First try to load the Windows hand cursor. If
// this fails (win95/nt4), load the cursor from our own resources.

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)   // Taken from WINUSER.H
#endif // IDC_HAND

HCURSOR CDorgemApp::LoadHandCursor()
{
  static HCURSOR hCursor = NULL;

  // First try to load Windows Hand Cursor
  if (hCursor == NULL)
    hCursor = LoadStandardCursor(IDC_HAND);

  // No cursor handle - load our own
  if (hCursor == NULL)
    hCursor = LoadCursor(IDC_LINK_CURSOR);

  ASSERT(hCursor != NULL);
  return(hCursor);
}

////////////////////////////////////////////////////////////////////////////////
// A Crypt routine

void CDorgemApp::Crypt(CString& strMessage)
{
  CString strKey;
  strKey.LoadString(IDS_PRODUCT_NAME);

  int iLenK = strKey.GetLength(), iLenM = strMessage.GetLength(), i, j, d;

  for (i = 0, j = 0; i < iLenM; i++)
  {
    d = strMessage[i] + strKey[j] - 64;
    d -= (d / 93) * 93;
    strMessage.SetAt(i, char(d + 32));
    j++;
    j -= (j / iLenK) * iLenK;
  }
}

////////////////////////////////////////////////////////////////////////////////
// A DeCrypt routine

void CDorgemApp::DeCrypt(CString& strMessage)
{
  CString strKey;
  strKey.LoadString(IDS_PRODUCT_NAME);

  int iLenK = strKey.GetLength(), iLenM = strMessage.GetLength(), i, j, d;

  for (i = 0, j = 0; i < iLenM; i++)
  {
    d = strMessage[i] - strKey[j];
    if (d < 0)
      d += 93;
    strMessage.SetAt(i, char(d + 32));
    j++;
    j -= (j / iLenK) * iLenK;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Start the webserver, use bRestart if a running server should be restarted

void CDorgemApp::StartWebServer(BOOL bRestart)
{
  // Is the server already running?
  if (IsWebServerRunning())
  {
    if (!bRestart)
      return;
    StopWebServer();
  }

  // Start the web server
  UINT uPort = GetProfileInt(szGeneralWebServer, szPort, DEFAULT_WEB_PORT);
  m_pWebServer = new CListenSocket;
  ASSERT(m_pWebServer != NULL);
  if (!m_pWebServer->Create(uPort, SOCK_STREAM, FD_ACCEPT))
  {
    CString strTmp;
    strTmp.Format("Cannot start web server at port %d", uPort);
    MsgBoxStop(strTmp);
    delete m_pWebServer;
    m_pWebServer = NULL;
  }
  else
  {
    m_pWebServer->Listen();
    TRACE("Web Server listening at port %d\n", uPort);
  }
}

////////////////////////////////////////////////////////////////////////////////
//

void CDorgemApp::StopWebServer()
{
  if (!IsWebServerRunning())
    return;

  m_pWebServer->Close();
  delete m_pWebServer;
  m_pWebServer = NULL;
  TRACE("Web Server stopped\n");
}

////////////////////////////////////////////////////////////////////////////////
//

void AFXAPI DDV_MinChars(CDataExchange* pDX, CString const& strValue, int iChars)
{
  if (!pDX->m_bSaveAndValidate)
    return;

  if (strValue.GetLength() < iChars)
  {
    MsgBoxEx("String too short");
    pDX->Fail();
  }
}
