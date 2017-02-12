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
// OptionsGeneralPage.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "OptionsGeneralPage.h"

#include "DorgemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsGeneralPage property page

IMPLEMENT_DYNCREATE(COptionsGeneralPage, CHelpPropertyPage)

COptionsGeneralPage::COptionsGeneralPage() : CHelpPropertyPage(COptionsGeneralPage::IDD)
{
  //{{AFX_DATA_INIT(COptionsGeneralPage)
  m_bTrayIcon = GetApp()->GetProfileInt(szGeneral, szTrayIcon, TRUE);
  m_bUseMotionDetection = GetApp()->GetProfileInt(szGeneral, szMotionDetection, FALSE);
  m_bReconnect = GetApp()->GetProfileInt(szGeneral, szReconnect, FALSE);
  m_iReconnectTime = GetApp()->GetProfileInt(szGeneral, szReconnectTime, 1);
  m_bShowMsgBoxes = GetApp()->GetProfileInt(szGeneral, szShowMsgBoxes, TRUE);
  //}}AFX_DATA_INIT

  m_help.SetPopups("PopupsOptionsGeneral.txt");
}

COptionsGeneralPage::~COptionsGeneralPage()
{
}

void COptionsGeneralPage::DoDataExchange(CDataExchange* pDX)
{
  CHelpPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(COptionsGeneralPage)
  DDX_Check(pDX, IDC_TRAY_ICON, m_bTrayIcon);
  DDX_Check(pDX, IDC_MOTION_DETECTION, m_bUseMotionDetection);
  DDX_Check(pDX, IDC_RECONNECT, m_bReconnect);
  DDX_Text(pDX, IDC_RECONNECT_TIME, m_iReconnectTime);
  DDV_MinMaxInt(pDX, m_iReconnectTime, 1, 180);
  DDX_Check(pDX, IDC_SHOW_MSGBOX, m_bShowMsgBoxes);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionsGeneralPage, CHelpPropertyPage)
  //{{AFX_MSG_MAP(COptionsGeneralPage)
  ON_BN_CLICKED(IDC_MOTION_DETECTION, OnMotionDetection)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL COptionsGeneralPage::OnInitDialog()
{
  CHelpPropertyPage::OnInitDialog();

  OnMotionDetection();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsGeneralPage::OnOK()
{
  // Save the settings
  GetApp()->WriteProfileInt(szGeneral, szTrayIcon, m_bTrayIcon);
  GetApp()->WriteProfileInt(szGeneral, szMotionDetection, m_bUseMotionDetection);
  GetApp()->WriteProfileInt(szGeneral, szReconnect, m_bReconnect);
  GetApp()->WriteProfileInt(szGeneral, szReconnectTime, m_iReconnectTime);
  GetApp()->WriteProfileInt(szGeneral, szShowMsgBoxes, m_bShowMsgBoxes);

  // Refresh the settings in the dialog
  GetMainWnd()->ReadSettings();

  CHelpPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsGeneralPage::OnMotionDetection()
{
  BOOL bEnable = IsDlgButtonChecked(IDC_MOTION_DETECTION);
  GetDlgItem(IDC_RECONNECT)->EnableWindow(bEnable);
  GetDlgItem(IDC_RECONNECT_TIME)->EnableWindow(bEnable);
  GetDlgItem(IDC_MINUTES)->EnableWindow(bEnable);
}
