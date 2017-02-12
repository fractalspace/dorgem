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
// CaptionBitmapDlg.h : header file

#ifndef __CAPTIONBITMAPDLG_H__
#define __CAPTIONBITMAPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HelpDialog.h"

class CCaptionBitmap;
class CCaptionManager;

/////////////////////////////////////////////////////////////////////////////
// CCaptionBitmapDlg dialog

class CCaptionBitmapDlg : public CHelpDialog
{
// Construction
public:
  CCaptionBitmapDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CCaptionBitmapDlg)
	enum { IDD = IDD_CAPTION_BMP };
  CComboBox m_cmbType;
  CString m_strBitmap;
  BOOL  m_bEnabled;
  CString m_strName;
	//}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCaptionBitmapDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CCaptionBitmapDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnKillFocusBitmap();
  afx_msg void OnBrowse();
  virtual void OnOK();
	afx_msg void OnPosition();
	virtual void OnCancel();
	afx_msg void OnSelectionChangedBmpType();
	afx_msg void OnTransparentSelect();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

public:
  CCaptionBitmap* GetEvent()        { return(m_pEvent); };
  void SetEvent(CCaptionBitmap* p)  { m_pEvent = p;     };
  void SetMngr(CCaptionManager* p)  { m_pMngr = p;      };

protected:
  CCaptionBitmap*   m_pEvent;           // The current event
  CCaptionManager*  m_pMngr;            // The current Caption Manager
  BOOL              m_bCreate;          // Is the event created by this dialog?
  COLORREF          m_colTransparent;   // The tranparant color

};

#endif // __CAPTIONBITMAPDLG_H__
