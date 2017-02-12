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
// CaptionSettingsDlg.h : header file

#ifndef __CAPTIONSETTINGSDLG_H__
#define __CAPTIONSETTINGSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCaptionManager;

/////////////////////////////////////////////////////////////////////////////
// CCaptionSettingsDlg dialog

class CCaptionSettingsDlg : public CDialog
{
// Construction
public:
  CCaptionSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CCaptionSettingsDlg)
  enum { IDD = IDD_CAPTION_SETTINGS };
  CListBox  m_lbCaptions;
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCaptionSettingsDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CCaptionSettingsDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnAdd();
  afx_msg void OnChange();
  afx_msg void OnDelete();
  afx_msg void OnDoubleClickCaption();
  afx_msg void OnSelChangeCaption();
  afx_msg void OnMoveUp();
  afx_msg void OnMoveDown();
	afx_msg void OnClose();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

public:
  void SetCaptionMngr(CCaptionManager* p)  { m_pMngr = p; };

protected:
  void SelectItem(int);

  CCaptionManager* m_pMngr;         // Pointer to current Caption Manager

};

#endif // __CAPTIONSETTINGSDLG_H__
