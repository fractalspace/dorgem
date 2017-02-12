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
// CaptionBitmapDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "CaptionBitmapDlg.h"

#include "CaptionEvent.h"
#include "CaptionBitmap.h"
#include "CaptionPosDlg.h"
#include "ColorSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaptionBitmapDlg dialog

CCaptionBitmapDlg::CCaptionBitmapDlg(CWnd* pParent /*=NULL*/)
  : CHelpDialog(CCaptionBitmapDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCaptionBitmapDlg)
  m_strBitmap = _T("");
  m_bEnabled = FALSE;
  m_strName = _T("");
  //}}AFX_DATA_INIT

  m_pMngr = NULL;
  m_pEvent = NULL;
  m_bCreate = FALSE;
  m_help.SetPopups("PopupsCaptionBitmap.txt");
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionBitmapDlg::DoDataExchange(CDataExchange* pDX)
{
  CHelpDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCaptionBitmapDlg)
  DDX_Control(pDX, IDC_BMP_TYPE, m_cmbType);
  DDX_Text(pDX, IDC_BITMAP, m_strBitmap);
  DDX_Check(pDX, IDC_ENABLE, m_bEnabled);
  DDX_Text(pDX, IDC_NAME, m_strName);
  //}}AFX_DATA_MAP

  DDV_MinChars(pDX, m_strName, 1);
  DDV_MinChars(pDX, m_strBitmap, 1);

  if (m_bCreate)
    DDV_CaptionEventUnique(pDX, m_strName, m_pMngr);
}

BEGIN_MESSAGE_MAP(CCaptionBitmapDlg, CHelpDialog)
  //{{AFX_MSG_MAP(CCaptionBitmapDlg)
  ON_EN_KILLFOCUS(IDC_BITMAP, OnKillFocusBitmap)
  ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  ON_BN_CLICKED(IDC_POSITION, OnPosition)
  ON_CBN_SELCHANGE(IDC_BMP_TYPE, OnSelectionChangedBmpType)
  ON_BN_CLICKED(IDC_TRANS_SELECT, OnTransparentSelect)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionBitmapDlg::OnInitDialog()
{
  CHelpDialog::OnInitDialog();

  ASSERT(m_pMngr != NULL);

  m_cmbType.AddString("Multiply");
  m_cmbType.AddString("Transparent");
  m_cmbType.AddString("Copy");

  if (m_pEvent == NULL)
  {
    m_pEvent = new CCaptionBitmap;
    ASSERT(m_pEvent != NULL);
    m_bCreate = TRUE;

    m_cmbType.SetCurSel(OVL_TYPE_TRANS);
  }
  else
  {
    GetDlgItem(IDC_NAME)->EnableWindow(FALSE);
    m_strName = m_pEvent->GetName();
    m_bEnabled = m_pEvent->IsEnabled();
    m_strBitmap = m_pEvent->GetBitmap();
    m_colTransparent = m_pEvent->GetTransparentColor();
    UpdateData(FALSE);

    m_cmbType.SetCurSel(m_pEvent->GetOverlayType());
  }

  OnKillFocusBitmap();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionBitmapDlg::OnKillFocusBitmap()
{
  TRACE("[CCaptionBitmapDlg]OnKillFocusBitmap()\n");

  CString strTmp;
  GetDlgItem(IDC_BITMAP)->GetWindowText(strTmp);
  BOOL bEnable = !strTmp.IsEmpty();

  GetDlgItem(IDC_BMP_TYPE)->EnableWindow(bEnable);
  GetDlgItem(IDC_POSITION)->EnableWindow(bEnable);

  OnSelectionChangedBmpType();
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionBitmapDlg::OnBrowse()
{
  TRACE("[CCaptionBitmapDlg]OnBrowse()\n");

  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
     "Bitmap files (*.bmp)|*.bmp|"
     "All files (*.*)|*.*|"
     "|");

  if (dlg.DoModal() != IDOK)
    return;

  GetDlgItem(IDC_BITMAP)->SetWindowText(dlg.m_ofn.lpstrFile);
  OnKillFocusBitmap();
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionBitmapDlg::OnOK()
{
  TRACE("[CCaptionBitmapDlg]OnOK()\n");

  if (!UpdateData())
    return;

  m_pEvent->SetName(m_strName);
  m_pEvent->SetEnabled(m_bEnabled);
  m_pEvent->SetBitmap(m_strBitmap);
  m_pEvent->SetOverlayType(m_cmbType.GetCurSel());
  m_pEvent->SetTransparentColor(m_colTransparent);

  CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionBitmapDlg::OnCancel()
{
  if (m_bCreate)
    delete m_pEvent;

  CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionBitmapDlg::OnPosition()
{
  CCaptionPosDlg dlg;
  dlg.SetEvent(m_pEvent);
  dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionBitmapDlg::OnSelectionChangedBmpType()
{
  CString strTmp;
  GetDlgItem(IDC_BITMAP)->GetWindowText(strTmp);
  BOOL bEnable = (m_cmbType.GetCurSel() == OVL_TYPE_TRANS && !strTmp.IsEmpty());
  GetDlgItem(IDC_TRANS_SELECT)->EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionBitmapDlg::OnTransparentSelect()
{
  CString strTmp;
  GetDlgItem(IDC_BITMAP)->GetWindowText(strTmp);

  CColorSelectDlg dlg;
  dlg.SetBitmapFile(strTmp);
  dlg.SetColor(m_colTransparent);
  if (dlg.DoModal() == IDOK)
    m_colTransparent = dlg.GetColor();
}
