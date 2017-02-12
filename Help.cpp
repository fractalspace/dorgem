/////////////////////////////////////////////////////////////////////////////
// Help.cpp: The implementation of the CHelp class (see Help.h for details)
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

#include "StdAfx.h"
#include "Help.h"

#include <htmlhelp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelp constructor

CHelp::CHelp()
{
  m_hWnd = NULL;
  m_bAutoClose = FALSE;
  m_strPopups = "Popups.txt";

  m_dwCookie = NULL;
  HtmlHelp(NULL, NULL, HH_INITIALIZE, (DWORD) &m_dwCookie);

  // The help file path needs to be set to a .chm file!
  m_strPath = AfxGetApp()->m_pszHelpFilePath;
  if (m_strPath.Right(3).CompareNoCase("hlp") == 0)
  {
    m_strPath = m_strPath.Mid(m_strPath.GetLength() - 3);
    m_strPath += ".chm";
  }
}

/////////////////////////////////////////////////////////////////////////////
// CHelp destructor

CHelp::~CHelp()
{
  if (m_bAutoClose)
    HtmlHelp(NULL, NULL, HH_UNINITIALIZE, (DWORD) m_dwCookie);
}

/////////////////////////////////////////////////////////////////////////////
// Show the table of contents

void CHelp::ShowContents()
{
  HtmlHelp(m_hWnd, m_strPath, HH_DISPLAY_TOC, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// Show the Index.

void CHelp::ShowIndex(LPCSTR szTopic)
{
  HtmlHelp(m_hWnd, m_strPath, HH_DISPLAY_INDEX, (DWORD) szTopic);
}

/////////////////////////////////////////////////////////////////////////////
// Show the Search

void CHelp::ShowSearch()
{
  HH_FTS_QUERY query;
  ZeroMemory(&query, sizeof(HH_FTS_QUERY));
  query.cbStruct = sizeof(HH_FTS_QUERY);
  query.fUniCodeStrings = FALSE;
  query.iProximity = HH_FTS_DEFAULT_PROXIMITY;
  query.fStemmedSearch = FALSE;
  query.fTitleOnly = FALSE;
  query.fExecute = TRUE;
  query.pszWindow = NULL;
  query.pszSearchQuery = "";

  HtmlHelp(m_hWnd, m_strPath, HH_DISPLAY_SEARCH, (DWORD) &query);
}

/////////////////////////////////////////////////////////////////////////////
// Show the Topic, with alias uID. This requires an [ALIAS] and [MAP]
// section in your .hhp

void CHelp::ShowTopic(UINT uID)
{
  HtmlHelp(m_hWnd, m_strPath, HH_HELP_CONTEXT, uID);
}

/////////////////////////////////////////////////////////////////////////////
// Show the Topic 'szURL' (relative URL)

void CHelp::ShowTopic(LPCSTR szURL)
{
  CString strTmp = m_strPath;
  strTmp += "::/";
  strTmp += szURL;

  HtmlHelp(m_hWnd, strTmp, HH_DISPLAY_TOPIC, NULL);
}

/////////////////////////////////////////////////////////////////////////////
//

void CHelp::ShowKeyword(LPCSTR szKeyword)
{
  UNREFERENCED_PARAMETER(szKeyword);
  ASSERT(FALSE);
/*
  // Delete the following line if you want the Topics Found
  // dialog to be displayed.
  HtmlHelp(m_hWnd, m_strPath, HH_DISPLAY_TOPIC, NULL);

  HH_AKLINK link;
  ZeroMemory(&link, sizeof(HH_AKLINK));
  link.cbStruct = sizeof(HH_AKLINK);
  link.pszKeywords = szKeyword;

  if (GetCheckedRadioButton(IDC_SHOW_TOPIC, IDC_SHOW_MSGBOX) == IDC_SHOW_TOPIC)
  {
    CWnd* pWnd = GetDlgItem(IDC_DEFAULT_TOPIC);
    pWnd->GetWindowText(m_DefaultTopic);

    link.pszUrl = m_DefaultTopic;
    link.pszMsgText = NULL;
    link.pszMsgTitle = NULL;
  }
  else
  {
    link.pszUrl = NULL;

    CWnd* pWnd = GetDlgItem(IDC_DEFAULT_TEXT);
    pWnd->GetWindowText(m_DefaultText);

    link.pszMsgText = m_DefaultText;

    CWnd* pWnd2 = GetDlgItem(IDC_DEFAULT_TITLE);
    pWnd2->GetWindowText(m_DefaultTitle);

    link.pszMsgTitle = m_DefaultTitle;
  }

  link.pszWindow = NULL;
  link.fIndexOnFail = FALSE;

  HtmlHelp(m_hWnd, m_strPath, HH_KEYWORD_LOOKUP, (DWORD) &link);
*/
}

/////////////////////////////////////////////////////////////////////////////
// The popup requires that there is a Popups.txt is added to the .hhp!

void CHelp::ShowContextHelp(CWnd* pWnd, POINT pt, UINT uID, BOOL bPopup)
{
  if (bPopup)
  {
    // Initialize structure
    HH_POPUP hPopup;
    ZeroMemory(&hPopup, sizeof(HH_POPUP));
    hPopup.cbStruct = sizeof(HH_POPUP);

    // Make it a standard tooltip
    hPopup.clrBackground = GetSysColor(COLOR_INFOBK);
    hPopup.clrForeground = GetSysColor(COLOR_INFOTEXT);

    // Put it at the right position
    hPopup.rcMargins.left = -1;
    hPopup.rcMargins.right = -1;
    hPopup.rcMargins.bottom = -1;
    hPopup.rcMargins.top = -1;
    hPopup.pt = pt;

    // Set the message
    hPopup.idString = uID;

    // Show the popup
    HtmlHelp(m_hWnd, m_strPath + "::/" + m_strPopups, HH_DISPLAY_TEXT_POPUP, (DWORD) &hPopup);
  }
  else
  {
    CString strTmp;
    if (strTmp.LoadString(uID))
      ShowContextHelp(pWnd, pt, strTmp);
  }
}

/////////////////////////////////////////////////////////////////////////////
//

void CHelp::ShowContextHelp(CWnd* pWnd, POINT pt, LPCSTR szText)
{
  // Initialize structure
  HH_POPUP hPopup;
  ZeroMemory(&hPopup, sizeof(HH_POPUP));
  hPopup.cbStruct = sizeof(HH_POPUP);

  // Make it a standard tooltip
  hPopup.clrBackground = GetSysColor(COLOR_INFOBK);
  hPopup.clrForeground = GetSysColor(COLOR_INFOTEXT);

  // Put it at the right position
  hPopup.rcMargins.left = -1;
  hPopup.rcMargins.right = -1;
  hPopup.rcMargins.bottom = -1;
  hPopup.rcMargins.top = -1;
  hPopup.pt = pt;

  // Set the message
  hPopup.pszText = szText;

  // Show the popup
  HtmlHelp(pWnd->GetSafeHwnd(), NULL, HH_DISPLAY_TEXT_POPUP, (DWORD) &hPopup);
}
