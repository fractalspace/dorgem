/////////////////////////////////////////////////////////////////////////////
//
// RegKey.cpp:
//
//   Implementation of CRegKey, an abstraction of a Registry Key
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

#include "StdAfx.h"
#include "RegKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

void CRegKey::InitVariables()
{
  m_hKey = NULL;
  m_dwSubKeyCount = 0;
  m_dwSubKeys = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Default constructor and destructor.

CRegKey::CRegKey()
{
  InitVariables();
}

CRegKey::CRegKey(HKEY hKey, LPCSTR szSubKeyName)
{
  InitVariables();

  if (szSubKeyName == NULL)
    m_hKey = hKey;
  else
    Open(hKey, szSubKeyName);
}

CRegKey::~CRegKey()
{
  Close();
}

/////////////////////////////////////////////////////////////////////////////
// CRegKey diagnostics

#ifdef _DEBUG
void CRegKey::AssertValid() const
{
  CObject::AssertValid();
}

void CRegKey::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Open the specified registry key

BOOL CRegKey::Open(HKEY hKey, LPCSTR szSubKeyName)
{
  ASSERT(m_hKey == NULL);
  ASSERT(szSubKeyName != NULL);

  if ((RegOpenKeyEx(hKey, szSubKeyName, 0,
                         KEY_ALL_ACCESS, &m_hKey)) != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]Open: Could not open key '%s'\n", szSubKeyName);
    return(FALSE);
  }
  else
    return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Create (or open) the specified registry key

BOOL CRegKey::Create(HKEY hKey, LPCSTR szSubKeyName)
{
  ASSERT(m_hKey == NULL);
  ASSERT(szSubKeyName != NULL);

  DWORD dwDisp;

  long lResult = RegCreateKeyEx(hKey, szSubKeyName, 0, NULL, REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisp);

  if (lResult != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]Create: Could not create key '%s' (error %ld)\n",
                            szSubKeyName, lResult);
    return(FALSE);
  }
  else
    return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Close the registry key

BOOL CRegKey::Close()
{
  if (m_hKey != NULL)
  {
    RegCloseKey(m_hKey);
    m_hKey = NULL;
  }
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Generic routine to get a value. Used by Get????Value

int CRegKey::GetValue(LPCSTR szName, LPDWORD pdwType,
                                        LPBYTE pBuffer, LPDWORD pdwBufferSize)
{
  ASSERT(m_hKey != NULL);
  ASSERT(szName != NULL);

  return(RegQueryValueEx(m_hKey, szName, 0, pdwType, pBuffer, pdwBufferSize));
}

/////////////////////////////////////////////////////////////////////////////
// Generic routine to set a value. Used by Set????Value

int CRegKey::SetValue(LPCSTR szName, DWORD dwType,
                                        LPBYTE pBuffer, DWORD dwBufferSize)
{
  ASSERT(m_hKey != NULL);
  ASSERT(szName != NULL);

  return(RegSetValueEx(m_hKey, szName, 0, dwType, pBuffer, dwBufferSize));
}

/////////////////////////////////////////////////////////////////////////////
// Delete a registry value

BOOL CRegKey::DeleteValue(LPCSTR szName)
{
  ASSERT(m_hKey != NULL);
  ASSERT(szName != NULL);

  int iResult = RegDeleteValue(m_hKey, szName);
  if (iResult != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]RemoveValue: Error %d while removing value '%s'\n", iResult, szName);
    return(FALSE);
  }
  else
    return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

CString CRegKey::GetStringValue(LPCSTR szName, LPCSTR szDefault)
{
  ASSERT(szName != NULL);

  DWORD dwBufLen = 0;
  DWORD dwEntryType = 0;
  CString strRet = szDefault;

  // First check to see if we are actually getting a string, and how
  // much space we need. Return default string on error.
  int iResult = GetValue(szName, &dwEntryType, NULL, &dwBufLen);
  if (iResult != ERROR_SUCCESS || dwBufLen <= 1 ||
                   (dwEntryType != REG_SZ && dwEntryType != REG_EXPAND_SZ))
  {
    TRACE("[CRegKey]GetStringValue: Returning default string '%s'\n", strRet);
    return(strRet);
  }

  // Ok, we need dwBufLen bytes.
  char* szBuf = new char[dwBufLen + 1]; // Just in case
  if (szBuf == NULL)
    return(strRet);

  // Get value
  iResult = GetValue(szName, &dwEntryType, (LPBYTE) szBuf, &dwBufLen);
  if (iResult != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]GetStringValue: Returning default string '%s'\n", strRet);
    return(strRet);
  }

  // Save string and delete buffer
  strRet = szBuf;
  delete [] szBuf;

  return(strRet);
}

