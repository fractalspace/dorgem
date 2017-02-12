//////////////////////////////////////////////////////////////////////
//
// DunManager.cpp:
//
//   Implementation for CDunManager, a Dial Up Network Manager
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

#include "StdAfx.h"
#include "DunManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Pointers to the used functions of the RAS-API
typedef DWORD (APIENTRY *tRasGetConnectStatus) (HRASCONN, LPRASCONNSTATUS);
typedef DWORD (APIENTRY *tRasEnumConnections) (LPRASCONN, LPDWORD, LPDWORD);
typedef DWORD (APIENTRY *tRasEnumEntries) (LPSTR, LPSTR, LPRASENTRYNAME, LPDWORD, LPDWORD);
typedef DWORD (APIENTRY *tRasGetErrorString) (UINT, LPSTR, DWORD);

static tRasGetConnectStatus lpfnRasGetConnectStatus;
static tRasEnumConnections lpfnRasEnumConnections;
static tRasEnumEntries lpfnRasEnumEntries;
static tRasGetErrorString lpfnRasGetErrorString;

//////////////////////////////////////////////////////////////////////
// Construction. Get RAS-calls from DLL. Since RAS doesn't have to be
// installed on the machine load them dynamic

CDunManager::CDunManager()
{
  m_bHasRas = FALSE;
  m_bUseRas = FALSE;
  m_bAnyConnection = FALSE;
  m_iEntryCount = 0;

  // Load the RAS-DLL
  m_hRasDLL = ::LoadLibrary("rasapi32.dll");
  if (m_hRasDLL == NULL)
  {
    TRACE("RAS not installed on the machine\n");
    return;
  }

  lpfnRasGetConnectStatus = (tRasGetConnectStatus) GetProcAddress(m_hRasDLL, "RasGetConnectStatusA");
  lpfnRasEnumConnections = (tRasEnumConnections) GetProcAddress(m_hRasDLL, "RasEnumConnectionsA");
  lpfnRasEnumEntries = (tRasEnumEntries) GetProcAddress(m_hRasDLL, "RasEnumEntriesA");
  lpfnRasGetErrorString = (tRasGetErrorString) GetProcAddress(m_hRasDLL, "RasGetErrorStringA");

  if (lpfnRasGetConnectStatus == NULL || lpfnRasEnumConnections == NULL ||
      lpfnRasEnumEntries == NULL || lpfnRasGetErrorString == NULL)
  {
    TRACE("Incompatible version of RAS????\n");
    ::FreeLibrary(m_hRasDLL);
    lpfnRasGetConnectStatus = NULL;
    lpfnRasEnumConnections = NULL;
    lpfnRasEnumEntries = NULL;
    lpfnRasGetErrorString = NULL;
    m_hRasDLL = NULL;
    return;
  }

  m_bHasRas = TRUE;
}

//////////////////////////////////////////////////////////////////////
// Destruction. Release the RAS DLL

CDunManager::~CDunManager()
{
  if (m_hRasDLL != NULL)
    ::FreeLibrary(m_hRasDLL);
}

//////////////////////////////////////////////////////////////////////
// When an entry is active, doesn't mean it is connected! It can be
// connecting (and so on). So check if it really is connected.

BOOL CDunManager::ReallyConnected(RASCONN* pRasConn)
{
  ASSERT(lpfnRasGetConnectStatus != NULL);

  RASCONNSTATUS rcsStatus;
  rcsStatus.dwSize = sizeof(RASCONNSTATUS);
  lpfnRasGetConnectStatus(pRasConn->hrasconn, &rcsStatus);

  return(rcsStatus.rasconnstate == RASCS_Connected);
}

//////////////////////////////////////////////////////////////////////
// Enumerate connections, check the name and its status
//
// From some MS sample. Still has to be updated to new/delete calls.
//

BOOL CDunManager::IsConnected()
{
  // If not using RAS, we're always connected
  if (!m_bUseRas)
    return(TRUE);

  ASSERT(m_bHasRas);
  ASSERT(lpfnRasEnumConnections != NULL);
  ASSERT(lpfnRasGetErrorString != NULL);

  // Trying to find out if really connected
  BOOL bRet = FALSE;

  // Get the active connections
  DWORD dwBuf = sizeof(RASCONN);
  RASCONN* pRasConn = (RASCONN*) malloc(dwBuf);
  if (pRasConn != NULL)
  {
    DWORD dwConn = 0;
    pRasConn->dwSize = sizeof(RASCONN);
    DWORD dwRet = lpfnRasEnumConnections(pRasConn, &dwBuf, &dwConn);
    if (dwRet == ERROR_BUFFER_TOO_SMALL)  // buffer too small -> realloc
    {
      RASCONN* pTemp = (RASCONN*) realloc(pRasConn, dwBuf);
      if (pTemp != NULL)                  // and try again
      {
        pRasConn = pTemp;
        dwRet = lpfnRasEnumConnections(pRasConn, &dwBuf, &dwConn);
      }
      else
        dwRet = ERROR_NOT_ENOUGH_MEMORY;
    }

    // No errors, so check connections
    if (dwRet == 0)
    {
      for (UINT uIndex = 0; uIndex < dwConn; uIndex++)
      {
        // Must we use a specific connection or not?
        if (m_bAnyConnection || pRasConn[uIndex].szEntryName == m_strConnection)
        {
          if (ReallyConnected(&pRasConn[uIndex]))
          {
            bRet = TRUE;
            break;
          }
        }
      }
    }
    else // other (not ERROR_BUFFER_TOO_SMALL) errors
    {
      char szMessage[256];
      if (lpfnRasGetErrorString(dwRet, szMessage, 256) != 0)
        wsprintf(szMessage, "Undefined RasEnumConnections Error (%ld).", dwRet);
      AfxMessageBox(szMessage, MB_OK | MB_ICONSTOP);
    }

    free(pRasConn);
  }

  return(bRet);
}

