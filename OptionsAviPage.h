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
// OptionsAviPage.h : header file

#ifndef __OPTIONSAVIPAGE_H__
#define __OPTIONSAVIPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HelpPropertyPage.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsAviPage dialog

class COptionsAviPage : public CHelpPropertyPage
{
  DECLARE_DYNCREATE(COptionsAviPage)

// Construction
public:
  COptionsAviPage();
  ~COptionsAviPage();

// Dialog Data
  //{{AFX_DATA(COptionsAviPage)
  enum { IDD = IDD_OPTIONS_AVI };
  CSpinButtonCtrl m_spinFramesPerSecond;
  CString m_strFilename;
  UINT  m_uFramesPerSecond;
  BOOL  m_bChangeDay;
  BOOL  m_bUseAVI;
  //}}AFX_DATA

// Overrides
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(COptionsAviPage)
  public:
  virtual void OnOK();
  virtual BOOL OnKillActive();
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(COptionsAviPage)
  virtual BOOL OnInitDialog();
  afx_msg void OnUseAvi();
  afx_msg void OnBrowse();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

};

#endif // __OPTIONSAVIPAGE_H__
