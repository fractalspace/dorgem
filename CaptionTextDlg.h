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
// CaptionTextDlg.h : header file

#ifndef __CAPTIONTEXTDLG_H__
#define __CAPTIONTEXTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HelpDialog.h"

class CCaptionText;
class CCaptionManager;

/////////////////////////////////////////////////////////////////////////////
// CCaptionTextDlg dialog

class CCaptionTextDlg : public CHelpDialog
{
// Construction
public:
  CCaptionTextDlg(CWnd* pParent = NULL);   // standard constructor
  ~CCaptionTextDlg();

// Dialog Data
  //{{AFX_DATA(CCaptionTextDlg)
	enum { IDD = IDD_CAPTION_TXT };
	CComboBox	m_cmbShadow;
  CComboBox m_cmbLang;
  CString m_strName;
  BOOL  m_bTransparent;
  BOOL  m_bEnabled;
  CString m_strCaption;
	CString	m_strFile;
	UINT	m_uMaxLength;
	//}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCaptionTextDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

public:
  CCaptionText* GetEvent()          { return(m_pEvent); };
  void SetEvent(CCaptionText* p)    { m_pEvent = p;     };
  void SetMngr(CCaptionManager* p)  { m_pMngr = p;      };

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CCaptionTextDlg)
  afx_msg void OnBackColor();
  afx_msg void OnFont();
  afx_msg void OnForeColor();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnTransparent();
	afx_msg void OnPosition();
	virtual void OnCancel();
	afx_msg void OnBrowse();
	afx_msg void OnChangeFile();
	afx_msg void OnChangeCaption();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  CCaptionText*     m_pEvent;     // The current event
  CCaptionManager*  m_pMngr;      // The current Caption Manager
  COLORREF          m_colBack;    // Background color
  COLORREF          m_colFore;    // Foreground color
  LOGFONT*          m_plfFont;    // The selected font
  BOOL              m_bCreate;    // Is the event created by this dialog?

};

#endif // __CAPTIONTEXTDLG_H__
