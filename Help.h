/////////////////////////////////////////////////////////////////////////////
//
// Help.h
//   The definition of the CHelp class, a HTML Help helper class
//
// Copyright / Usage Details:
//
//   Written and Copyright by Frank Fesevur, 2002
//
//   You are allowed to include the source code in any product
//   (commercial, shareware, freeware or otherwise) when your product
//   is released in binary form. You are allowed to modify the source
//   code in any way you want except you cannot modify the copyright
//   details at the top of each module. If you want to distribute source
//   code with your application, then you are only allowed to distribute
//   versions released by the author. This is to maintain a single
//   distribution point for the source code.
//
// Version History:
//    16-Oct-2002: Created
//

#ifndef __HELP_H__
#define __HELP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHelp dialog

class CHelp : public CObject
{
public:
  CHelp();
  ~CHelp();

  void ShowContents();
  void ShowIndex(LPCSTR =NULL);
  void ShowSearch();
  void ShowTopic(UINT);
  void ShowTopic(LPCSTR);
  void ShowKeyword(LPCSTR);
  void ShowContextHelp(CWnd*, POINT, UINT, BOOL = FALSE);
  void ShowContextHelp(CWnd*, POINT, LPCSTR);

  void SetAutoClose(BOOL b = TRUE)   { m_bAutoClose = b; };
  void SetWindow(HWND h)             { m_hWnd = h;       };
  void SetPopups(LPCSTR sz)          { m_strPopups = sz; };

private:
  CString m_strPath;
  HWND    m_hWnd;
  BOOL    m_bAutoClose;
  DWORD   m_dwCookie;
  CString m_strPopups;

};

#endif // __HELP_H__
