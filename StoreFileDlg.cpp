/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2001-2003 Frank Fesevur                                  //
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
// StoreFileDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "StoreFileDlg.h"

#include "StoreEvent.h"
#include "StoreFile.h"
#include "Tm.h"
#include "CaptionSettingsDlg.h"
#include "CaptionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStoreFileDlg dialog

CStoreFileDlg::CStoreFileDlg(CWnd* pParent /*=NULL*/)
  : CHelpDialog(CStoreFileDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CStoreFileDlg)
  m_strFilename = _T("");
  m_dwInterval = DEFAULT_INTERVAL;
  m_strName = _T("");
  m_bEnable = TRUE;
  m_bResetTime = TRUE;
  m_bCaptionCustom = FALSE;
	m_bCreateDirs = FALSE;
	//}}AFX_DATA_INIT

  m_pEvent = NULL;
  m_bCreate = FALSE;

  m_help.SetPopups("PopupsStoreFile.txt");
}

void CStoreFileDlg::DoDataExchange(CDataExchange* pDX)
{
  CHelpDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CStoreFileDlg)
  DDX_Control(pDX, IDC_INTERVAL_TYPE, m_cmbIntervalType);
  DDX_Text(pDX, IDC_FILE, m_strFilename);
  DDX_Text(pDX, IDC_INTERVAL, m_dwInterval);
  DDV_MinMaxDWord(pDX, m_dwInterval, 0, 3600);
  DDX_Text(pDX, IDC_NAME, m_strName);
  DDX_Check(pDX, IDC_ENABLE, m_bEnable);
  DDX_Check(pDX, IDC_RESET_TIME, m_bResetTime);
  DDX_Check(pDX, IDC_CAP_CUST, m_bCaptionCustom);
	DDX_Check(pDX, IDC_CREATE_DIRS, m_bCreateDirs);
	//}}AFX_DATA_MAP

  DDV_MinChars(pDX, m_strName, 1);
  DDV_MinChars(pDX, m_strFilename, 1);

  if (m_pEvent == NULL)
    DDV_StoreEventUnique(pDX, m_strName);
}

BEGIN_MESSAGE_MAP(CStoreFileDlg, CHelpDialog)
  //{{AFX_MSG_MAP(CStoreFileDlg)
  ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  ON_BN_CLICKED(IDC_CAP_CUST, OnCaptionCustom)
  ON_BN_CLICKED(IDC_CAPTION_SETTINGS, OnCaptionSettings)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreFileDlg::OnInitDialog()
{
  CHelpDialog::OnInitDialog();

  int iBetween = BETWEEN_SECS;

  // Create an event
  if (m_pEvent == NULL)
  {
    m_pEvent = new CStoreFile;
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
    m_strFilename = m_pEvent->GetFile();
    m_bResetTime = m_pEvent->ResetTime();
    m_bCaptionCustom = m_pEvent->HasCaptions();
    m_bCreateDirs = m_pEvent->CreateDirs();
    UpdateData(FALSE);

  }
  OnCaptionCustom();

  // Fill the combo
  AddIntervalType("Hours", BETWEEN_HOURS, iBetween);
  AddIntervalType("Minutes", BETWEEN_MINS, iBetween);
  AddIntervalType("Seconds", BETWEEN_SECS, iBetween);

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFileDlg::OnOK()
{
  // Validate the entered values
  if (!UpdateData())
    return;

  int iIndex = m_cmbIntervalType.GetCurSel();
  if (iIndex == LB_ERR)
    return;

  // Store setting in the event
  m_pEvent->SetName(m_strName);
  m_pEvent->SetEnabled(m_bEnable);
  m_pEvent->SetInterval(m_dwInterval, m_cmbIntervalType.GetItemData(iIndex));
  m_pEvent->SetFile(m_strFilename);
  m_pEvent->ResetTime(m_bResetTime);
  m_pEvent->CreateDirs(m_bCreateDirs);

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

void CStoreFileDlg::OnCancel()
{
  if (m_bCreate)
    delete m_pEvent;

  CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFileDlg::OnBrowse()
{
  TRACE("[CStoreFileDlg]OnBrowse()\n");

  CFileDialog dlg(FALSE, "*.jpg", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
     "JPEG files (*.jpg,*.jpeg)|*.jpg;*.jpeg|"
     "All files (*.*)|*.*|"
     "|");

  if (dlg.DoModal() == IDOK)
    GetDlgItem(IDC_FILE)->SetWindowText(dlg.m_ofn.lpstrFile);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFileDlg::AddIntervalType(LPCSTR szText, int iType, int iSel)
{
  int iIndex = m_cmbIntervalType.AddString(szText);
  m_cmbIntervalType.SetItemData(iIndex, iType);
  if (iSel == iType)
    m_cmbIntervalType.SetCurSel(iIndex);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFileDlg::OnCaptionCustom()
{
  TRACE("[CStoreFileDlg]OnCaptionCustom()\n");

  BOOL bCaptions = IsDlgButtonChecked(IDC_CAP_CUST);
  GetDlgItem(IDC_CAPTION_SETTINGS)->EnableWindow(bCaptions);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreFileDlg::OnCaptionSettings()
{
  TRACE("[CStoreFileDlg]OnCaptionSettings()\n");

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
