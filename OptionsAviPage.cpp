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
// OptionsAviPage.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "OptionsAviPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsAviPage property page

IMPLEMENT_DYNCREATE(COptionsAviPage, CHelpPropertyPage)

COptionsAviPage::COptionsAviPage() : CHelpPropertyPage(COptionsAviPage::IDD)
{
  //{{AFX_DATA_INIT(COptionsAviPage)
  m_strFilename  = GetApp()->GetProfileString(szGeneral, szArchiveAVI);
  m_uFramesPerSecond = GetApp()->GetProfileInt(szGeneral, szFramesPerSecond, 1);
  m_bChangeDay = GetApp()->GetProfileInt(szGeneral, szAVIChangeInterval, FALSE);
  m_bUseAVI = GetApp()->GetProfileInt(szGeneral, szUseAVI, FALSE);
  //}}AFX_DATA_INIT

  m_help.SetPopups("PopupsOptionsAVI.txt");
}

COptionsAviPage::~COptionsAviPage()
{
}

void COptionsAviPage::DoDataExchange(CDataExchange* pDX)
{
  CHelpPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(COptionsAviPage)
  DDX_Control(pDX, IDC_FPS_SPIN, m_spinFramesPerSecond);
  DDX_Text(pDX, IDC_FILE, m_strFilename);
  DDX_Text(pDX, IDC_FPS, m_uFramesPerSecond);
  DDV_MinMaxUInt(pDX, m_uFramesPerSecond, 1, 30);
  DDX_Check(pDX, IDC_CHANGE, m_bChangeDay);
  DDX_Check(pDX, IDC_USE_AVI, m_bUseAVI);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionsAviPage, CHelpPropertyPage)
  //{{AFX_MSG_MAP(COptionsAviPage)
  ON_BN_CLICKED(IDC_USE_AVI, OnUseAvi)
  ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL COptionsAviPage::OnInitDialog()
{
  CHelpPropertyPage::OnInitDialog();

  // Configure the spin control
  m_spinFramesPerSecond.SetBuddy(GetDlgItem(IDC_FPS));
  m_spinFramesPerSecond.SetRange(1, 30);

  // Enable the items
  OnUseAvi();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsAviPage::OnOK()
{
  // Save the settings
  GetApp()->WriteProfileInt(szGeneral, szUseAVI, m_bUseAVI);
  GetApp()->WriteProfileString(szGeneral, szArchiveAVI, m_strFilename);
  GetApp()->WriteProfileInt(szGeneral, szFramesPerSecond, m_uFramesPerSecond);
  GetApp()->WriteProfileInt(szGeneral, szAVIChangeInterval, m_bChangeDay);

  CHelpPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL COptionsAviPage::OnKillActive()
{
  CString strFile;
  GetDlgItem(IDC_FILE)->GetWindowText(strFile);



  return(CHelpPropertyPage::OnKillActive());
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsAviPage::OnUseAvi()
{
  BOOL bEnable = IsDlgButtonChecked(IDC_USE_AVI);
  GetDlgItem(IDC_FILE)->EnableWindow(bEnable);
  GetDlgItem(IDC_BROWSE)->EnableWindow(bEnable);
  GetDlgItem(IDC_FPS)->EnableWindow(bEnable);
  GetDlgItem(IDC_CHANGE)->EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsAviPage::OnBrowse()
{
  TRACE("[COptionsAviPage]OnBrowse()\n");

  CFileDialog dlg(FALSE, "*.avi", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
     "Video files (*.avi)|*.avi|"
     "All files (*.*)|*.*|"
     "|");

  if (dlg.DoModal() == IDOK)
    GetDlgItem(IDC_FILE)->SetWindowText(dlg.m_ofn.lpstrFile);
}
