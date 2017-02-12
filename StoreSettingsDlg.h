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
// StoreSettingsDlg.h : header file

#ifndef __STORESETTINGSDLG_H__
#define __STORESETTINGSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStoreSettingsDlg dialog

class CStoreSettingsDlg : public CDialog
{
// Construction
public:
  CStoreSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CStoreSettingsDlg)
  enum { IDD = IDD_STORE_SETTINGS };
  CListBox  m_lbStore;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CStoreSettingsDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CStoreSettingsDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnSelChangeStore();
  afx_msg void OnAdd();
  afx_msg void OnChange();
  afx_msg void OnDelete();
  afx_msg void OnDoubleClickStore();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnClose();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  void SelectItem(int);

};

#endif // __STORESETTINGSDLG_H__
