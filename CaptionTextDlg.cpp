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
// CaptionTextDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "CaptionTextDlg.h"

#include "CaptionEvent.h"
#include "CaptionText.h"
#include "CaptionPosDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaptionTextDlg dialog

CCaptionTextDlg::CCaptionTextDlg(CWnd* pParent /*=NULL*/)
  : CHelpDialog(CCaptionTextDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCaptionTextDlg)
  m_strName = _T("");
  m_bTransparent = FALSE;
  m_bEnabled = TRUE;
  m_strCaption = _T("");
  m_strFile = _T("");
	m_uMaxLength = 0;
	//}}AFX_DATA_INIT

  m_pMngr = NULL;
  m_pEvent = NULL;
  m_bCreate = FALSE;
  m_colBack = DEFAULT_BACK_COLOR;
  m_colFore = DEFAULT_FORE_COLOR;
  m_plfFont = new LOGFONT;
  ASSERT(m_plfFont != NULL);
  ZeroMemory(m_plfFont, sizeof(LOGFONT));

  m_help.SetPopups("PopupsCaptionText.txt");
}

CCaptionTextDlg::~CCaptionTextDlg()
{
  delete [] m_plfFont;
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionTextDlg::DoDataExchange(CDataExchange* pDX)
{
  CHelpDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCaptionTextDlg)
  DDX_Control(pDX, IDC_SHADOW, m_cmbShadow);
  DDX_Control(pDX, IDC_CAP_LANG, m_cmbLang);
  DDX_Text(pDX, IDC_NAME, m_strName);
  DDX_Check(pDX, IDC_TRANSPARENT, m_bTransparent);
  DDX_Check(pDX, IDC_ENABLE, m_bEnabled);
  DDX_Text(pDX, IDC_CAPTION, m_strCaption);
  DDX_Text(pDX, IDC_FILE, m_strFile);
	DDX_Text(pDX, IDC_LENGTH, m_uMaxLength);
	//}}AFX_DATA_MAP

  DDV_MinChars(pDX, m_strName, 1);

  // Check if caption OR filename is filled (not both)
  if (pDX->m_bSaveAndValidate)
  {
    if (!(m_strCaption.IsEmpty() ^ m_strFile.IsEmpty()))
    {
      MsgBoxEx("Fill the text OR the filename!");
      pDX->Fail();
    }
  }

  if (m_bCreate)
    DDV_CaptionEventUnique(pDX, m_strName, m_pMngr);
}

