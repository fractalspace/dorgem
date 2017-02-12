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
// ColorSelectDlg.h : header file

#ifndef __COLORSELECTDLG_H__
#define __COLORSELECTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColorSelectDlg dialog

#include "ColorStatic.h"

class CColorSelectDlg : public CDialog
{
// Construction
public:
  CColorSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
  //{{AFX_DATA(CColorSelectDlg)
  enum { IDD = IDD_COLOR_SELECT };
    // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA
  CColorStatic m_stCurTrans;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CColorSelectDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CColorSelectDlg)
  virtual BOOL OnInitDialog();
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnDestroy();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

public:
  void SetBitmapFile(LPCSTR sz)   { m_strBitmapFile = sz;   };
  void SetColor(COLORREF c)       { m_colSelected = c;      };
  COLORREF GetColor()             { return(m_colSelected);  };

protected:
  CBitmap   m_bmpPicture;       //
  CString   m_strBitmapFile;    //
  COLORREF  m_colSelected;      //
  HCURSOR   m_hCursor;          //
  int       m_iOffsetX;         //
  int       m_iOffsetY;         //
  long      m_lBmpHeight;       //
  long      m_lBmpWidth;        //

  BOOL CursorAboveBitmap(CPoint&);

};

#endif // __COLORSELECTDLG_H__
