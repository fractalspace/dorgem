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
// StoreFTPDlg.h : header file

#ifndef __STOREFTPDLG_H__
#define __STOREFTPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HelpDialog.h"

class CStoreFTP;

/////////////////////////////////////////////////////////////////////////////
// CStoreFTPDlg dialog

class CStoreFTPDlg : public CHelpDialog
{
// Construction
public:
  CStoreFTPDlg(CWnd* pParent = NULL);

  CString    GetName()             { return(m_strName); };
  CStoreFTP* GetEvent()            { return(m_pEvent);  };
  void SetEvent(CStoreFTP* pEvent) { m_pEvent = pEvent; };

// Dialog Data
  //{{AFX_DATA(CStoreFTPDlg)
	enum { IDD = IDD_STORE_FTP };
  CComboBox m_cmbIntervalType;
  CComboBox m_cmbRasConn;
  CString m_strFilename;
  CString m_strHost;
  CString m_strName;
  CString m_strPassword;
  CString m_strUser;
  DWORD m_dwInterval;
  BOOL  m_bUseRAS;
  BOOL  m_bEnable;
  BOOL  m_bResetTime;
  BOOL  m_bKeepOpen;
  BOOL  m_bCaptionCustom;
  DWORD m_dwPort;
	BOOL	m_bPassiveFTP;
	//}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CStoreFTPDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CStoreFTPDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnUseRas();
  virtual void OnOK();
  afx_msg void OnCaptionCustom();
  afx_msg void OnCaptionSettings();
  virtual void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

protected:
  CStoreFTP* m_pEvent;
  BOOL       m_bCreate;

  void AddIntervalType(LPCSTR, int, int);

};

#endif // __STOREFTPDLG_H__
