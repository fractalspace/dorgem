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
// StoreFTPDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "StoreFTPDlg.h"

#include "DunManager.h"
#include "StoreEvent.h"
#include "StoreFTP.h"
#include "Tm.h"
#include "CaptionManager.h"
#include "CaptionSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStoreFTPDlg dialog

CStoreFTPDlg::CStoreFTPDlg(CWnd* pParent /*=NULL*/)
  : CHelpDialog(CStoreFTPDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CStoreFTPDlg)
  m_strFilename = _T("");
  m_strHost = _T("");
  m_strName = _T("");
  m_strPassword = _T("");
  m_strUser = _T("");
  m_dwInterval = DEFAULT_INTERVAL;
  m_bUseRAS = FALSE;
  m_bEnable = TRUE;
  m_bResetTime = TRUE;
  m_bKeepOpen = FALSE;
  m_bCaptionCustom = FALSE;
  m_dwPort = INTERNET_DEFAULT_FTP_PORT;
  m_bPassiveFTP = FALSE;
  //}}AFX_DATA_INIT

  m_pEvent = NULL;
  m_bCreate = FALSE;

  m_help.SetPopups("PopupsStoreFTP.txt");
}

void CStoreFTPDlg::DoDataExchange(CDataExchange* pDX)
{
  CHelpDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CStoreFTPDlg)
  DDX_Control(pDX, IDC_INTERVAL_TYPE, m_cmbIntervalType);
  DDX_Control(pDX, IDC_RAS_CONN, m_cmbRasConn);
  DDX_Text(pDX, IDC_FILE, m_strFilename);
  DDX_Text(pDX, IDC_HOST, m_strHost);
  DDX_Text(pDX, IDC_NAME, m_strName);
  DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
  DDX_Text(pDX, IDC_USER, m_strUser);
  DDX_Text(pDX, IDC_INTERVAL, m_dwInterval);
  DDV_MinMaxDWord(pDX, m_dwInterval, 0, 3600);
  DDX_Check(pDX, IDC_USE_RAS, m_bUseRAS);
  DDX_Check(pDX, IDC_ENABLE, m_bEnable);
  DDX_Check(pDX, IDC_RESET_TIME, m_bResetTime);
  DDX_Check(pDX, IDC_KEEP_OPEN, m_bKeepOpen);
  DDX_Check(pDX, IDC_CAP_CUST, m_bCaptionCustom);
  DDX_Text(pDX, IDC_PORT, m_dwPort);
  DDX_Check(pDX, IDC_PASSIVE_FTP, m_bPassiveFTP);
  //}}AFX_DATA_MAP

  DDV_MinChars(pDX, m_strName, 1);
  DDV_MinChars(pDX, m_strHost, 1);
  DDV_MinChars(pDX, m_strUser, 1);
  DDV_MinChars(pDX, m_strPassword, 1);
  DDV_MinChars(pDX, m_strFilename, 1);

  if (m_pEvent == NULL)
    DDV_StoreEventUnique(pDX, m_strName);
}

