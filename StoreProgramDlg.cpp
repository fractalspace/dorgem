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
// StoreProgramDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "StoreProgramDlg.h"

#include "CaptionManager.h"
#include "CaptionSettingsDlg.h"
#include "StoreEvent.h"
#include "StoreProgram.h"
#include "Tm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStoreProgramDlg dialog

CStoreProgramDlg::CStoreProgramDlg(CWnd* pParent /*=NULL*/)
  : CHelpDialog(CStoreProgramDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CStoreProgramDlg)
  m_strCommand = _T("");
  m_bEnable = TRUE;
  m_dwInterval = DEFAULT_INTERVAL;
  m_strName = _T("");
  m_bResetTime = TRUE;
	m_bCaptionCustom = FALSE;
	//}}AFX_DATA_INIT

  m_pEvent = NULL;
  m_bCreate = FALSE;

  m_help.SetPopups("PopupsStoreProgram.txt");
}

void CStoreProgramDlg::DoDataExchange(CDataExchange* pDX)
{
  CHelpDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CStoreProgramDlg)
  DDX_Control(pDX, IDC_INTERVAL_TYPE, m_cmbIntervalType);
  DDX_Text(pDX, IDC_COMMAND, m_strCommand);
  DDX_Check(pDX, IDC_ENABLE, m_bEnable);
  DDX_Text(pDX, IDC_INTERVAL, m_dwInterval);
  DDX_Text(pDX, IDC_NAME, m_strName);
  DDX_Check(pDX, IDC_RESET_TIME, m_bResetTime);
	DDX_Check(pDX, IDC_CAP_CUST, m_bCaptionCustom);
	//}}AFX_DATA_MAP

  DDV_MinChars(pDX, m_strName, 1);
  DDV_MinChars(pDX, m_strCommand, 1);

  if (m_pEvent == NULL)
    DDV_StoreEventUnique(pDX, m_strName);

  if (pDX->m_bSaveAndValidate)
  {
    if (m_strCommand.Find("%s") == -1)
    {
      MsgBoxEx("The command MUST contain %s, which\n"
               "is replaced with the name of the JPEG file!");
      pDX->Fail();
    }
  }
}

BEGIN_MESSAGE_MAP(CStoreProgramDlg, CHelpDialog)
  //{{AFX_MSG_MAP(CStoreProgramDlg)
	ON_BN_CLICKED(IDC_CAP_CUST, OnCaptionCustom)
	ON_BN_CLICKED(IDC_CAPTION_SETTINGS, OnCaptionSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL CStoreProgramDlg::OnInitDialog()
{
  CHelpDialog::OnInitDialog();

  int iBetween = BETWEEN_SECS;

  // Create an event
  if (m_pEvent == NULL)
  {
    m_pEvent = new CStoreProgram;
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
    m_strCommand = m_pEvent->GetCommand();
    m_bResetTime = m_pEvent->ResetTime();
    m_bCaptionCustom = m_pEvent->HasCaptions();
    UpdateData(FALSE);
  }
  OnCaptionCustom();

  // Fill the interval type combo
  AddIntervalType("Hours", BETWEEN_HOURS, iBetween);
  AddIntervalType("Minutes", BETWEEN_MINS, iBetween);
  AddIntervalType("Seconds", BETWEEN_SECS, iBetween);

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreProgramDlg::OnOK()
{
  // Validate the entered values
  if (!UpdateData())
    return;

  // Is the interval type combo filled?
  int iIndex = m_cmbIntervalType.GetCurSel();
  if (iIndex == LB_ERR)
    return;

  // Store setting in the event
  m_pEvent->SetName(m_strName);
  m_pEvent->SetEnabled(m_bEnable);
  m_pEvent->SetInterval(m_dwInterval, m_cmbIntervalType.GetItemData(iIndex));
  m_pEvent->SetCommand(m_strCommand);
  m_pEvent->ResetTime(m_bResetTime);

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

void CStoreProgramDlg::OnCancel() 
{
  if (m_bCreate)
    delete m_pEvent;

	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreProgramDlg::AddIntervalType(LPCSTR szText, int iType, int iSel)
{
  int iIndex = m_cmbIntervalType.AddString(szText);
  m_cmbIntervalType.SetItemData(iIndex, iType);
  if (iSel == iType)
    m_cmbIntervalType.SetCurSel(iIndex);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreProgramDlg::OnCaptionCustom() 
{
  TRACE("[CStoreProgramDlg]OnCaptionCustom()\n");

  BOOL bCaptions = IsDlgButtonChecked(IDC_CAP_CUST);
  GetDlgItem(IDC_CAPTION_SETTINGS)->EnableWindow(bCaptions);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreProgramDlg::OnCaptionSettings() 
{
  TRACE("[CStoreProgramDlg]OnCaptionSettings()\n");

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

