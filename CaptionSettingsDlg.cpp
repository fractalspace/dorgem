/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2001-2002 Frank Fesevur                                  //
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
// CaptionSettingsDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "CaptionSettingsDlg.h"

#include "CaptionManager.h"
#include "CaptionEvent.h"
#include "CaptionBitmap.h"
#include "CaptionText.h"
#include "CaptionSelectDlg.h"
#include "CaptionBitmapDlg.h"
#include "CaptionTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaptionSettingsDlg dialog

CCaptionSettingsDlg::CCaptionSettingsDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CCaptionSettingsDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCaptionSettingsDlg)
  //}}AFX_DATA_INIT

  m_pMngr = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCaptionSettingsDlg)
  DDX_Control(pDX, IDC_CAPTION, m_lbCaptions);
  //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//

BEGIN_MESSAGE_MAP(CCaptionSettingsDlg, CDialog)
  //{{AFX_MSG_MAP(CCaptionSettingsDlg)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_CHANGE, OnChange)
  ON_BN_CLICKED(IDC_DELETE, OnDelete)
  ON_LBN_DBLCLK(IDC_CAPTION, OnDoubleClickCaption)
  ON_LBN_SELCHANGE(IDC_CAPTION, OnSelChangeCaption)
  ON_BN_CLICKED(IDC_MOVE_UP, OnMoveUp)
  ON_BN_CLICKED(IDC_MOVE_DOWN, OnMoveDown)
  ON_BN_CLICKED(IDCANCEL, OnClose)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Initialize the dialog

