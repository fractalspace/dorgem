/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2002-2005 Frank Fesevur                                  //
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
// OptionsWebServerPage.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "OptionsWebServerPage.h"

#include "ListenSocket.h"
#include "CaptionManager.h"
#include "CaptionSettingsDlg.h"
#include "RegKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsWebServerPage property page

IMPLEMENT_DYNCREATE(COptionsWebServerPage, CHelpPropertyPage)

COptionsWebServerPage::COptionsWebServerPage() : CHelpPropertyPage(COptionsWebServerPage::IDD)
{
  // Read the captions for the web server
  m_strCapReg = GetApp()->m_strRegKey + szGeneralWebServer;
  CString strCap = m_strCapReg;
  strCap += "\\";
  strCap += szCaptionEvents;

  CRegKey reg;
  if (reg.Open(HKEY_CURRENT_USER, strCap))
  {
    reg.Close();
    m_pCaptions = new CCaptionManager(m_strCapReg);
    ASSERT(m_pCaptions != NULL);
  }
  else
    m_pCaptions = NULL;

  //{{AFX_DATA_INIT(COptionsWebServerPage)
  m_uPort = GetApp()->GetProfileInt(szGeneralWebServer, szPort, DEFAULT_WEB_PORT);
  m_uRefreshRate = GetApp()->GetProfileInt(szGeneralWebServer, szRefreshRate, 0);
  m_strLogFile = GetApp()->GetProfileString(szGeneralWebServer, szAccessLog);
  m_bCaptionCustom = (m_pCaptions != NULL);
  //}}AFX_DATA_INIT

  m_help.SetPopups("PopupsOptionsWebServer.txt");
}

COptionsWebServerPage::~COptionsWebServerPage()
{
}