/////////////////////////////////////////////////////////////////////////////
//

DWORD CRegKey::GetDWORDValue(LPCSTR szName, DWORD dwDefault)
{
  ASSERT(szName != NULL);

  DWORD dwRet = dwDefault;
  DWORD dwBufLen = 0;
  DWORD dwEntryType = 0;

  // First check to see if we are actually getting a DWORD. We may also
  // read a string, in which case we have to convert it to a DWORD
  int iResult = GetValue(szName, &dwEntryType, NULL, &dwBufLen);
  if (iResult != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]GetDWORDValue: Can't get the EntryType\n");
    return(dwDefault);
  }

  // If we the value is stored as a string, use GetStringValue
  if (dwEntryType == REG_SZ)
  {
    TRACE("[CRegKey]GetDWORDValue: Getting DWORD from String value '%s'\n", szName);
    CString strDefault;
    strDefault.Format("%ld", dwDefault);
    CString strDWORDVal = GetStringValue(szName, strDefault);
    dwRet = atol(strDWORDVal);
    return(dwRet);
  }

  // If we don't have a DWORD, things are a little odd.
  if (dwEntryType != REG_DWORD && dwEntryType != REG_DWORD_LITTLE_ENDIAN)
  {
    TRACE("[CRegKey]GetDWORDValue: Value '%s' has a wrong EntryType.\n", szName);
    return(dwRet);
  }

  // Ok, let's retrieve the value
  DWORD dwBuf;
  dwBufLen = sizeof(dwBuf);
  iResult = GetValue(szName, &dwEntryType, (LPBYTE) &dwBuf, &dwBufLen);
  if (iResult == ERROR_SUCCESS && dwBufLen == sizeof(dwBuf))
    dwRet = dwBuf;

  return(dwRet);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CRegKey::GetBinaryValue(LPCSTR szName, LPBYTE* ppData, UINT* pBytes)
{
  ASSERT(szName != NULL);
  ASSERT(ppData != NULL);
  ASSERT(pBytes != NULL);
  *ppData = NULL;
  *pBytes = 0;

  DWORD dwType, dwCount;
  LONG lResult = GetValue(szName, &dwType, NULL, &dwCount);
  *pBytes = dwCount;
  if (lResult != ERROR_SUCCESS || dwType != REG_BINARY || dwCount <= 1)
  {
    TRACE("[CRegKey]GetBinaryValue: Can't get the EntryType\n");
    return(FALSE);
  }

  *ppData = new BYTE[*pBytes];
  lResult = GetValue(szName, &dwType, *ppData, &dwCount);
  if (lResult == ERROR_SUCCESS)
    return(TRUE);
  delete [] *ppData;
  *ppData = NULL;
  return(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CRegKey::SetDWORDValue(LPCSTR szName, DWORD dwValue)
{
  ASSERT(szName != NULL);

  int iResult = SetValue(szName, REG_DWORD, (LPBYTE) &dwValue, sizeof(DWORD));
  if (iResult != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]SetDWORDValue: Error %d while writing value '%s'\n", iResult, szName);
    return(FALSE);
  }
  else
    return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CRegKey::SetStringValue(LPCSTR szName, LPCSTR szValue)
{
  ASSERT(szName != NULL);
  ASSERT(szValue != NULL);

  DWORD dwLen = strlen(szValue) + 1;
  int iResult = SetValue(szName, REG_SZ, (LPBYTE) szValue, dwLen);
  if (iResult != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]SetStringValue: Error %d while writing value '%s'\n", iResult, szName);
    return(FALSE);
  }
  else
    return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CRegKey::SetBinaryValue(LPCSTR szName, LPBYTE pData, UINT nBytes)
{
  ASSERT(szName != NULL);
  ASSERT(pData != NULL);

  int iResult = SetValue(szName, REG_BINARY, (LPBYTE) pData, nBytes);
  if (iResult != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]SetStringValue: Error %d while writing value '%s'\n", iResult, szName);
    return(FALSE);
  }
  else
    return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Find the first item of the subtree

BOOL CRegKey::FindFirstSubKey(CString& strName)
{
  m_dwSubKeyCount = 0;

  // Values
  DWORD dwValues;             // Number of value entries
  DWORD dwMaxValueName;       // Longest value name length
  DWORD dwMaxValueData;       // Longest value data length
  DWORD dwSecurityDescriptor; // Security descriptor length
  FILETIME ftLastWriteTime;   // Last write time

  // Allocate for class string of the key.
  // Although we can expect this to be REG_SZ, you never know.
  DWORD dwClassLen = 7; // Size of REG_SZ

  LONG lResult;
  lResult = RegQueryInfoKey(m_hKey, m_strClass.GetBuffer(dwClassLen),
                            &dwClassLen, NULL, &m_dwSubKeys, &m_dwMaxSubkey,
                            &m_dwMaxClass, &dwValues, &dwMaxValueName,
                            &dwMaxValueData, &dwSecurityDescriptor,
                            &ftLastWriteTime);

  // If we got ERROR_MORE_DATA, we need more space for the class string.
  // dwClassLen now contains the correct size, so let's try one more time.
  if (lResult == ERROR_MORE_DATA)
  {
    TRACE("[CRegKey]FindFirstSubKey: Buffer for class string to small, should be %ld\n", dwClassLen);

    lResult = RegQueryInfoKey(m_hKey, m_strClass.GetBuffer(dwClassLen),
                              &dwClassLen, NULL, &m_dwSubKeys, &m_dwMaxSubkey,
                              &m_dwMaxClass, &dwValues, &dwMaxValueName,
                              &dwMaxValueData, &dwSecurityDescriptor,
                              &ftLastWriteTime);
  }

  m_strClass.ReleaseBuffer();

  // Bail out if we failed this time.
  if (lResult != ERROR_SUCCESS)
    return(FALSE);

  // Now we know how much subkeys to expect and how much space to allocate
  // for a data buffer to receive the key-strings and key-class-strings
  TRACE("[CRegKey]FindFirstSubKey: Number of subkeys: %ld\n", m_dwSubKeys);

  return(FindNextSubKey(strName));
}

/////////////////////////////////////////////////////////////////////////////
// Find the next item of the subtree

BOOL CRegKey::FindNextSubKey(CString& strName)
{
  if (m_dwSubKeyCount >= m_dwSubKeys)
    return(FALSE);

  // Go through the keys.
  DWORD dwClassLen;
  DWORD dwName;
  HKEY hKey;
  LONG lResult;
  FILETIME ftLastWriteTime;

  // Reset the size of the buffers.
  dwClassLen = m_dwMaxClass + 1;
  dwName = m_dwMaxSubkey + 1;

  // Get Keyname
  lResult = RegEnumKeyEx(m_hKey, m_dwSubKeyCount, strName.GetBuffer(dwName),
                        &dwName, NULL, m_strClass.GetBuffer(dwClassLen),
                        &dwClassLen, &ftLastWriteTime);

  m_strClass.ReleaseBuffer();
  strName.ReleaseBuffer();

  if (lResult != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]FindNextSubKey: ERROR: RegEnumKeyEx returned %ld\n", lResult);
    return(FALSE);
  }

  TRACE("[CRegKey]FindNextSubKey: Found SubTree %ld = '%s'\n", m_dwSubKeyCount, strName);

  lResult = RegOpenKeyEx(m_hKey, strName, 0, KEY_ALL_ACCESS, &hKey);
  if (lResult != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]FindNextSubKey: ERROR: RegOpenKeyEx returned %ld\n", lResult);
    return(FALSE);
  }

  m_dwSubKeyCount++;

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Delete a registry tree

BOOL CRegKey::DeleteTree()
{
  ASSERT(m_hKey != NULL);

  CString strName;
  LONG lResult;

  // Always use 'FindFirst' since otherwise only half of the entries are
  // deleted. When a key is deleted, all the entries move up by one position
  while (FindFirstSubKey(strName))
  {
    CRegKey reg2;
    reg2.Open(m_hKey, strName);
    if (!reg2.DeleteTree())
      return(FALSE);

    lResult = RegDeleteKey(m_hKey, strName);
    if (lResult != ERROR_SUCCESS)
    {
      TRACE("[CRegKey]DeleteTree: ERROR: RegDeleteKey returned %ld\n", lResult);
      return(FALSE);
    }
  }
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Delete a specific subkey, even if it's not empty
//
BOOL CRegKey::DeleteKey(LPCSTR szSubKeyName)
{
  ASSERT(m_hKey != NULL);

  // First make sure the specified is empty
  CRegKey subKey;
  if (!subKey.Open(m_hKey, szSubKeyName))
    return(FALSE);
  if (!subKey.DeleteTree())
    return(FALSE);
  if (!subKey.Close())
    return(FALSE);

  // Then delete the specified key
  LONG lResult = RegDeleteKey(m_hKey, szSubKeyName);
  if (lResult != ERROR_SUCCESS)
  {
    TRACE("[CRegKey]DeleteKey: ERROR: RegDeleteKey returned %ld\n", lResult);
    return(FALSE);
  }

  return(TRUE);
}
