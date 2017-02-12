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
// StoreSelectDlg.h : header file

#ifndef __STORESELECTDLG_H__
#define __STORESELECTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStoreSelectDlg dialog

class CStoreSelectDlg : public CDialog
{
// Construction
public:
  CStoreSelectDlg(CWnd* pParent = NULL);   // standard constructor

  int GetType()   { return(m_iType); };

// Dialog Data
  //{{AFX_DATA(CStoreSelectDlg)
  enum { IDD = IDD_STORE_SELECT };
    // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CStoreSelectDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CStoreSelectDlg)
  afx_msg void OnSelectFile();
  afx_msg void OnSelectFTP();
	afx_msg void OnSelectProgram();
	afx_msg void OnSelectHTTP();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

protected:

  int m_iType;

};

#endif // __STORESELECTDLG_H__
