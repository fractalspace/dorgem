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
// OptionsDlg.h : header file

#ifndef __OPTIONSDLG_H__
#define __OPTIONSDLG_H__

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg

#include "OptionsGeneralPage.h"
#include "OptionsAviPage.h"
#include "OptionsWebServerPage.h"

class COptionsDlg : public CPropertySheet
{
  DECLARE_DYNAMIC(COptionsDlg)

// Construction
public:
  COptionsDlg(CWnd* = NULL, UINT = 0);

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COptionsDlg)
  public:
  virtual BOOL OnInitDialog();
  //}}AFX_VIRTUAL

// Implementation
public:
  virtual ~COptionsDlg();

  void MakeTransparent(int);

  // Generated message map functions
protected:
  //{{AFX_MSG(COptionsDlg)
  //}}AFX_MSG
  afx_msg LRESULT OnAppCommand(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

  HICON   m_hIcon;
  BOOL    m_bInitTransparent;     // Do we need to initialize the transparancy?

private:
  COptionsGeneralPage     m_dlgGeneral;
  COptionsAviPage         m_dlgAvi;
  COptionsWebServerPage   m_dlgWebServer;

};

#endif // __OPTIONSDLG_H__