void COptionsWebServerPage::DoDataExchange(CDataExchange* pDX)
{
  CHelpPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(COptionsWebServerPage)
  DDX_Control(pDX, IDC_SERVER_TYPE, m_cmbServerType);
  DDX_Text(pDX, IDC_PORT, m_uPort);
  DDX_Text(pDX, IDC_REFRESH, m_uRefreshRate);
  DDV_MinMaxUInt(pDX, m_uRefreshRate, 0, 60);
  DDX_Text(pDX, IDC_LOG_FILE, m_strLogFile);
  DDX_Check(pDX, IDC_CAP_CUST, m_bCaptionCustom);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionsWebServerPage, CHelpPropertyPage)
  //{{AFX_MSG_MAP(COptionsWebServerPage)
  ON_CBN_SELCHANGE(IDC_SERVER_TYPE, OnSelectionChangedServerType)
  ON_EN_CHANGE(IDC_PORT, SetModified)
  ON_BN_CLICKED(IDC_CAPTION_SETTINGS, OnCaptionSettings)
  ON_EN_CHANGE(IDC_REFRESH, SetModified)
  ON_EN_CHANGE(IDC_LOG_FILE, SetModified)
  ON_BN_CLICKED(IDC_CAP_CUST, OnCaptionCustom)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL COptionsWebServerPage::OnInitDialog()
{
  CHelpPropertyPage::OnInitDialog();

  int iType = GetApp()->GetProfileInt(szGeneralWebServer, szWebServerType, WEB_SERVER_NONE);
  AddServerTypeString("None", WEB_SERVER_NONE, iType);
  AddServerTypeString("Still images", WEB_SERVER_STILL, iType);
  //AddServerTypeString("Streaming", WEB_SERVER_STREAM, iType);
  OnSelectionChangedServerType();
  OnCaptionCustom();

  // Reset the modified flag
  m_bModified = FALSE;

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsWebServerPage::OnOK()
{
  int iType, iIndex = m_cmbServerType.GetCurSel();
  if (iIndex == CB_ERR)
    iType = WEB_SERVER_NONE;
  else
    iType = m_cmbServerType.GetItemData(iIndex);

  GetApp()->WriteProfileInt(szGeneralWebServer, szWebServerType, iType);
  GetApp()->WriteProfileInt(szGeneralWebServer, szPort, m_uPort);
  GetApp()->WriteProfileInt(szGeneralWebServer, szRefreshRate, m_uRefreshRate);
  GetApp()->WriteProfileString(szGeneralWebServer, szAccessLog, m_strLogFile);

  // Store or delete the custom captions
  if (m_bCaptionCustom)
  {
    if (m_pCaptions != NULL)
    {
      m_pCaptions->WriteEvents();
      delete m_pCaptions;
    }
  }
  else
  {
    if (m_pCaptions != NULL)
    {
      delete m_pCaptions;
      m_pCaptions = NULL;
    }
  }

  // (Re)start or stop the server
  if (iType == WEB_SERVER_NONE)
    GetApp()->StopWebServer();
  else
  {
    if (!GetApp()->IsWebServerRunning() || m_bModified)
      GetApp()->StartWebServer();
  }

  CHelpPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsWebServerPage::AddServerTypeString(LPCSTR szStr, int iValue, int iType)
{
  int iIndex = m_cmbServerType.AddString(szStr);
  if (iIndex == CB_ERR)
    return;

  m_cmbServerType.SetItemData(iIndex, iValue);
  if (iValue == iType)
    m_cmbServerType.SetCurSel(iIndex);
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsWebServerPage::OnSelectionChangedServerType()
{
  int iType, iIndex = m_cmbServerType.GetCurSel();
  if (iIndex == CB_ERR)
    iType = WEB_SERVER_NONE;
  else
    iType = m_cmbServerType.GetItemData(iIndex);

  BOOL bEnabled = TRUE;
  switch (iType)
  {
    case WEB_SERVER_NONE:
      bEnabled = FALSE;
      break;

    case WEB_SERVER_STILL:
      GetDlgItem(IDC_REFRESH_TYPE)->SetWindowText("seconds");
      bEnabled = TRUE;
      break;

    case WEB_SERVER_STREAM:
      GetDlgItem(IDC_REFRESH_TYPE)->SetWindowText("frames per second");
      bEnabled = TRUE;
      break;
  }

  GetDlgItem(IDC_PORT)->EnableWindow(bEnabled);
  GetDlgItem(IDC_REFRESH)->EnableWindow(bEnabled);
  GetDlgItem(IDC_LOG_FILE)->EnableWindow(bEnabled);
  GetDlgItem(IDC_CAP_CUST)->EnableWindow(bEnabled);
  GetDlgItem(IDC_CAPTION_SETTINGS)->EnableWindow(bEnabled ? (m_pCaptions != NULL) : FALSE);

  SetModified();
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsWebServerPage::SetModified()
{
  m_bModified = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsWebServerPage::OnCaptionCustom()
{
  BOOL bCaptions = IsDlgButtonChecked(IDC_CAP_CUST);
  GetDlgItem(IDC_CAPTION_SETTINGS)->EnableWindow(bCaptions);
}

/////////////////////////////////////////////////////////////////////////////
//

void COptionsWebServerPage::OnCaptionSettings()
{
  // If the web server doesn't have captions, create them
  CCaptionManager* pMngr = m_pCaptions;
  if (pMngr == NULL)
  {
    pMngr = new CCaptionManager(m_strCapReg);
    ASSERT(pMngr != NULL);
  }

  // Show the dialog (with the proper caption manager)
  CCaptionSettingsDlg dlg;
  dlg.SetCaptionMngr(pMngr);
  pMngr->SetAutoSave(FALSE);
  dlg.DoModal();

  // Make sure the server is restarted to let these changes take effect
  SetModified();

  // Do we need to link the caption manager to the webserver?
  if (m_pCaptions == NULL)
  {
    if (pMngr->GetFirstEvent() == NULL)
      delete pMngr;
    else
    {
      m_pCaptions = pMngr;
      pMngr->SetAutoSave(TRUE);
    }
  }
}
