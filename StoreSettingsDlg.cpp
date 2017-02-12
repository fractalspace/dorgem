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
// StoreSettingsDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "StoreSettingsDlg.h"

#include "StoreManager.h"
#include "StoreEvent.h"
#include "StoreFile.h"
#include "StoreFTP.h"
#include "StoreHTTP.h"
#include "StoreProgram.h"
#include "StoreSelectDlg.h"
#include "StoreFileDlg.h"
#include "StoreFTPDlg.h"
#include "StoreHTTPDlg.h"
#include "StoreProgramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStoreSettingsDlg dialog

CStoreSettingsDlg::CStoreSettingsDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CStoreSettingsDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CStoreSettingsDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CStoreSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CStoreSettingsDlg)
  DDX_Control(pDX, IDC_STORE, m_lbStore);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStoreSettingsDlg, CDialog)
  //{{AFX_MSG_MAP(CStoreSettingsDlg)
  ON_LBN_SELCHANGE(IDC_STORE, OnSelChangeStore)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_CHANGE, OnChange)
  ON_BN_CLICKED(IDC_DELETE, OnDelete)
  ON_LBN_DBLCLK(IDC_STORE, OnDoubleClickStore)
  ON_BN_CLICKED(IDC_MOVE_UP, OnMoveUp)
  ON_BN_CLICKED(IDC_MOVE_DOWN, OnMoveDown)
  ON_BN_CLICKED(IDCANCEL, OnClose)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Now all editing of the entries is done directly in the EventMngr.
// This must change! The events should be stored in separate allocated memory

BOOL CStoreSettingsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CStoreManager* pMngr = GetStoreMngr();
  CStoreEvent* pEvent = (CStoreEvent*) pMngr->GetFirstEvent();
  int iIndex;
  CString str;
  while (pEvent != NULL)
  {
    iIndex = m_lbStore.AddString(pEvent->GetLBName());
    if (iIndex != LB_ERR)
      m_lbStore.SetItemDataPtr(iIndex, pEvent);
    pEvent = (CStoreEvent*) pMngr->GetNextEvent();
  }

  // Update the buttons
  OnSelChangeStore();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreSettingsDlg::OnClose()
{
  CStoreEvent* pEvent;
  int iCount = m_lbStore.GetCount();
  for (int i = 0; i < iCount; i++)
  {
    pEvent = (CStoreEvent*) m_lbStore.GetItemDataPtr(i);
    pEvent->SetOrder(i + 1);
  }
  GetStoreMngr()->WriteEvents();

  CDialog::OnCancel();
}


/////////////////////////////////////////////////////////////////////////////
//