BOOL CCaptionSettingsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  ASSERT(m_pMngr != NULL);    // Caption manager not set

  CCaptionEvent* pEvent = (CCaptionEvent*) m_pMngr->GetFirstEvent();
  int iIndex;
  while (pEvent != NULL)
  {
    iIndex = m_lbCaptions.AddString(pEvent->GetLBName());
    if (iIndex != LB_ERR)
      m_lbCaptions.SetItemDataPtr(iIndex, pEvent);
    pEvent = (CCaptionEvent*) m_pMngr->GetNextEvent();
  }

  // Update the buttons
  OnSelChangeCaption();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionSettingsDlg::OnClose()
{
  CCaptionEvent* pEvent;
  int iCount = m_lbCaptions.GetCount();
  for (int i = 0; i < iCount; i++)
  {
    pEvent = (CCaptionEvent*) m_lbCaptions.GetItemDataPtr(i);
    pEvent->SetOrder(i + 1);
  }
  if (m_pMngr->GetAutoSave())
    m_pMngr->WriteEvents();

  CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionSettingsDlg::OnAdd()
{
  TRACE("[CCaptionSettingsDlg]OnAdd()\n");

  CCaptionEvent* pEvent = NULL;
  CCaptionSelectDlg dlg;
  if (dlg.DoModal() == IDOK)
  {
    switch (dlg.GetType())
    {
      case CAPTION_BMP:
      {
        CCaptionBitmapDlg dlg;
        dlg.SetMngr(m_pMngr);
        if (dlg.DoModal() == IDOK)
          pEvent = dlg.GetEvent();
        break;
      }
      case CAPTION_TXT:
      {
        CCaptionTextDlg dlg;
        dlg.SetMngr(m_pMngr);
        if (dlg.DoModal() == IDOK)
          pEvent = dlg.GetEvent();
        break;
      }
    }
    if (pEvent != NULL)
    {
      CString str = pEvent->GetLBName();
      int iIndex = m_lbCaptions.AddString(str);
      if (iIndex != LB_ERR)
      {
        m_lbCaptions.SetItemDataPtr(iIndex, pEvent);
        m_pMngr->AddEvent(pEvent);
        SelectItem(iIndex);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionSettingsDlg::OnChange()
{
  TRACE("[CCaptionSettingsDlg]OnChange()\n");

  int iIndex = m_lbCaptions.GetCurSel();
  if (iIndex == LB_ERR)  // Shouldn't happen, just in case
    return;

  CCaptionEvent* pEvent = (CCaptionEvent*) m_lbCaptions.GetItemDataPtr(iIndex);
  ASSERT(pEvent != NULL);

  switch (pEvent->GetType())
  {
    case CAPTION_BMP:
    {
      CCaptionBitmapDlg dlg;
      dlg.SetEvent((CCaptionBitmap*) pEvent);
      dlg.SetMngr(m_pMngr);
      dlg.DoModal();
      break;
    }
    case CAPTION_TXT:
    {
      CCaptionTextDlg dlg;
      dlg.SetEvent((CCaptionText*) pEvent);
      dlg.SetMngr(m_pMngr);
      dlg.DoModal();
      break;
    }
  }

  // Refresh the listbox strings
  int iCount = m_lbCaptions.GetCount();
  iIndex = m_lbCaptions.GetCurSel();
  for (int i = 0; i < iCount; i++)
  {
    pEvent = (CCaptionEvent*) m_lbCaptions.GetItemDataPtr(i);
    m_lbCaptions.DeleteString(i);
    m_lbCaptions.InsertString(i, pEvent->GetLBName());
    m_lbCaptions.SetItemDataPtr(i, pEvent);
  }
  SelectItem(iIndex);
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionSettingsDlg::OnDelete()
{
  TRACE("[CStoreSettingsDlg]OnDelete()\n");

  int iIndex = m_lbCaptions.GetCurSel();
  ASSERT (iIndex != LB_ERR);

  if (MsgBoxYesNo("Delete this entry?"))
  {
    // Delete from EventManager
    CCaptionEvent* pEvent = (CCaptionEvent*) m_lbCaptions.GetItemData(iIndex);
    m_pMngr->Remove(pEvent->GetName());

    // Delete from ListBox
    m_lbCaptions.DeleteString(iIndex);
  }
  OnSelChangeCaption();
}

/////////////////////////////////////////////////////////////////////////////
// Double click on the list box == Change button

void CCaptionSettingsDlg::OnDoubleClickCaption()
{
  OnChange();
}

/////////////////////////////////////////////////////////////////////////////
// If the selection of the listbox is changed, update the buttons

void CCaptionSettingsDlg::OnSelChangeCaption()
{
  TRACE("[CCaptionSettingsDlg]OnSelChangeStore()\n");

  int iIndex = m_lbCaptions.GetCurSel();
  GetDlgItem(IDC_CHANGE)->EnableWindow(iIndex != LB_ERR);
  GetDlgItem(IDC_DELETE)->EnableWindow(iIndex != LB_ERR);
  GetDlgItem(IDC_MOVE_UP)->EnableWindow(iIndex != LB_ERR && iIndex != 0);
  GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(iIndex != LB_ERR && iIndex != m_lbCaptions.GetCount() - 1);
}

/////////////////////////////////////////////////////////////////////////////
// Move the selected caption one line up

void CCaptionSettingsDlg::OnMoveUp()
{
  int iIndex = m_lbCaptions.GetCurSel();
  ASSERT(iIndex != LB_ERR);
  ASSERT(iIndex != 0);

  CCaptionEvent* pEvent = (CCaptionEvent*) m_lbCaptions.GetItemData(iIndex);
  ASSERT(pEvent != NULL);
  m_lbCaptions.DeleteString(iIndex);

  iIndex = m_lbCaptions.InsertString(iIndex - 1, pEvent->GetLBName());
  ASSERT(iIndex != LB_ERR);
  m_lbCaptions.SetItemDataPtr(iIndex, pEvent);

  SelectItem(iIndex);
}

/////////////////////////////////////////////////////////////////////////////
// Move the selected caption one line down

void CCaptionSettingsDlg::OnMoveDown()
{
  int iIndex = m_lbCaptions.GetCurSel();
  ASSERT(iIndex != LB_ERR);
  ASSERT(iIndex != m_lbCaptions.GetCount() - 1);

  CCaptionEvent* pEvent = (CCaptionEvent*) m_lbCaptions.GetItemData(iIndex);
  ASSERT(pEvent != NULL);
  m_lbCaptions.DeleteString(iIndex);

  iIndex = m_lbCaptions.InsertString(iIndex + 1, pEvent->GetLBName());
  ASSERT(iIndex != LB_ERR);
  m_lbCaptions.SetItemDataPtr(iIndex, pEvent);

  SelectItem(iIndex);
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionSettingsDlg::SelectItem(int iItem)
{
  ASSERT(iItem != LB_ERR);
  ASSERT(iItem < m_lbCaptions.GetCount());

  m_lbCaptions.SetCurSel(iItem);
  GetDlgItem(IDC_CAPTION)->SetFocus();
  OnSelChangeCaption();
}
