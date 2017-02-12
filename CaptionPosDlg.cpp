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
// CaptionPosDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "CaptionPosDlg.h"

#include "CaptionEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaptionPosDlg dialog

CCaptionPosDlg::CCaptionPosDlg(CWnd* pParent /*=NULL*/)
  : CHelpDialog(CCaptionPosDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCaptionPosDlg)
  m_uPosHorAbs = 0;
  m_uPosVerAbs = 0;
  //}}AFX_DATA_INIT

  m_help.SetPopups("PopupsCaptionPos.txt");
}

void CCaptionPosDlg::DoDataExchange(CDataExchange* pDX)
{
  CHelpDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCaptionPosDlg)
  DDX_Control(pDX, IDC_POS_VER_REL, m_cmbPosVerRel);
  DDX_Control(pDX, IDC_POS_HOR_REL, m_cmbPosHorRel);
  DDX_Text(pDX, IDC_POS_HOR_ABS, m_uPosHorAbs);
  DDX_Text(pDX, IDC_POS_VER_ABS, m_uPosVerAbs);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCaptionPosDlg, CHelpDialog)
  //{{AFX_MSG_MAP(CCaptionPosDlg)
  ON_BN_CLICKED(IDC_RELATIVE, OnRelative)
  ON_BN_CLICKED(IDC_ABSOLUTE, OnAbsolute)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionPosDlg::OnInitDialog()
{
  CHelpDialog::OnInitDialog();

  // Initialize the combos
  m_cmbPosVerRel.AddString("Top");
  m_cmbPosVerRel.AddString("Middle");
  m_cmbPosVerRel.AddString("Bottom");

  m_cmbPosHorRel.AddString("Left");
  m_cmbPosHorRel.AddString("Center");
  m_cmbPosHorRel.AddString("Right");

  // Set the radio
  ASSERT(m_pEvent != NULL);
  m_bAbsolute = m_pEvent->GetAbsolute();
  UINT uID = (m_bAbsolute ? IDC_ABSOLUTE : IDC_RELATIVE);    
  CheckRadioButton(uID, uID, uID);

  // Let the dialog match the settings
  if (m_bAbsolute)
  {
    m_cmbPosVerRel.SetCurSel(POS_TOP);
    m_cmbPosHorRel.SetCurSel(POS_LEFT);

    CString strTmp;
    strTmp.Format("%d", m_pEvent->GetPosHor());
    GetDlgItem(IDC_POS_HOR_ABS)->SetWindowText(strTmp);

    strTmp.Format("%d", m_pEvent->GetPosVer());
    GetDlgItem(IDC_POS_VER_ABS)->SetWindowText(strTmp);
  }
  else
  {
    m_cmbPosVerRel.SetCurSel(m_pEvent->GetPosVer());
    m_cmbPosHorRel.SetCurSel(m_pEvent->GetPosHor());
  }
  EnableItems();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionPosDlg::OnOK()
{
  if (!UpdateData())
    return;

  m_pEvent->SetAbsolute(m_bAbsolute);

  if (m_bAbsolute)
  {
    m_pEvent->SetPosHor(m_uPosHorAbs);
    m_pEvent->SetPosVer(m_uPosVerAbs);
  }
  else
  {
    m_pEvent->SetPosHor(m_cmbPosHorRel.GetCurSel());
    m_pEvent->SetPosVer(m_cmbPosVerRel.GetCurSel());
  }

  CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionPosDlg::OnRelative()
{
  m_bAbsolute = FALSE;
  EnableItems();
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionPosDlg::OnAbsolute()
{
  m_bAbsolute = TRUE;
  EnableItems();
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionPosDlg::EnableItems()
{
  GetDlgItem(IDC_POS_VER_REL)->EnableWindow(!m_bAbsolute);
  GetDlgItem(IDC_POS_HOR_REL)->EnableWindow(!m_bAbsolute);
  GetDlgItem(IDC_POS_VER_ABS)->EnableWindow(m_bAbsolute);
  GetDlgItem(IDC_POS_HOR_ABS)->EnableWindow(m_bAbsolute);
}
