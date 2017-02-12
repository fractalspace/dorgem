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
// ColorSelectDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "ColorSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorSelectDlg dialog

CColorSelectDlg::CColorSelectDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CColorSelectDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CColorSelectDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT

  m_colSelected = RGB(255, 255, 255);
  m_strBitmapFile.Empty();
  m_iOffsetX = 7;
  m_iOffsetY = 7;
}

void CColorSelectDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CColorSelectDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_CUR_TRANS, m_stCurTrans);
}

BEGIN_MESSAGE_MAP(CColorSelectDlg, CDialog)
  //{{AFX_MSG_MAP(CColorSelectDlg)
  ON_WM_SETCURSOR()
  ON_WM_LBUTTONDOWN()
  ON_WM_ERASEBKGND()
  ON_WM_DESTROY()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

BOOL CColorSelectDlg::OnInitDialog()
{
  ASSERT(!m_strBitmapFile.IsEmpty());

  // Do the standard dialog initialisation
  CDialog::OnInitDialog();

  // The current transparent color
	m_stCurTrans.SetBkColor(m_colSelected);

  // Load the bitmap
  HBITMAP hBMP = (HBITMAP) LoadImage(NULL, m_strBitmapFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
  if (!hBMP)
  {
    MsgBoxError("Failed to load bitmap!");
    GetDlgItem(IDC_PICTURE)->ShowWindow(SW_HIDE);
    GetDlgItem(IDOK)->EnableWindow(FALSE);    
    return(TRUE);
  }

  // Load the cursor
  m_hCursor = GetApp()->LoadCursor(IDC_COLOR_SELECT);

  // Get the bitmap information
  BITMAP bmp;
  int iRet = GetObject(hBMP, sizeof(BITMAP), &bmp);
  ASSERT(iRet != 0);
  m_lBmpHeight = bmp.bmHeight;
  m_lBmpWidth = bmp.bmWidth;

  // Attach the loaded bitmap
  m_bmpPicture.Attach(hBMP);

  // Move the frame around the picture to the right place
  CRect rc;
  GetWindowRect(&rc);
  ScreenToClient(&rc);
  CPoint pt = rc.CenterPoint();
  m_iOffsetX = pt.x - (m_lBmpWidth / 2);
  GetDlgItem(IDC_PICTURE)->MoveWindow(m_iOffsetX - 1, m_iOffsetY - 1, m_lBmpWidth + 1, m_lBmpHeight + 1);

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Draw the bitmap on the background

BOOL CColorSelectDlg::OnEraseBkgnd(CDC* pDC)
{
  CDialog::OnEraseBkgnd(pDC);

  if (m_bmpPicture.m_hObject)
  {
    CDC dc;
    dc.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = dc.SelectObject(&m_bmpPicture);

    pDC->BitBlt(m_iOffsetX, m_iOffsetY, m_lBmpWidth, m_lBmpHeight, &dc, 0, 0, SRCCOPY);
    dc.SelectObject(pOldBitmap);
  }

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CColorSelectDlg::CursorAboveBitmap(CPoint& point)
{
  // Get the size and position of the picture
  CRect rct;
  GetDlgItem(IDC_PICTURE)->GetWindowRect(rct);
  rct.DeflateRect(1, 1, 1, 1);

  // Check that the mouse cursor is in the rect of the picture
  VERIFY(GetCursorPos(&point));
  if (rct.PtInRect(point))
  {
    // Convert the coordinate to the bitmap
    ScreenToClient(&point);
    point.Offset(-m_iOffsetX, -m_iOffsetY);
    return(TRUE);
  }

  return(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// Change the cursor if it is above the bitmap

BOOL CColorSelectDlg::OnSetCursor(CWnd* pWnd, UINT uHitTest, UINT uMessage)
{
  if (uHitTest == HTCLIENT)
  {
    CPoint pt;
    if (CursorAboveBitmap(pt))
    {
      ::SetCursor(m_hCursor);
      return(TRUE);
    }
  }

  return(CDialog::OnSetCursor(pWnd, uHitTest, uMessage));
}

/////////////////////////////////////////////////////////////////////////////
// Get the selected color

void CColorSelectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
  CPoint pt;
  if (CursorAboveBitmap(pt))
  {
    // Create the proper DC
    CPaintDC pdc(this);
    CDC dc;
    dc.CreateCompatibleDC(&pdc);

    // Get the pixel from the bitmap
    CBitmap* pOldBitmap = dc.SelectObject(&m_bmpPicture);
    m_colSelected = dc.GetPixel(pt);
    dc.SelectObject(pOldBitmap);
  	m_stCurTrans.SetBkColor(m_colSelected);
  }
  else
    CDialog::OnLButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
//

void CColorSelectDlg::OnDestroy()
{
  m_bmpPicture.Detach();
  CDialog::OnDestroy();
}