void CStoreSettingsDlg::OnSelChangeStore()
{
  TRACE("[CStoreSettingsDlg]OnSelChangeStore()\n");

  int iIndex = m_lbStore.GetCurSel();
  GetDlgItem(IDC_CHANGE)->EnableWindow(iIndex != LB_ERR);
  GetDlgItem(IDC_DELETE)->EnableWindow(iIndex != LB_ERR);
  GetDlgItem(IDC_MOVE_UP)->EnableWindow(iIndex != LB_ERR && iIndex != 0);
  GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(iIndex != LB_ERR && iIndex != m_lbStore.GetCount() - 1);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreSettingsDlg::OnAdd()
{
  TRACE("[CStoreSettingsDlg]OnAdd()\n");

  CStoreEvent* pEvent = NULL;
  CStoreSelectDlg dlg;
  if (dlg.DoModal() == IDOK)
  {
    switch (dlg.GetType())
    {
      case STORE_FILE:
      {
        CStoreFileDlg dlg;
        if (dlg.DoModal() == IDOK)
          pEvent = dlg.GetEvent();
        break;
      }
      case STORE_FTP:
      {
        CStoreFTPDlg dlg;
        if (dlg.DoModal() == IDOK)
          pEvent = dlg.GetEvent();
        break;
      }
      case STORE_PROGRAM:
      {
        CStoreProgramDlg dlg;
        if (dlg.DoModal() == IDOK)
          pEvent = dlg.GetEvent();
        break;
      }
      case STORE_HTTP:
      {
        CStoreHTTPDlg dlg;
        if (dlg.DoModal() == IDOK)
          pEvent = dlg.GetEvent();
        break;
      }
    }
    if (pEvent != NULL)
    {
      int iIndex = m_lbStore.AddString(pEvent->GetLBName());
      if (iIndex != LB_ERR)
      {
        m_lbStore.SetItemDataPtr(iIndex, pEvent);
        GetStoreMngr()->AddEvent(pEvent);
        SelectItem(iIndex);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreSettingsDlg::OnChange()
{
  TRACE("[CStoreSettingsDlg]OnChange()\n");

  int iIndex = m_lbStore.GetCurSel();
  if (iIndex == LB_ERR)  // Shouldn't happen, just in case
    return;

  CStoreEvent* pEvent = (CStoreEvent*) m_lbStore.GetItemDataPtr(iIndex);
  ASSERT(pEvent != NULL);

  switch (pEvent->GetType())
  {
    case STORE_FILE:
    {
      CStoreFileDlg dlg;
      dlg.SetEvent((CStoreFile*) pEvent);
      dlg.DoModal();
      break;
    }
    case STORE_FTP:
    {
      CStoreFTPDlg dlg;
      dlg.SetEvent((CStoreFTP*) pEvent);
      dlg.DoModal();
      break;
    }
    case STORE_PROGRAM:
    {
      CStoreProgramDlg dlg;
      dlg.SetEvent((CStoreProgram*) pEvent);
      dlg.DoModal();
      break;
    }
    case STORE_HTTP:
    {
      CStoreHTTPDlg dlg;
      dlg.SetEvent((CStoreHTTP*) pEvent);
      dlg.DoModal();
      break;
    }
  }

  // Refresh the listbox strings
  int iCount = m_lbStore.GetCount();
  iIndex = m_lbStore.GetCurSel();
  for (int i = 0; i < iCount; i++)
  {
    pEvent = (CStoreEvent*) m_lbStore.GetItemDataPtr(i);
    m_lbStore.DeleteString(i);
    m_lbStore.InsertString(i, pEvent->GetLBName());
    m_lbStore.SetItemDataPtr(i, pEvent);
  }
  m_lbStore.SetCurSel(iIndex);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreSettingsDlg::OnDelete()
{
  TRACE("[CStoreSettingsDlg]OnDelete()\n");

  int iIndex = m_lbStore.GetCurSel();
  if (iIndex == LB_ERR)  // Shouldn't happen, just in case
    return;

  if (MsgBoxYesNo("Delete this entry?"))
  {
    // Delete from EventManager
    CStoreEvent* pEvent = (CStoreEvent*) m_lbStore.GetItemData(iIndex);
    GetStoreMngr()->Remove(pEvent->GetName());

    // Delete from ListBox
    m_lbStore.DeleteString(iIndex);

    // Since an entry has been removed, the list has been changed
    OnSelChangeStore();
  }
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreSettingsDlg::OnDoubleClickStore()
{
  OnChange();
}

/////////////////////////////////////////////////////////////////////////////
// Move the selected event one line up

void CStoreSettingsDlg::OnMoveUp()
{
  int iIndex = m_lbStore.GetCurSel();
  ASSERT(iIndex != LB_ERR);
  ASSERT(iIndex != 0);

  CStoreEvent* pEvent = (CStoreEvent*) m_lbStore.GetItemData(iIndex);
  ASSERT(pEvent != NULL);
  m_lbStore.DeleteString(iIndex);

  iIndex = m_lbStore.InsertString(iIndex - 1, pEvent->GetLBName());
  ASSERT(iIndex != LB_ERR);
  m_lbStore.SetItemDataPtr(iIndex, pEvent);

  SelectItem(iIndex);
}

/////////////////////////////////////////////////////////////////////////////
// Move the selected event one line down

void CStoreSettingsDlg::OnMoveDown()
{
  int iIndex = m_lbStore.GetCurSel();
  ASSERT(iIndex != LB_ERR);
  ASSERT(iIndex != m_lbStore.GetCount() - 1);

  CStoreEvent* pEvent = (CStoreEvent*) m_lbStore.GetItemData(iIndex);
  ASSERT(pEvent != NULL);
  m_lbStore.DeleteString(iIndex);

  iIndex = m_lbStore.InsertString(iIndex + 1, pEvent->GetLBName());
  ASSERT(iIndex != LB_ERR);
  m_lbStore.SetItemDataPtr(iIndex, pEvent);

  SelectItem(iIndex);
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreSettingsDlg::SelectItem(int iItem)
{
  ASSERT(iItem != LB_ERR);
  ASSERT(iItem < m_lbStore.GetCount());

  m_lbStore.SetCurSel(iItem);
  GetDlgItem(IDC_STORE)->SetFocus();
  OnSelChangeStore();
}
