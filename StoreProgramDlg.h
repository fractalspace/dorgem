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
// StoreProgramDlg.h : header file

#ifndef __STOREPROGRAMDLG_H__
#define __STOREPROGRAMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HelpDialog.h"

class CStoreProgram;

/////////////////////////////////////////////////////////////////////////////
// CStoreProgramDlg dialog

class CStoreProgramDlg : public CHelpDialog
{
// Construction
public:
  CStoreProgramDlg(CWnd* pParent = NULL);   // standard constructor

  CStoreProgram* GetEvent()               { return(m_pEvent);  };
  void SetEvent(CStoreProgram* pEvent)    { m_pEvent = pEvent; };

// Dialog Data
  //{{AFX_DATA(CStoreProgramDlg)
	enum { IDD = IDD_STORE_PROGRAM };
  CComboBox m_cmbIntervalType;
  CString m_strCommand;
  BOOL  m_bEnable;
  DWORD m_dwInterval;
  CString m_strName;
  BOOL  m_bResetTime;
	BOOL	m_bCaptionCustom;
	//}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CStoreProgramDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CStoreProgramDlg)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
	afx_msg void OnCaptionCustom();
	afx_msg void OnCaptionSettings();
	virtual void OnCancel();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

protected:
  CStoreProgram* m_pEvent;
  BOOL           m_bCreate;

  void AddIntervalType(LPCSTR, int, int);

};

#endif // __STOREPROGRAMDLG_H__
