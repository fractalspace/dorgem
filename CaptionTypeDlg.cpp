/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2003 Frank Fesevur                                       //
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
// CaptionTypeDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "CaptionTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaptionTypeDlg dialog


CCaptionTypeDlg::CCaptionTypeDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CCaptionTypeDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCaptionTypeDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CCaptionTypeDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCaptionTypeDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCaptionTypeDlg, CDialog)
  //{{AFX_MSG_MAP(CCaptionTypeDlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionTypeDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionTypeDlg::OnOK()
{
  // TODO: Add extra validation here

  CDialog::OnOK();
}
