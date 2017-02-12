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
// StoreHTTPDlg.h : header file

#ifndef __STOREHTTPDLG_H__
#define __STOREHTTPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HelpDialog.h"

class CStoreHTTP;

/////////////////////////////////////////////////////////////////////////////
// CStoreHTTPDlg dialog

class CStoreHTTPDlg : public CHelpDialog
{
// Construction
public:
  CStoreHTTPDlg(CWnd* pParent = NULL);   // standard constructor

  CString    GetName()              { return(m_strName); };
  CStoreHTTP* GetEvent()            { return(m_pEvent);  };
  void SetEvent(CStoreHTTP* pEvent) { m_pEvent = pEvent; };

// Dialog Data
  //{{AFX_DATA(CStoreHTTPDlg)
  enum { IDD = IDD_STORE_HTTP };
  CComboBox m_cmbIntervalType;
  CComboBox m_cmbRasConn;
  CString m_strName;
  DWORD m_dwInterval;
  BOOL  m_bEnable;
  CString m_strHost;
  CString m_strPassword;
  BOOL  m_bUseRAS;
  BOOL  m_bResetTime;
  CString m_strUser;
  CString m_strFilename;
  BOOL  m_bSecure;
  BOOL  m_bCaptionCustom;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CStoreHTTPDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CStoreHTTPDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnUseRas();
  virtual void OnOK();
  virtual void OnCancel();
  afx_msg void OnCaptionCustom();
  afx_msg void OnCaptionSettings();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

protected:
  CStoreHTTP* m_pEvent;
  BOOL        m_bCreate;

  void AddIntervalType(LPCSTR, int, int);

};

#endif // __STOREHTTPDLG_H__
