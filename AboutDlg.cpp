/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2001-2005 Frank Fesevur                                  //
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
// AboutDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "AboutDlg.h"

#include "Tm.h"
#include "Version.h"

#ifdef _DEBUG
//#define __LOCALHOST_TEST__
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  DDX_Control(pDX, IDC_URL, m_hypURL);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
  ON_BN_CLICKED(IDC_CHECK_VERSION, OnCheckVersion)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Initialize the dialog: put the right version information in it

BOOL CAboutDlg::OnInitDialog()
{
  TRACE("[CAboutDlg]OnInitDialog()\n");

  CDialog::OnInitDialog();

  // Fill with the program name and version
  CString strTmp, strTmp2;
  strTmp.LoadString(IDS_PRODUCT_NAME);
  strTmp += " version ";
  strTmp2.LoadString(IDS_PRODUCT_VERSION);
  strTmp += strTmp2;
#ifdef _DEBUG
  strTmp += " (Debug)";
#endif
  GetDlgItem(IDC_VERSION_STR)->SetWindowText(strTmp);

  strTmp = GetApp()->m_ptmBuildDate->Format("Built on %d %O %Y");
  GetDlgItem(IDC_RELEASE_STR)->SetWindowText(strTmp);

  // Fill with the copyright notice
  strTmp.LoadString(IDS_PRODUCT_COPYRIGHT);
  GetDlgItem(IDC_COPYRIGHT_STR)->SetWindowText(strTmp);

  m_hypURL.SetTooltip("Open Dorgem website");

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CAboutDlg::OnCheckVersion()
{
  // Create a session
  CWaitCursor wait;
  CInternetSession session;

  CString strNewVersion, strNewName, strError;
  try
  {
    // Get the file from the URL
#ifdef __LOCALHOST_TEST__
    CHttpFile* pFile = (CHttpFile*) session.OpenURL("http://localhost/version.txt");
#else
    CHttpFile* pFile = (CHttpFile*) session.OpenURL("http://dorgem.sourceforge.net/version.txt");
#endif

    // If the file is found
    if (pFile != NULL)
    {
      // Only the first line is relevant
      pFile->ReadString(strNewVersion);
      pFile->ReadString(strNewName);
      pFile->Close();
      delete pFile;

      // Just in case...
      strNewVersion.TrimLeft();
      strNewVersion.TrimRight();
      strNewName.TrimLeft();
      strNewName.TrimRight();
    }
  }
  catch (CInternetException* pEx)
  {
    TCHAR szErr[1024];
    pEx->GetErrorMessage(szErr, 1024);
    strError = szErr;
    pEx->Delete();
  }

  // Close the session
  session.Close();
  wait.Restore();

  // Spilt the new version in the seperate numbers
  int iNew[4];
  int iRead = sscanf(strNewVersion, "%d.%d.%d.%d", &iNew[0], &iNew[1], &iNew[2], &iNew[3]);

  // Set the items not read to 0
  for (int i = iRead; i < 4; i++)
    iNew[i] = 0;

  // Check it with the current version number
  CString strMsg;
  int iCurrent[4] = { PRODUCT_VERSION_NUM };
  for (i = 0; i < 4; i++)
  {
    if (iNew[i] > iCurrent[i])
    {
      if (strNewName.IsEmpty())
        strMsg.Format("%s version %s is available!", PRODUCT_NAME, strNewVersion);
      else
        strMsg.Format("%s version %s is available!", PRODUCT_NAME, strNewName);
      break;
    }
    if (iNew[i] < iCurrent[i])
      break;
  }

  // If there is no new version, then what?
  if (strMsg.IsEmpty())
  {
    if (strError.IsEmpty())
      strMsg = "You are running the latest version";
    else
      strMsg.Format("Error while checking for newest version:\n%s", strError);
  }
  MsgBoxEx(strMsg);
}