//////////////////////////////////////////////////////////////////////
// Get all RAS entries and store them in the 'm_strEntries' array.
// The first entry is returned by this function, the remaining
// entries are returned by GetNextEntry()
//
// From some MS sample. Still has to be updated to new/delete calls.
//

CString CDunManager::GetFirstEntry()
{
  TRACE("[CDunManager]GetFirstEntry()\n");

  m_iEntryCount = 0;
  m_strEntries.RemoveAll();

  // Is RAS installed on the machine?
  if (!m_bHasRas)
    return("");

  ASSERT(lpfnRasEnumEntries != NULL);
  ASSERT(lpfnRasGetErrorString != NULL);

  DWORD dwBuf = sizeof(RASENTRYNAME);
  RASENTRYNAME* pRasEntry = (RASENTRYNAME*) malloc(dwBuf);
  if (pRasEntry != NULL)
  {
    pRasEntry->dwSize = sizeof(RASENTRYNAME);
    DWORD dwEntry;
    DWORD dwRet = lpfnRasEnumEntries(NULL, NULL, pRasEntry, &dwBuf, &dwEntry);
    if (dwRet == ERROR_BUFFER_TOO_SMALL)
    {
      RASENTRYNAME* pTemp = (RASENTRYNAME*) realloc(pRasEntry, dwBuf);
      if (pTemp != NULL)
      {
        pRasEntry = pTemp;
        dwRet = lpfnRasEnumEntries(NULL, NULL, pRasEntry, &dwBuf, &dwEntry);
      }
      else
        dwRet = ERROR_NOT_ENOUGH_MEMORY;
    }

    if (dwRet == 0) // No error
    {
      TRACE("Begin Entry-list\n");
      for (UINT uIndex = 0; uIndex < dwEntry; uIndex++)
      {
        TRACE("Entry: %s\n", pRasEntry[uIndex].szEntryName);
        m_strEntries.Add(pRasEntry[uIndex].szEntryName);
      }
    }
    else // other (not ERROR_BUFFER_TOO_SMALL) errors
    {
      if (dwRet != ERROR_CANNOT_OPEN_PHONEBOOK)
      {
        char szMessage[256];
        if (lpfnRasGetErrorString(dwRet, szMessage, 256) != 0)
          sprintf(szMessage, "Undefined RasEnumEntries Error (%ld).", dwRet);
        AfxMessageBox(szMessage, MB_OK | MB_ICONSTOP);
      }
    }

    free(pRasEntry);
  }

  if (m_strEntries.GetSize() == 0)
    return("");
  return(m_strEntries[m_iEntryCount]);
}

//////////////////////////////////////////////////////////////////////
// Return the next entry in the 'm_strEntries' array

CString CDunManager::GetNextEntry()
{
  TRACE("[CDunManager]GetNextEntry()\n");

  m_iEntryCount++;
  if (m_iEntryCount < m_strEntries.GetSize())
    return(m_strEntries[m_iEntryCount]);
  else
    return("");
}

//////////////////////////////////////////////////////////////////////
// Set the connection the DUN Manager should be aware of

BOOL CDunManager::SetConnection(LPCSTR szConnection)
{
  TRACE("[CDunManager]SetConnection(%s)\n", szConnection);

  // Always reset the old value
  m_strConnection.Empty();

  // NULL is 'Any connection will do'
  if (szConnection == NULL)
  {
    m_bAnyConnection = TRUE;
    return(TRUE);
  }

  // An emtpy string is handled as NULL!
  if (strlen(szConnection) == 0)
  {
    m_bAnyConnection = TRUE;
    return(TRUE);
  }

  m_bAnyConnection = FALSE;

  // Find out if 'szConnection' is defined on machine
  CString str = GetFirstEntry();
  while (!str.IsEmpty())
  {
    if (str == szConnection)
    {
      m_strConnection = szConnection;
      return(TRUE);
    }
    str = GetNextEntry();
  }

  return(FALSE);
}