BEGIN_MESSAGE_MAP(CCaptionTextDlg, CHelpDialog)
  //{{AFX_MSG_MAP(CCaptionTextDlg)
  ON_BN_CLICKED(IDC_BACK_COLOR, OnBackColor)
  ON_BN_CLICKED(IDC_FONT, OnFont)
  ON_BN_CLICKED(IDC_FORE_COLOR, OnForeColor)
  ON_BN_CLICKED(IDC_TRANSPARENT, OnTransparent)
  ON_BN_CLICKED(IDC_POSITION, OnPosition)
  ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
  ON_EN_CHANGE(IDC_FILE, OnChangeFile)
  ON_EN_CHANGE(IDC_CAPTION, OnChangeCaption)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionTextDlg::OnInitDialog()
{
  CHelpDialog::OnInitDialog();

  ASSERT(m_pMngr != NULL);

  // Initialize the shadow combo
  m_cmbShadow.AddString("None");
  m_cmbShadow.AddString("Full");
  m_cmbShadow.AddString("Drop");

  if (m_pEvent == NULL)
  {
    m_pEvent = new CCaptionText;
    ASSERT(m_pEvent != NULL);
    m_bCreate = TRUE;

    m_cmbShadow.SetCurSel(SHADOW_NONE);
  }
  else
  {
    GetDlgItem(IDC_NAME)->EnableWindow(FALSE);
    m_strName = m_pEvent->GetName();
    m_bEnabled = m_pEvent->IsEnabled();
    m_strCaption = m_pEvent->GetCaption();
    m_bTransparent = m_pEvent->GetTranparent();
    m_strFile = m_pEvent->GetFilename();
    m_uMaxLength = m_pEvent->GetMaxLength();
    UpdateData(FALSE);

    m_colBack = m_pEvent->GetBackColor();
    m_colFore = m_pEvent->GetForeColor();

    if (m_pEvent->GetFont() != NULL)
      CopyMemory(m_plfFont, m_pEvent->GetFont(), sizeof(LOGFONT));

    m_cmbShadow.SetCurSel(m_pEvent->GetShadow());
  }
  OnTransparent();
  OnChangeCaption();
  OnChangeFile();

  // Fill the language selection
  WORD wLang = LANG_NEUTRAL;
  if (m_pEvent != NULL)
    wLang = m_pEvent->GetLang();

  char szStr[_MAX_PATH];
  GetLocaleInfo(MAKELCID(wLang, SORT_DEFAULT), LOCALE_SENGLANGUAGE, szStr, _MAX_PATH);
  CString strLangCurrent = szStr;
  for (wLang = 0x01; wLang < 0xff; wLang++)
  {
    if (GetLocaleInfo(MAKELCID(wLang, SORT_DEFAULT), LOCALE_SENGLANGUAGE, szStr, _MAX_PATH) != 0)
    {
      int iIndex = m_cmbLang.AddString(szStr);
      ASSERT(iIndex != CB_ERR);
      m_cmbLang.SetItemData(iIndex, wLang);
      if (strLangCurrent == szStr)
        m_cmbLang.SetCurSel(iIndex);
    }
  }

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Open a color selector dialog and use it for the back color of the text

void CCaptionTextDlg::OnBackColor()
{
  CColorDialog dlg(m_colBack);
  if (dlg.DoModal() == IDOK)
    m_colBack = dlg.GetColor();
}

/////////////////////////////////////////////////////////////////////////////
// Open a color selector dialog and use it for the fore color of the text

void CCaptionTextDlg::OnForeColor()
{
  CColorDialog dlg(m_colFore);
  if (dlg.DoModal() == IDOK)
    m_colFore = dlg.GetColor();
}

/////////////////////////////////////////////////////////////////////////////
// Open a font selector dialog

void CCaptionTextDlg::OnFont()
{
  CFontDialog dlg(m_plfFont);
  if (dlg.DoModal() == IDOK)
    CopyMemory(m_plfFont, dlg.m_cf.lpLogFont, sizeof(LOGFONT));
}

/////////////////////////////////////////////////////////////////////////////
// Save the data

void CCaptionTextDlg::OnOK()
{
  TRACE("[CCaptionTextDlg]OnOK()\n");

  if (!UpdateData())
    return;

  m_pEvent->SetName(m_strName);
  m_pEvent->SetEnabled(m_bEnabled);
  m_pEvent->SetCaption(m_strCaption);
  m_pEvent->SetFilename(m_strFile);
  m_pEvent->SetBackColor(m_colBack);
  m_pEvent->SetForeColor(m_colFore);
  m_pEvent->SetFont(m_plfFont);
  m_pEvent->SetTranparent(m_bTransparent);
  m_pEvent->SetShadow(m_cmbShadow.GetCurSel());
  m_pEvent->SetMaxLength(m_uMaxLength);

  int iIndex = m_cmbLang.GetCurSel();
  if (iIndex != CB_ERR)
    m_pEvent->SetLang((WORD) m_cmbLang.GetItemData(iIndex));

  CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// Close the dialog and clean up the mess

void CCaptionTextDlg::OnCancel()
{
  if (m_bCreate)
    delete m_pEvent;

  CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// Enable/disable the shadow combo

void CCaptionTextDlg::OnTransparent()
{
  BOOL bTransparent = IsDlgButtonChecked(IDC_TRANSPARENT);
  GetDlgItem(IDC_SHADOW)->EnableWindow(bTransparent);
}

/////////////////////////////////////////////////////////////////////////////
// Open the position dialog

void CCaptionTextDlg::OnPosition()
{
  CCaptionPosDlg dlg;
  dlg.SetEvent(m_pEvent);
  dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// Open the FileOpen dialog to specify the text file

void CCaptionTextDlg::OnBrowse()
{
  TRACE("[CCaptionTextDlg]OnBrowse()\n");

  CFileDialog dlg(TRUE, "*.txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
     "Text files (*.txt)|*.txt|"
     "All files (*.*)|*.*|"
     "|");

  if (dlg.DoModal() == IDOK)
    GetDlgItem(IDC_FILE)->SetWindowText(dlg.m_ofn.lpstrFile);
}

/////////////////////////////////////////////////////////////////////////////
// Enable/disable the caption and language fields

void CCaptionTextDlg::OnChangeFile()
{
  TRACE("[CCaptionTextDlg]OnChangeFile()\n");

  BOOL bAan = (GetDlgItem(IDC_FILE)->GetWindowTextLength() == 0);
  GetDlgItem(IDC_CAPTION)->EnableWindow(bAan);
  GetDlgItem(IDC_CAP_LANG)->EnableWindow(bAan);
}

/////////////////////////////////////////////////////////////////////////////
// Enable/disable the file field and browse button

void CCaptionTextDlg::OnChangeCaption()
{
  TRACE("[CCaptionTextDlg]OnChangeCaption()\n");

  BOOL bAan = (GetDlgItem(IDC_CAPTION)->GetWindowTextLength() == 0);
  GetDlgItem(IDC_FILE)->EnableWindow(bAan);
  GetDlgItem(IDC_BROWSE)->EnableWindow(bAan);
}
