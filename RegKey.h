/////////////////////////////////////////////////////////////////////////////
//
// RegKey.h
//
//   Interface for CRegKey, an abstraction of a Registry Key
//
// Copyright / Usage Details:
//
//   Written and Copyright by Frank Fesevur, 1995-2001
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
// Distibution point at release:
//
//   http://home.hetnet.nl/~fesevur/
//
// Version History:
//
//   21-May-2001: First public release
//

#ifndef __REGKEY_H__
#define __REGKEY_H__

#ifndef __AFXWIN_H__
  #error include 'StdAfx.h' before including this file for PCH
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CRegKey : public CObject
{
public:
  CRegKey();
  CRegKey(HKEY, LPCSTR =NULL);
  ~CRegKey();

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  BOOL Create(HKEY, LPCSTR);
  BOOL Open(HKEY, LPCSTR);
  BOOL Close();

  operator HKEY() const  { return(m_hKey); };

  BOOL DeleteKey(LPCSTR);
  BOOL DeleteValue(LPCSTR);

  DWORD   GetDWORDValue(LPCSTR, DWORD =0);
  CString GetStringValue(LPCSTR, LPCSTR =NULL);
  BOOL    GetBinaryValue(LPCSTR, LPBYTE*, UINT*);

  BOOL SetDWORDValue(LPCSTR, DWORD);
  BOOL SetStringValue(LPCSTR, LPCSTR);
  BOOL SetBinaryValue(LPCSTR, LPBYTE, UINT);

  BOOL FindFirstSubKey(CString&);
  BOOL FindNextSubKey(CString&);

  BOOL DeleteTree();

protected:
  HKEY m_hKey;

  int GetValue(LPCSTR, LPDWORD, LPBYTE, LPDWORD);
  int SetValue(LPCSTR, DWORD, LPBYTE, DWORD);
  void InitVariables();

  DWORD   m_dwSubKeyCount;
  DWORD   m_dwSubKeys;
  DWORD   m_dwMaxSubkey;
  DWORD   m_dwMaxClass;
  CString m_strClass;
};

#endif // __REGKEY_H__
