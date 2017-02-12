/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2002-2005 Frank Fesevur                                  //
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
// OptionsWebServerPage.h : header file

#ifndef __OPTIONSWEBSERVERPAGE_H__
#define __OPTIONSWEBSERVERPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HelpPropertyPage.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsWebServerPage dialog

class CCaptionManager;

class COptionsWebServerPage : public CHelpPropertyPage
{
  DECLARE_DYNCREATE(COptionsWebServerPage)

// Construction
public:
  COptionsWebServerPage();
  ~COptionsWebServerPage();

// Dialog Data
  //{{AFX_DATA(COptionsWebServerPage)
	enum { IDD = IDD_OPTIONS_WEB_SERVER };
	CComboBox	m_cmbServerType;
	UINT	m_uPort;
	UINT	m_uRefreshRate;
	CString	m_strLogFile;
	BOOL	m_bCaptionCustom;
	//}}AFX_DATA

// Overrides
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(COptionsWebServerPage)
  public:
  virtual void OnOK();
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(COptionsWebServerPage)
  virtual BOOL OnInitDialog();
	afx_msg void OnSelectionChangedServerType();
	afx_msg void SetModified();
	afx_msg void OnCaptionSettings();
	afx_msg void OnCaptionCustom();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

protected:
  void AddServerTypeString(LPCSTR, int, int);

  BOOL              m_bModified;      // Is anything modified (needs the server a restart?)
  CCaptionManager*  m_pCaptions;      //
  CString           m_strCapReg;      // The registry location for the custom captions

};

#endif // __OPTIONSWEBSERVERPAGE_H__
