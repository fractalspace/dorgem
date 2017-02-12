/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2001-2003 Frank Fesevur                                  //
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
// StoreFileDlg.h : header file

#ifndef __STOREFILEDLG_H__
#define __STOREFILEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HelpDialog.h"

class CStoreFile;

/////////////////////////////////////////////////////////////////////////////
// CStoreFileDlg dialog

class CStoreFileDlg : public CHelpDialog
{
// Construction
public:
  CStoreFileDlg(CWnd* pParent = NULL);   // standard constructor

  CString    GetName()                 { return(m_strName); };
  CStoreFile* GetEvent()               { return(m_pEvent);  };
  void SetEvent(CStoreFile* pEvent)    { m_pEvent = pEvent; };

// Dialog Data
  //{{AFX_DATA(CStoreFileDlg)
	enum { IDD = IDD_STORE_FILE };
  CComboBox m_cmbIntervalType;
  CString m_strFilename;
  DWORD m_dwInterval;
  CString m_strName;
  BOOL  m_bEnable;
  BOOL  m_bResetTime;
  BOOL  m_bCaptionCustom;
	BOOL	m_bCreateDirs;
	//}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CStoreFileDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CStoreFileDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnBrowse();
  virtual void OnOK();
  afx_msg void OnCaptionCustom();
  afx_msg void OnCaptionSettings();
  virtual void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

protected:
  CStoreFile* m_pEvent;
  BOOL        m_bCreate;

  void AddIntervalType(LPCSTR, int, int);

};

#endif // __STOREFILEDLG_H__
