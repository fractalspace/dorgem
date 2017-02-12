/////////////////////////////////////////////////////////////////////////////
// This file was created on March 28th 2001. By Robert Brault
//
// ColorStatic.h : header file
//

#ifndef __COLORSTATIC_H__
#define __COLORSTATIC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColorStatic window

class CColorStatic : public CStatic
{
// Construction
public:
  void SetTextColor(COLORREF crColor); // This Function is to set the Color for the Text.
  void SetBkColor(COLORREF crColor); // This Function is to set the BackGround Color for the Text.
  CColorStatic();

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CColorStatic)
  //}}AFX_VIRTUAL

  virtual ~CColorStatic();

  // Generated message map functions
protected:
  //{{AFX_MSG(CColorStatic)
  afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

  CBrush   m_brBkgnd;     // Holds Brush Color for the Static Text
  COLORREF m_crBkColor;   // Holds the Background Color for the Text
  COLORREF m_crTextColor; // Holds the Color for the Text

};

#endif // __COLORSTATIC_H__