BEGIN_MESSAGE_MAP(CStoreFTPDlg, CHelpDialog)
  //{{AFX_MSG_MAP(CStoreFTPDlg)
  ON_BN_CLICKED(IDC_USE_RAS, OnUseRas)
  ON_BN_CLICKED(IDC_CAP_CUST, OnCaptionCustom)
  ON_BN_CLICKED(IDC_CAPTION_SETTINGS, OnCaptionSettings)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreFTPDlg::OnInitDialog()
{
  CHelpDialog::OnInitDialog();

  BOOL bRas = TRUE;
  CDunManager* pDun = GetDunMngr();
  if (pDun->HasRas())
  {
    CString str = pDun->GetFirstEntry();
    while (!str.IsEmpty())
    {
      m_cmbRasConn.AddString(str);
      str = pDun->GetNextEntry();
    }
  }
  if (m_cmbRasConn.GetCount() == 0)
  {
    GetDlgItem(IDC_USE_RAS)->EnableWindow(FALSE);
    GetDlgItem(IDC_RAS_CONN)->EnableWindow(FALSE);
    bRas = FALSE;
  }

  int iBetween = BETWEEN_SECS;

  // Create an event
  if (m_pEvent == NULL)
  {
    m_pEvent = new CStoreFTP;
    ASSERT(m_pEvent != NULL);
    m_bCreate = TRUE;
  }
  else
  {
    time_t tInterval;
    m_pEvent->GetInterval(tInterval, iBetween);
    m_dwInterval = (DWORD) tInterval;

    GetDlgItem(IDC_NAME)->EnableWindow(FALSE);
    m_strName = m_pEvent->GetName();
    m_bEnable = m_pEvent->IsEnabled();
    m_strHost = m_pEvent->GetHost();
    m_dwPort = m_pEvent->GetPort();
    m_strUser = m_pEvent->GetUser();
    m_strPassword = m_pEvent->GetPassword();
    m_strFilename = m_pEvent->GetFile();
    m_bKeepOpen = m_pEvent->KeepOpen();
    m_bUseRAS = (bRas ? m_pEvent->UseRAS() : FALSE);
    m_bPassiveFTP = m_pEvent->UsePassiveFTP();
    m_bCaptionCustom = m_pEvent->HasCaptions();
    m_bResetTime = m_pEvent->ResetTime();
    UpdateData(FALSE);

    if (m_bUseRAS)
    {
      int iIndex =  m_cmbRasConn.FindString(-1, m_pEvent->GetConnection());
      if (iIndex != CB_ERR)
        m_cmbRasConn.SetCurSel(iIndex);
    }

  }
  OnCaptionCustom();
  OnUseRas();

  // Fill the interval type combo
  AddIntervalType("Hours", BETWEEN_HOURS, iBetween);
  AddIntervalType("Minutes", BETWEEN_MINS, iBetween);
  AddIntervalType("Seconds", BETWEEN_SECS, iBetween);

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFTPDlg::OnUseRas()
{
  TRACE("[CStoreFTPDlg]OnUseRas\n");

  BOOL bUseRAS = IsDlgButtonChecked(IDC_USE_RAS);
  GetDlgItem(IDC_RAS_CONN)->EnableWindow(bUseRAS);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFTPDlg::OnOK()
{
  TRACE("[CStoreFTPDlg]OnOK()\n");

  if (!UpdateData())
    return;

  int iIndex = m_cmbIntervalType.GetCurSel();
  if (iIndex == LB_ERR)
    return;

  if (m_pEvent == NULL)
  {
    m_pEvent = new CStoreFTP;
    ASSERT(m_pEvent != NULL);
  }

  m_pEvent->SetName(m_strName);
  m_pEvent->SetEnabled(m_bEnable);
  m_pEvent->SetHost(m_strHost);
  m_pEvent->SetPort(m_dwPort);
  m_pEvent->SetUser(m_strUser);
  m_pEvent->SetPassword(m_strPassword);
  m_pEvent->SetFile(m_strFilename);
  m_pEvent->KeepOpen(m_bKeepOpen);
  m_pEvent->UseRAS(m_bUseRAS);
  m_pEvent->UsePassiveFTP(m_bPassiveFTP);
  m_pEvent->SetInterval(m_dwInterval, m_cmbIntervalType.GetItemData(iIndex));
  m_pEvent->ResetTime(m_bResetTime);

  iIndex = m_cmbRasConn.GetCurSel();
  CString strConnection;
  if (iIndex != LB_ERR)
  {
    m_cmbRasConn.GetLBText(iIndex, strConnection);
    m_pEvent->SetConnection(strConnection);
  }

  // Need to add a caption manager?
  if (m_bCaptionCustom)
  {
    if (!m_pEvent->HasCaptions())
    {
      CCaptionManager* pMngr;
      pMngr = new CCaptionManager(m_pEvent->GetRegCaption());
      m_pEvent->SetCaptions(pMngr);
    }
  }
  else
    m_pEvent->DeleteCaptions();

  CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFTPDlg::OnCancel()
{
  if (m_bCreate)
    delete m_pEvent;

  CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFTPDlg::AddIntervalType(LPCSTR szText, int iType, int iSel)
{
  int iIndex = m_cmbIntervalType.AddString(szText);
  m_cmbIntervalType.SetItemData(iIndex, iType);
  if (iSel == iType)
    m_cmbIntervalType.SetCurSel(iIndex);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFTPDlg::OnCaptionCustom()
{
  BOOL bCaptions = IsDlgButtonChecked(IDC_CAP_CUST);
  GetDlgItem(IDC_CAPTION_SETTINGS)->EnableWindow(bCaptions);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFTPDlg::OnCaptionSettings()
{
  // If the event doesn't have captions, create them
  CCaptionManager* pMngr;
  if (m_pEvent->HasCaptions())
    pMngr = m_pEvent->GetCaptions();
  else
  {
    pMngr = new CCaptionManager(m_pEvent->GetRegCaption());
    ASSERT(pMngr != NULL);
  }

  // Show the dialog (with the proper caption manager)
  CCaptionSettingsDlg dlg;
  dlg.SetCaptionMngr(pMngr);
  pMngr->SetAutoSave(FALSE);
  dlg.DoModal();

  // Do we need to add the caption manager to this event
  if (!m_pEvent->HasCaptions())
  {
    if (pMngr->GetFirstEvent() == NULL)
      delete pMngr;
    else
    {
      m_pEvent->SetCaptions(pMngr);
      pMngr->SetAutoSave(TRUE);
    }
  }
}

