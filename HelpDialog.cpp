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
// HelpDialog.cpp : implementation file

#include "StdAfx.h"
#include "HelpDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpDialog dialog

CHelpDialog::CHelpDialog(UINT uID, CWnd* pParent) : CDialog(uID, pParent)
{
  //{{AFX_DATA_INIT(CHelpDialog)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT

  m_uID = uID;
}

void CHelpDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CHelpDialog)
    // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHelpDialog, CDialog)
  //{{AFX_MSG_MAP(CHelpDialog)
  ON_COMMAND(ID_HELP, OnHelp)
  ON_WM_HELPINFO()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// The help button is pressed, show page specific help

void CHelpDialog::OnHelp()
{
  m_help.ShowTopic(m_uID);
}

/////////////////////////////////////////////////////////////////////////////
// Handling of the context help ('?' arrow)

BOOL CHelpDialog::OnHelpInfo(HELPINFO* pHelpInfo)
{
  // Only show the popup if the HelpID property of the control is set.
  if (pHelpInfo->dwContextId > 0)
  {
    m_help.ShowContextHelp(CWnd::FromHandle((HWND)pHelpInfo->hItemHandle),
                           pHelpInfo->MousePos, pHelpInfo->dwContextId, TRUE);
  }
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Handling of pressing F1

BOOL CHelpDialog::PreTranslateMessage(MSG* pMsg)
{
  // Magical mystical MFC F1 Help msg! (found in www.codeproject.com article of Peter Moss)
  if (pMsg->message == 0x4d)
  {
    if (GetKeyState(VK_SHIFT) >= 0)
    {
      // Shift key not down, show page specific help
      OnHelp();
      return(TRUE);
    }
  }
  return(CDialog::PreTranslateMessage(pMsg));
}
