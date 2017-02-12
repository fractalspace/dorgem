//////////////////////////////////////////////////////////////////////
//
// DunManager.h:
//
//   Interface for CDunManager, a Dial Up Network Manager
//
// Copyright / Usage Details:
//
//   Written and Copyright by Frank Fesevur, 1998-2001
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
//   01-Jun-2001: First public release
//

#ifndef __DUNMANAGER_H__
#define __DUNMANAGER_H__

#ifndef _RAS_H_
#include <ras.h>        // Remote Access API
#endif

#ifndef _RASERROR_H_
#include <raserror.h>   // RAS Error handling
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDunManager : public CObject
{
public:
  CDunManager();
  virtual ~CDunManager();

  BOOL HasRas()           { return(m_bHasRas); };
  BOOL UseRas()           { return(m_bUseRas); };
  BOOL AnyConnection()    { return(m_bAnyConnection); };
  CString Connection()    { return(m_strConnection); };

  BOOL IsConnected();
  CString GetFirstEntry();
  CString GetNextEntry();

  void SetUseRas(BOOL b)  { m_bUseRas = (m_bHasRas ? b : FALSE); };
  BOOL SetConnection(LPCSTR =NULL);

protected:
  BOOL ReallyConnected(RASCONN*);

  BOOL      m_bHasRas;            // Has the OS RAS installed?
  BOOL      m_bUseRas;            // Use RAS?
  CString   m_strConnection;      // Check for connection with...
  BOOL      m_bAnyConnection;     // Any connection will do?
  HINSTANCE m_hRasDLL;            // Handle to the DLL

  CStringArray m_strEntries;
  int          m_iEntryCount;

};

#endif // __DUNMANAGER_H__
