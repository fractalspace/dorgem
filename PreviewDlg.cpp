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
// PreviewDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "PreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg dialog

CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CPreviewDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CPreviewDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPreviewDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
  //{{AFX_MSG_MAP(CPreviewDlg)
  ON_WM_DESTROY()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Move to the default position

BOOL CPreviewDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Get saved position
  CString strPos = GetApp()->GetProfileString(szGeneral, szPreviewPos);
  int iIndex = strPos.Find(',');
  if (iIndex != -1) // Bail out if the string is odd or if no string
  {
    // Get the window size
    CRect place;
    GetWindowRect(place);
    LONG lSizeX = place.right - place.left;
    LONG lSizeY = place.bottom - place.top;

    // Determine the new position
    place.left = atoi(strPos);
    place.right = place.left + lSizeX;
    place.top = atoi(strPos.Mid(iIndex + 1));
    place.bottom = place.top + lSizeY;

    // Move the window
    MoveWindow(place);
  }

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Save Window Position

void CPreviewDlg::OnDestroy()
{
  WINDOWPLACEMENT wndpl;
  wndpl.length = sizeof(WINDOWPLACEMENT);

  // Gets current window position
  if (GetWindowPlacement(&wndpl))
  {
    CString strText;
    strText.Format("%d,%d", wndpl.rcNormalPosition.left,
                                        wndpl.rcNormalPosition.top);
    GetApp()->WriteProfileString(szGeneral, szPreviewPos, strText);
  }

  CDialog::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// Disable the keyboard input to this dialog. Mainly to prevent that ESC
// cancels the dialog

BOOL CPreviewDlg::PreTranslateMessage(MSG* pMsg)
{
  if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
    return(TRUE);

  return(CDialog::PreTranslateMessage(pMsg));
}
