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
// CaptionSelectDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "CaptionSelectDlg.h"

#include "CaptionEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaptionSelectDlg dialog

CCaptionSelectDlg::CCaptionSelectDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CCaptionSelectDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCaptionSelectDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT

  m_iType = CAPTION_NONE;
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionSelectDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCaptionSelectDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCaptionSelectDlg, CDialog)
  //{{AFX_MSG_MAP(CCaptionSelectDlg)
  ON_BN_CLICKED(IDC_SEL_BMP, OnSelectBitmap)
  ON_BN_CLICKED(IDC_SEL_TXT, OnSelectText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionSelectDlg::OnSelectBitmap()
{
  TRACE("[CCaptionSelectDlg]OnSelectBitmap()\n");
  m_iType = CAPTION_BMP;
  GetDlgItem(IDOK)->EnableWindow();
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionSelectDlg::OnSelectText()
{
  TRACE("[CCaptionSelectDlg]OnSelectText()\n");
  m_iType = CAPTION_TXT;
  GetDlgItem(IDOK)->EnableWindow();
}
