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
// OptionsGeneralPage.h : header file

#ifndef __OPTIONSGENERALPAGE_H__
#define __OPTIONSGENERALPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HelpPropertyPage.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsGeneralPage dialog

class COptionsGeneralPage : public CHelpPropertyPage
{
  DECLARE_DYNCREATE(COptionsGeneralPage)

// Construction
public:
  COptionsGeneralPage();
  ~COptionsGeneralPage();

// Dialog Data
  //{{AFX_DATA(COptionsGeneralPage)
	enum { IDD = IDD_OPTIONS_GENERAL };
  BOOL  m_bTrayIcon;
  BOOL  m_bUseMotionDetection;
	BOOL	m_bReconnect;
	int		m_iReconnectTime;
	BOOL	m_bShowMsgBoxes;
	//}}AFX_DATA

// Overrides
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(COptionsGeneralPage)
  public:
  virtual void OnOK();
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(COptionsGeneralPage)
  virtual BOOL OnInitDialog();
	afx_msg void OnMotionDetection();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

};

#endif // __OPTIONSGENERALPAGE_H__
