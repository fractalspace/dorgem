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
// StoreSelectDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "StoreSelectDlg.h"

#include "StoreEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStoreSelectDlg dialog

CStoreSelectDlg::CStoreSelectDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CStoreSelectDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CStoreSelectDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT

  m_iType = STORE_NONE;
}

void CStoreSelectDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CStoreSelectDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStoreSelectDlg, CDialog)
  //{{AFX_MSG_MAP(CStoreSelectDlg)
  ON_BN_CLICKED(IDC_SEL_FILE, OnSelectFile)
  ON_BN_CLICKED(IDC_SEL_FTP, OnSelectFTP)
  ON_BN_CLICKED(IDC_SEL_PROGRAM, OnSelectProgram)
	ON_BN_CLICKED(IDC_SEL_HTTP, OnSelectHTTP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

void CStoreSelectDlg::OnSelectFile()
{
  m_iType = STORE_FILE;
  GetDlgItem(IDOK)->EnableWindow();
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreSelectDlg::OnSelectFTP()
{
  m_iType = STORE_FTP;
  GetDlgItem(IDOK)->EnableWindow();
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreSelectDlg::OnSelectHTTP() 
{
  m_iType = STORE_HTTP;
  GetDlgItem(IDOK)->EnableWindow();
}

/////////////////////////////////////////////////////////////////////////////
//

void CStoreSelectDlg::OnSelectProgram()
{
  m_iType = STORE_PROGRAM;
  GetDlgItem(IDOK)->EnableWindow();
}
