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
// HelpPropertyPage.h : header file

#ifndef __HELPPROPERTYPAGE_H__
#define __HELPPROPERTYPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Help.h"

/////////////////////////////////////////////////////////////////////////////
// CHelpPropertyPage dialog

class CHelpPropertyPage : public CPropertyPage
{
  DECLARE_DYNCREATE(CHelpPropertyPage)

// Construction
public:
  CHelpPropertyPage();
  CHelpPropertyPage(UINT);

// Dialog Data
  //{{AFX_DATA(CHelpPropertyPage)
    // NOTE - ClassWizard will add data members here.
    //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_DATA

// Overrides
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CHelpPropertyPage)
  public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CHelpPropertyPage)
  afx_msg void OnHelp();
  afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

protected:
  UINT  m_uID;
  CHelp m_help;

};

#endif // __HELPPROPERTYPAGE_H__
