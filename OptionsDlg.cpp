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
// OptionsDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COptionsDlg, CPropertySheet)

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg

COptionsDlg::COptionsDlg(CWnd* pParentWnd, UINT uSelectPage)
  : CPropertySheet(IDS_PRODUCT_NAME, pParentWnd, uSelectPage)
{
  m_bInitTransparent = TRUE;

  // Add the pages to the PropertySheet
  AddPage(&m_dlgGeneral);
  AddPage(&m_dlgAvi);
  AddPage(&m_dlgWebServer);

  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = GetApp()->LoadIcon(IDR_MAINFRAME);

  // Hide the 'Apply Now' button
  m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

/////////////////////////////////////////////////////////////////////////////
// Clean up the mess

COptionsDlg::~COptionsDlg()
{
}

/////////////////////////////////////////////////////////////////////////////
//

#ifndef WM_APPCOMMAND
#define WM_APPCOMMAND                  0x0319
#define FAPPCOMMAND_MASK               0x8000
#define GET_APPCOMMAND_LPARAM(lParam)  ((short)(HIWORD(lParam) & ~FAPPCOMMAND_MASK))
#define APPCOMMAND_BROWSER_BACKWARD    1
#define APPCOMMAND_BROWSER_FORWARD     2
#endif

BEGIN_MESSAGE_MAP(COptionsDlg, CPropertySheet)
  //{{AFX_MSG_MAP(COptionsDlg)
  //}}AFX_MSG_MAP
  ON_MESSAGE(WM_APPCOMMAND, OnAppCommand)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Set the window to iPercent visibility

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#endif

typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
static lpfnSetLayeredWindowAttributes mySetLayeredWindowAttributes;

void COptionsDlg::MakeTransparent(int iPercent)
{
  // First check if we use Windows 2000 or newer?
  if (!GetApp()->m_bWin2k)
    return;

  // Initialize for transparancy
  if (m_bInitTransparent)
  {
    // Here we import the function from USER32.DLL
    HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
    mySetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");

    // If the import did not succeed, don't go any further!
    if (mySetLayeredWindowAttributes == NULL)
      return;

    // Check the current state of the dialog, and then add the WS_EX_LAYERED attribute
    SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    // All first time initialization is done
    m_bInitTransparent = FALSE;
  }

  // Sets the window to iPercent visibility
  ASSERT(iPercent > 0 && iPercent <= 100);
  double dPercent = 255.0 * ((double) iPercent / 100.0);
  mySetLayeredWindowAttributes(m_hWnd, 0, (BYTE) dPercent, LWA_ALPHA);
}

/////////////////////////////////////////////////////////////////////////////
// Initialize the dialog

BOOL COptionsDlg::OnInitDialog()
{
  BOOL bResult = CPropertySheet::OnInitDialog();

  // Set the icon for this dialog. The framework does this automatically
  // when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);       // Set big icon

  // First try to load the 16*16 icon. If this fails, then use the 32*32
  HICON hIcon = (HICON) ::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0);
  if (hIcon == NULL)
    SetIcon(m_hIcon, FALSE);    // Set small icon
  else
    SetIcon(hIcon, FALSE);      // Set small icon

  // Add a context-help-button
  ModifyStyleEx(0, WS_EX_CONTEXTHELP);

  return(bResult);
}

/////////////////////////////////////////////////////////////////////////////
// With the 'Browser Backward' and 'Browser Forward' keys of an Internet
// Keyboard you can go to the previous or next tab

LRESULT COptionsDlg::OnAppCommand(WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(wParam);

  // Here we only handle the Browser Backward and Browser Forward keys
  int iCommand = GET_APPCOMMAND_LPARAM(lParam);
  if (iCommand != APPCOMMAND_BROWSER_BACKWARD && iCommand != APPCOMMAND_BROWSER_FORWARD)
    return(FALSE);

  // With only one (or less) page, there is no need to switch the page
  int iPages = m_pages.GetSize();
  if (iPages <= 1)
    return(TRUE);

  // This is the current page
  int iIndex = GetActiveIndex();

  // Which key is pressed?
  switch (iCommand)
  {
    case APPCOMMAND_BROWSER_BACKWARD:
      if (iIndex == 0)
        iIndex = iPages - 1;
      else
        iIndex--;
      break;
    case APPCOMMAND_BROWSER_FORWARD:
      if (iIndex == iPages - 1)
        iIndex = 0;
      else
        iIndex++;
      break;
  }

  // Set the new active page
  SetActivePage(iIndex);
  return(TRUE);
}
