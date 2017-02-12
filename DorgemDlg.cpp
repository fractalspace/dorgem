/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2001-2002 Frank Fesevur                                  //
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
// DorgemDlg.cpp : implementation file

#include "StdAfx.h"
#include "Dorgem.h"
#include "DorgemDlg.h"

#include "AboutDlg.h"
#include "VFWWnd.h"
#include "NTray.h"
#include "PreviewDlg.h"
#include "CaptionSettingsDlg.h"
#include "StoreSettingsDlg.h"
#include "StoreManager.h"
#include "StoreEvent.h"
#include "JpegFile.h"
#include "Tm.h"
#include "CaptionManager.h"
#include "CaptionEvent.h"
#include "WriteAVI.h"
#include "OptionsDlg.h"
#include "SInstance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_TRAYNOTIFY (WM_USER + 100)

#define CAP_ID           100
#define TIMER_ID           1
#define DEFAULT_COMPRESS  75

/////////////////////////////////////////////////////////////////////////////
// CDorgemDlg dialog

CDorgemDlg::CDorgemDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CDorgemDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDorgemDlg)
  m_iCompress = GetApp()->GetProfileInt(szGeneral, szCompress, DEFAULT_COMPRESS);
  m_bCapture = GetApp()->GetProfileInt(szGeneral, szCapture, TRUE);
  //}}AFX_DATA_INIT

  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = GetApp()->LoadIcon(IDR_MAINFRAME);

  // Create the preview window
  m_dlgPreview = new CPreviewDlg;
  ASSERT(m_dlgPreview != NULL);
  m_dlgPreview->Create(IDD_PREVIEW, AfxGetMainWnd());

  // Create the AVI capture window
  m_pwndPreview = new CAVICapWnd;
  ASSERT(m_pwndPreview != NULL);

  // Create the AVI file
  m_pAviFile = new CWriteAVI;
  ASSERT(m_pAviFile != NULL);

  // Create the instance checker
  CString strMutex = GetApp()->m_strConfig;
  if (strMutex.IsEmpty())
    strMutex.LoadString(IDS_PRODUCT_NAME);
  m_pInstanceChecker = new CInstanceChecker(strMutex);
  ASSERT(m_pInstanceChecker != NULL);

  m_pTrayIcon = NULL;
  m_pPrevBits = NULL;
  m_dwPreviewRate = 15;
  m_iDeviceID = -1;
  m_bPreviewShown = FALSE;
  ReadSettings();
}

/////////////////////////////////////////////////////////////////////////////
//

void CDorgemDlg::ReadSettings()
{
  m_bUseMotionDetection = GetApp()->GetProfileInt(szGeneral, szMotionDetection, FALSE);
  m_bReconnect = GetApp()->GetProfileInt(szGeneral, szReconnect, FALSE);
  m_iReconnectTime = GetApp()->GetProfileInt(szGeneral, szReconnectTime, 1);
}

/////////////////////////////////////////////////////////////////////////////
//

CDorgemDlg::~CDorgemDlg()
{
  // These are created in the constructor
  delete m_pwndPreview;
  delete m_dlgPreview;
  delete m_pAviFile;
  delete m_pInstanceChecker;

  // These are created somewhere else
  if (m_pTrayIcon != NULL)
    delete m_pTrayIcon;

  if (m_pPrevBits != NULL)
    delete [] m_pPrevBits;
}

/////////////////////////////////////////////////////////////////////////////
//

void CDorgemDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDorgemDlg)
  DDX_Control(pDX, IDC_COMPRESS_SPIN, m_spinCompress);
  DDX_Control(pDX, IDC_DEVICES, m_cmbDevices);
  DDX_Text(pDX, IDC_COMPRESS, m_iCompress);
  DDV_MinMaxUInt(pDX, m_iCompress, 1, 100);
  DDX_Check(pDX, IDC_CAPTURE, m_bCapture);
  //}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//

BEGIN_MESSAGE_MAP(CDorgemDlg, CDialog)
  //{{AFX_MSG_MAP(CDorgemDlg)
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
  ON_CBN_SELCHANGE(IDC_DEVICES, OnSelChangeDevices)
  ON_BN_CLICKED(IDC_CAPTURE_NOW, OnCaptureNow)
  ON_BN_CLICKED(IDC_DEV_DISPLAY, OnDevDisplay)
  ON_BN_CLICKED(IDC_DEV_FORMAT, OnDevFormat)
  ON_BN_CLICKED(IDC_DEV_SOURCE, OnDevSource)
  ON_WM_DESTROY()
  ON_BN_CLICKED(ID_HELP, OnHelp)
  ON_WM_SIZE()
  ON_WM_TIMER()
  ON_BN_CLICKED(IDM_ABOUTBOX, OnAboutbox)
  ON_WM_ACTIVATE()
  ON_BN_CLICKED(IDC_CAPTION_SETTINGS, OnCaptionSettings)
  ON_BN_CLICKED(IDC_STORE_SETTINGS, OnStoreSettings)
  ON_EN_CHANGE(IDC_COMPRESS, OnChangeCompress)
  ON_COMMAND(IDC_RESTORE, OnRestore)
  ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
  ON_WM_SHOWWINDOW()
  ON_BN_CLICKED(IDC_CAPTURE, OnCapture)
  //}}AFX_MSG_MAP
  ON_MESSAGE(WM_TRAYNOTIFY, OnTrayNotification)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

void CDorgemDlg::OnDestroy()
{
  // Disconnect the preview window
  if (m_pwndPreview->GetSafeHwnd())
  {
    m_pwndPreview->Disconnect();
    m_pwndPreview->DestroyWindow();
  }

  // Destroy the preview window
  m_dlgPreview->DestroyWindow();

  WINDOWPLACEMENT wndpl;
  wndpl.length = sizeof(WINDOWPLACEMENT);

  // Gets current window position
  if (GetWindowPlacement(&wndpl))
  {
    CString strText;
    strText.Format("%d,%d", wndpl.rcNormalPosition.left, wndpl.rcNormalPosition.top);
    GetApp()->WriteProfileString(szGeneral, szMainWndPos, strText);
  }

  // Store the state of the preview button
  CButton* but = (CButton*) GetDlgItem(IDC_PREVIEW);
  GetApp()->WriteProfileInt(szGeneral, szPreview, but->GetCheck());

  // Remove the TrayIcon
  m_pTrayIcon->RemoveIcon();

  CDialog::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CDorgemDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Do some handling of the SystemMenu. Since it is a dialog,
  // the framework doesn't handle it.
  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
  {
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    pSysMenu->EnableMenuItem(SC_SIZE, MF_BYCOMMAND | MF_GRAYED);
    pSysMenu->EnableMenuItem(SC_MAXIMIZE, MF_BYCOMMAND | MF_GRAYED);
    pSysMenu->EnableMenuItem(SC_RESTORE, MF_BYCOMMAND | MF_GRAYED);
  }

  // Adjust the window caption
  CString strCaption;
  strCaption.LoadString(IDS_PRODUCT_NAME);
  if (!GetApp()->m_strConfig.IsEmpty())
  {
    strCaption += " (";
    strCaption += GetApp()->m_strConfig;
    strCaption += ")";
  }
  SetWindowText(strCaption);

  // Set the icon for this dialog. The framework does this automatically
  // when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);       // Set big icon

  // First try to load the 16*16 icon. If this fails, then use the 32*32
  HICON hIcon = (HICON) ::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0);
  if (hIcon == NULL)
    SetIcon(m_hIcon, FALSE);    // Set small icon
  else
    SetIcon(hIcon, FALSE);      // Set small icon

  // Track this instance so any other instances can find us.
  m_pInstanceChecker->TrackFirstInstanceRunning();

  // Configure the spin control
  m_spinCompress.SetBuddy(GetDlgItem(IDC_COMPRESS));
  m_spinCompress.SetRange(1, 100);

  // Get saved position
  CString strPos = GetApp()->GetProfileString(szGeneral, szMainWndPos);
  int iIndex = strPos.Find(',');
  if (iIndex != -1) // Bail out if the string is odd or if no string
  {
    // Get the window size
    CRect place;
    GetWindowRect(place);
    LONG lSizeX = place.right - place.left;
    LONG lSizeY = place.bottom - place.top;

    // Determine the new position
    place.left = atoi(strPos);
    place.right = place.left + lSizeX;
    place.top = atoi(strPos.Mid(iIndex + 1));
    place.bottom = place.top + lSizeY;

    // Move the window
    MoveWindow(place);
  }

  // Create the system tray icon handler
  m_pTrayIcon = new CTrayNotifyIcon;
  ASSERT(m_pTrayIcon != NULL);

  if (!m_pTrayIcon->Create(this, IDR_TRAYPOPUP, strCaption,
                CTrayNotifyIcon::LoadIconResource(IDR_MAINFRAME), WM_TRAYNOTIFY))
  {
    MsgBoxStop("Failed to create tray icon");
    return(-1);
  }
  m_pTrayIcon->HideIcon();

  // Create the AVICap window
  if (!m_pwndPreview->Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 640, 480), m_dlgPreview, CAP_ID))
  {
    MsgBoxStop("Failed to create the preview window");
    return(FALSE);
  }

  // Add the available capture device(s) to the combo
  BOOL bFound = FALSE;
  m_iDeviceID = GetApp()->GetProfileInt(szGeneral, szDevice, -1);
  for (WORD i = 0; i < 10; i++)
  {
    CString strName, strVersion;
    if (CAVICapWnd::GetDescription(i, strName, strVersion))
    {
      CString strText;
      if (strVersion.GetLength() != 0)
        strText.Format("%s\t%s", strName, strVersion);
      else
        strText = strName;
      int iIndex = m_cmbDevices.AddString(strText);
      m_cmbDevices.SetItemData(iIndex, i);
      if (i == m_iDeviceID)
      {
        m_cmbDevices.SetCurSel(iIndex);
        bFound = TRUE;
      }
    }
  }

  if (bFound)
  {
    if (!ConnectDevice())
      return(FALSE);

    // Show the preview window?
    if (GetApp()->GetProfileInt(szGeneral, szPreview, FALSE))
    {
      CButton* but = (CButton*) GetDlgItem(IDC_PREVIEW);
      but->SetCheck(1);
      OnPreview();
    }
  }
  else
  {
    m_iDeviceID = -1;
    ConnectDevice();
  }

  // Set the time
  SetTimer(TIMER_ID, 1000, NULL);

  // Need to start minimized?
  if (GetApp()->m_bStartMinimized)
    PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);

  // Give the status bar a default value
  SetStatusBar();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon. For MFC applications using the document/view model,
// this is automatically done for you by the framework.

void CDorgemDlg::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
    CDialog::OnPaint();
}

/////////////////////////////////////////////////////////////////////////////
// The system calls this to obtain the cursor to display while the user
// drags the minimized window.

HCURSOR CDorgemDlg::OnQueryDragIcon()
{
  return((HCURSOR) m_hIcon);
}

/////////////////////////////////////////////////////////////////////////////
// Do the handling of the SystemMenu. Since it is a dialog, the framework
// doesn't handle it. On a minimize/restore also hide/show the preview wnd
// Also, if the users want to, minimize to the system tray

void CDorgemDlg::OnSize(UINT nType, int cx, int cy)
{
  TRACE("[CDorgemDlg]OnSize(%d,%d,%d)\n", nType, cx, cy);

  // Do we need to minimize to the system tray?
  if (nType == SIZE_MINIMIZED && m_pTrayIcon->IsHidden())
  {
    if (GetApp()->GetProfileInt(szGeneral, szTrayIcon, TRUE))
    {
      if (m_bPreviewShown)
      {
        m_dlgPreview->ShowWindow(SW_HIDE);
        m_pwndPreview->SetPreview(FALSE);
      }
      m_pTrayIcon->ShowIcon();
      ShowWindow(SW_HIDE);
      return;
    }
  }

  // Adjust the system menu and hide/show the preview (if it is shown)
  CMenu* pSysMenu = GetSystemMenu(FALSE);
  switch (nType)
  {
    case SIZE_MINIMIZED:
      pSysMenu->EnableMenuItem(SC_RESTORE, MF_BYCOMMAND | MF_ENABLED);
      pSysMenu->EnableMenuItem(SC_MINIMIZE, MF_BYCOMMAND | MF_GRAYED);
      if (m_bPreviewShown)
      {
        m_dlgPreview->ShowWindow(SW_HIDE);
        m_pwndPreview->SetPreview(FALSE);
      }
      break;

    case SIZE_RESTORED:
      pSysMenu->EnableMenuItem(SC_RESTORE, MF_BYCOMMAND | MF_GRAYED);
      pSysMenu->EnableMenuItem(SC_MINIMIZE, MF_BYCOMMAND | MF_ENABLED);
      if (m_bPreviewShown)
      {
        m_dlgPreview->ShowWindow(SW_SHOWNORMAL);
        m_pwndPreview->SetPreview(TRUE);
      }
      break;
  }

  CDialog::OnSize(nType, cx, cy);
}

/////////////////////////////////////////////////////////////////////////////
// If the window is activated, also active the preview (if enabled)

void CDorgemDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
  if (m_dlgPreview->IsWindowVisible())
  {
    switch (nState)
    {
      case WA_INACTIVE:     // The window is being deactivated.
        m_dlgPreview->SetWindowPos(this, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        break;
      case WA_ACTIVE:       // The window is being activated
      case WA_CLICKACTIVE:
        m_dlgPreview->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        break;
    }
  }

  CDialog::OnActivate(nState, pWndOther, bMinimized);
}

/////////////////////////////////////////////////////////////////////////////
//

void CDorgemDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
  TRACE("[CDorgemDlg]OnShowWindow(%d,%d)\n", bShow, nStatus);

  if (bShow)
  {
    TRACE("MAIN WINDOW NEEDS TO BE SHOWN\n");
  }


  CDialog::OnShowWindow(bShow, nStatus);
}

/////////////////////////////////////////////////////////////////////////////
// Delegate all the work back to the default implementation in
// CTrayNotifyIcon.

LRESULT CDorgemDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
  ASSERT(m_pTrayIcon != NULL);
  return(m_pTrayIcon->OnTrayNotification(wParam, lParam));
}

/////////////////////////////////////////////////////////////////////////////
//

void CDorgemDlg::SetStatusBar(LPCSTR szMsg)
{
  GetDlgItem(IDC_STATUS)->SetWindowText(szMsg == NULL ? "Ready" : szMsg);
}

/////////////////////////////////////////////////////////////////////////////
// Pressing the Preview button

void CDorgemDlg::OnPreview()
{
  TRACE("[CDorgemDlg]OnPreview()\n");

  // Is the preview window visible
  if (::IsWindowVisible(m_dlgPreview->m_hWnd))
  {
    m_dlgPreview->ShowWindow(SW_HIDE);
    m_pwndPreview->SetPreview(FALSE);
    m_bPreviewShown = FALSE;
    return;
  }

  if (m_iDeviceID == -1)
  {
    MsgBoxEx("No device selected!");
    return;
  }

  m_dlgPreview->ShowWindow(SW_SHOWNORMAL);
  m_bPreviewShown = TRUE;
  m_dlgPreview->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

  // If already connected to the device, ignore the remaining
  if (m_pwndPreview->GetSafeHwnd())
  {
    m_pwndPreview->SetPreview(TRUE);
    return;
  }

  ConnectDevice();
}

/////////////////////////////////////////////////////////////////////////////
// A or another device is selected. So connect to that device.

void CDorgemDlg::OnSelChangeDevices()
{
  int iIndex = m_cmbDevices.GetCurSel();
  if (iIndex == CB_ERR)
  {
    m_iDeviceID = -1;
    return;
  }
  m_iDeviceID = m_cmbDevices.GetItemData(iIndex);

  if (ConnectDevice())
  {
    m_iDeviceID = m_cmbDevices.GetItemData(iIndex);
    GetApp()->WriteProfileInt(szGeneral, szDevice, m_iDeviceID);
  }
}

/////////////////////////////////////////////////////////////////////////////
// Put the overlay(s) on the bitmap

void CDorgemDlg::PutOverlays(CCaptionManager* pMngr, HDC hDC, LONG lBmpX, LONG lBmpY)
{
  if (pMngr == NULL)
    return;

  CCaptionEvent* pEvent = (CCaptionEvent*) pMngr->GetFirstEvent();
  while (pEvent != NULL)
  {
    if (pEvent->IsEnabled())
      pEvent->Overlay(hDC, lBmpX, lBmpY);
    pEvent = (CCaptionEvent*) pMngr->GetNextEvent();
  }
}

/////////////////////////////////////////////////////////////////////////////
// Write the buffer to the JPEG file

BOOL CDorgemDlg::WriteJPEG(LPCSTR szFile, BYTE* pBits24, long lWidth, long lHeight)
{
  // Vertical flip
  JpegFile::VertFlipBuf(pBits24, lWidth * 3, lHeight);

  // Reverse BGR
  JpegFile::BGRFromRGB(pBits24, lWidth, lHeight);

  // Write it to JPEG file
  return(JpegFile::RGBToJpegFile(szFile, pBits24, lWidth, lHeight, TRUE, m_iCompress));
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CDorgemDlg::ShowMsgBoxes()
{
  if (!GetApp()->GetProfileInt(szGeneral, szShowMsgBoxes, TRUE))
    return(FALSE);

  return(IsWindowVisible());
}

/////////////////////////////////////////////////////////////////////////////
// Really do the capture. Called from OnCaptureNow() and OnTimer() and the
// web server

BOOL CDorgemDlg::DoCapture(CString strJPGFile, BOOL bWebServer, CCaptionManager* pMngr)
{
  TRACE("[CDorgemDlg]DoCapture(%s,%d)\n", strJPGFile, bWebServer);

  // Do the real capture to a standard .bmp file
  CString strBMPFile = GetApp()->GetTempFileName();
  m_pwndPreview->GrabFrameNoStop();
  if (!m_pwndPreview->SaveDib(strBMPFile))
  {
    if (ShowMsgBoxes())
      MsgBoxEx("Couldn't capture to temporary BMP file!");

    // Try to clean up the temp file (just in case)
    DeleteFile(strBMPFile);
    return(FALSE);
  }

  // Load this capture into memory and delete temporary file
  HBITMAP hBMP = (HBITMAP) LoadImage(NULL, strBMPFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
  if (!hBMP)
  {
    if (ShowMsgBoxes())
      MsgBoxEx("LoadImage failed!");

    // Try to clean up the temp file
    DeleteFile(strBMPFile);
    return(FALSE);
  }
  DeleteFile(strBMPFile);

  // Get the bitmap information
  BITMAP bmp;
  int iRet = GetObject(hBMP, sizeof(BITMAP), &bmp);
  ASSERT(iRet != 0);
  LONG lHeight = bmp.bmHeight;
  LONG lWidth = bmp.bmWidth;

  // To ensure the captured frames can be saved to JPEG, a 24 bits
  // bitmap is created and the captured bitmap is BitBlt to it.

  // Select the original capture into a DC
  HDC hDC = CreateCompatibleDC(NULL);
  HBITMAP hBmpOld = (HBITMAP) SelectObject(hDC, hBMP);

  // Create a DC with the new 24 bits bitmap
  BITMAPINFOHEADER bihBMP24;
  ZeroMemory(&bihBMP24, sizeof(BITMAPINFOHEADER));
  bihBMP24.biSize = sizeof(BITMAPINFOHEADER);
  bihBMP24.biWidth = lWidth;
  bihBMP24.biHeight = lHeight;
  bihBMP24.biPlanes = 1;
  bihBMP24.biBitCount = 24;
  bihBMP24.biCompression = BI_RGB;
  bihBMP24.biSizeImage = (bihBMP24.biWidth * (bihBMP24.biBitCount / 8)) * bihBMP24.biHeight;

  HDC hdc24Bits = CreateCompatibleDC(hDC);
  VOID* pBits24;
  HBITMAP hBMP24 = CreateDIBSection(hdc24Bits, (BITMAPINFO *) &bihBMP24, DIB_RGB_COLORS, &pBits24, NULL, 0);
  HBITMAP hBmp24Old = (HBITMAP) SelectObject(hdc24Bits, hBMP24);

  // Copy the original bitmap to the 24 bits bitmap
  BitBlt(hdc24Bits, 0, 0, lWidth, lHeight, hDC, 0, 0, SRCCOPY);

  // Clean up the original bitmap
  SelectObject(hDC, hBmpOld);
  DeleteObject(hBmpOld);
  DeleteObject(hBMP);
  DeleteDC(hDC);

  // Do we need to create the buffer?
  if (m_pPrevBits == NULL)
  {
    m_pPrevBits = new BYTE[bihBMP24.biSizeImage];
    ASSERT(m_pPrevBits != NULL);
    ZeroMemory(m_pPrevBits, bihBMP24.biSizeImage);
  }

  // Flush the GDI
  GdiFlush();

  // When it is called from the web server, the handling is different (more easy)!
  BOOL bRet = FALSE;
  if (bWebServer)
  {
    // Put the overlay(s) on the bitmap?
    PutOverlays(pMngr, hdc24Bits, lWidth, lHeight);

    // Write it to the JPEG file
    bRet = WriteJPEG(strJPGFile, (BYTE*) pBits24, lWidth, lHeight);
  }
  else
  {
    // Is there any changed since the last capture?
    if (MotionAnalysis(&bihBMP24, (BYTE*) pBits24, m_pPrevBits))
    {
      TRACE("[CDorgemDlg]DoCapture: Motion has been detected or motion detection is disabled\n");
      CopyMemory(m_pPrevBits, pBits24, bihBMP24.biSizeImage);

      // Update the last motion
      m_tmLastMotion.GetSystemTime();

      // Put the overlay(s) on the bitmap?
      PutOverlays(pMngr, hdc24Bits, lWidth, lHeight);

      // Archive in an .AVI file
      m_pAviFile->AddFrame(&bihBMP24, pBits24);

      // Write it to the JPEG file
      bRet = WriteJPEG(strJPGFile, (BYTE*) pBits24, lWidth, lHeight);
      if (!bRet)
      {
        if (ShowMsgBoxes())
          MsgBoxEx("Error writing the temporary JPEG file!");
      }
    }
    else
    {
      if (m_bReconnect)
      {
        // Workaround for bogus (Labtec) driver - reconnect to the driver
        // when the image freezes

        CTM tmNow;
        TRACE("No motion detected for %d minutes\n", tmNow.Between(m_tmLastMotion, BETWEEN_MINS));
        if (tmNow.Between(m_tmLastMotion, BETWEEN_MINS) >= m_iReconnectTime)
        {
          TRACE("[CDorgemDlg]DoCapture: No change detected for %d minutes, reconnect device\n", m_iReconnectTime);
          SetStatusBar("Reconnecting to device");
          m_pwndPreview->Disconnect();
          m_pwndPreview->Connect(m_iDeviceID);
          m_tmLastMotion.GetSystemTime();
          SetStatusBar();
        }
      }
    }
  }

  // Clean up the 24-bits bitmap
  SelectObject(hdc24Bits, hBmp24Old);
  DeleteObject(hBmp24Old);
  DeleteObject(hBMP24);
  DeleteDC(hdc24Bits);

  return(bRet);
}

/////////////////////////////////////////////////////////////////////////////
// The Capture Now button is pressed

void CDorgemDlg::OnCaptureNow()
{
  // Are we already connected to a device?
  if (!m_pwndPreview->GetSafeHwnd())
    return;

  // Go through all the events
  CWaitCursor wait;
  CStoreManager* pMngr = GetStoreMngr();
  CStoreEvent* pEvent = (CStoreEvent*) pMngr->GetFirstEvent();
  while (pEvent != NULL)
  {
    // Is this event enabled?
    if (pEvent->IsEnabled())
    {
      // Capture to a temporary file...
      CString strJPGFile = GetApp()->GetTempFileName();
      if (DoCapture(strJPGFile, FALSE, pEvent->GetCaptions()))
      {
        // and store it
        pEvent->SetOrigFile(strJPGFile);
        pEvent->Store();
      }
      DeleteFile(strJPGFile);
    }
    pEvent = (CStoreEvent*) pMngr->GetNextEvent();
  }
}

/////////////////////////////////////////////////////////////////////////////
// Adjust the size of the entire dialog to match the size of the preview

void CDorgemDlg::ResizePreview(UINT uiHeight, UINT uiWidth)
{
  // Get the current position
  CRect rect;
  m_dlgPreview->GetWindowRect(rect);

  // Calculate the new size and resize it
  int iBorderX = GetSystemMetrics(SM_CXFRAME);
  int iBorderY = GetSystemMetrics(SM_CYFRAME);
  int iCaptionY = GetSystemMetrics(SM_CYSMCAPTION);
  rect.bottom = rect.top + uiHeight + (2 * iBorderY) + iCaptionY;
  rect.right = rect.left + uiWidth + (2 * iBorderX);
  m_dlgPreview->MoveWindow(rect);

  // A new buffer needs te be created!
  if (m_pPrevBits != NULL)
  {
    delete [] m_pPrevBits;
    m_pPrevBits = NULL;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Enable or disable the config buttons

void CDorgemDlg::EnableItems(BOOL bEnable)
{
  if (bEnable)
  {
    GetDlgItem(IDC_COMPRESS)->EnableWindow(TRUE);
    GetDlgItem(IDC_PREVIEW)->EnableWindow(TRUE);
    GetDlgItem(IDC_CAPTURE_NOW)->EnableWindow(TRUE);
    GetDlgItem(IDC_DEV_SOURCE)->EnableWindow(m_pwndPreview->HasVideoSourceDlg());
    GetDlgItem(IDC_DEV_FORMAT)->EnableWindow(m_pwndPreview->HasVideoFormatDlg());
    GetDlgItem(IDC_DEV_DISPLAY)->EnableWindow(m_pwndPreview->HasVideoOutputDlg());
  }
  else
  {
    GetDlgItem(IDC_COMPRESS)->EnableWindow(FALSE);
    GetDlgItem(IDC_PREVIEW)->EnableWindow(FALSE);
    GetDlgItem(IDC_CAPTURE_NOW)->EnableWindow(FALSE);
    GetDlgItem(IDC_DEV_SOURCE)->EnableWindow(FALSE);
    GetDlgItem(IDC_DEV_FORMAT)->EnableWindow(FALSE);
    GetDlgItem(IDC_DEV_DISPLAY)->EnableWindow(FALSE);
  }
}

/////////////////////////////////////////////////////////////////////////////
// Connect to a the video device

BOOL CDorgemDlg::ConnectDevice()
{
  TRACE("[CDorgemDlg]ConnectDevice()\n");

  if (m_iDeviceID == -1)
  {
    EnableItems(FALSE);
    return(FALSE);
  }

  // Start a wait cursor (it ends upon destruction)
  CWaitCursor wait;

  // Connect to the specified device
  if (!m_pwndPreview->Connect(m_iDeviceID))
  {
    m_pwndPreview->DestroyWindow();
    EnableItems(FALSE);

    // Let the user know
    MsgBoxStop("Failed to connect to device!");
    return(FALSE);
  }

  // Resize the preview window
  CAPSTATUS status;
  m_pwndPreview->GetStatus(status);
  ResizePreview(status.uiImageHeight, status.uiImageWidth);

  // Enable the preview and compression
  EnableItems(TRUE);
  m_pwndPreview->SetPreviewRate((WORD) ((1000.0 / m_dwPreviewRate) + 0.5));
  m_pwndPreview->SetPreview(TRUE);

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// The Display button is pressed

void CDorgemDlg::OnDevDisplay()
{
  m_pwndPreview->ShowVideoOutputDlg();
}

/////////////////////////////////////////////////////////////////////////////
// The Format button is pressed

void CDorgemDlg::OnDevFormat()
{
  TRACE("[CDorgemDlg]OnDevFormat\n");

  // Save the current size of the preview window
  CAPSTATUS CapStatus;
  m_pwndPreview->GetStatus(CapStatus);
  UINT uiOldHeigth = CapStatus.uiImageHeight;
  UINT uiOldWidth = CapStatus.uiImageWidth;

  // Open the format dialog
  if (!m_pwndPreview->ShowVideoFormatDlg())
    return;

  // If the size of the preview is changed, resize the preview dialog
  m_pwndPreview->GetStatus(CapStatus);
  if (uiOldHeigth != CapStatus.uiImageHeight || uiOldWidth != CapStatus.uiImageWidth)
    ResizePreview(CapStatus.uiImageHeight, CapStatus.uiImageWidth);
}

/////////////////////////////////////////////////////////////////////////////
// The Source button is pressed

void CDorgemDlg::OnDevSource()
{
  m_pwndPreview->ShowVideoSourceDlg();
}

/////////////////////////////////////////////////////////////////////////////
// The Help button is pressed

void CDorgemDlg::OnHelp()
{
  TRACE("[CDorgemDlg]OnHelp()\n");
  ::HtmlHelp(NULL, GetApp()->m_pszHelpFilePath, HH_DISPLAY_TOPIC, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// Do the automatic capturing

void CDorgemDlg::OnTimer(UINT nIDEvent)
{
  //TRACE("[CDorgemDlg]OnTimer(%d)\n", nIDEvent);

  if (nIDEvent == TIMER_ID && m_bCapture)
  {
    // Go through the events and see if we need to capture?
    CStoreManager* pMngr = GetStoreMngr();
    CStoreEvent* pEvent = (CStoreEvent*) pMngr->GetFirstEvent();
    while (pEvent != NULL)
    {
      if (pEvent->NeedStore())
      {
        // Captured an image
        TRACE("[CDorgemDlg]OnTimer: '%s' Need Storage\n", pEvent->GetName());
        CString strJPGFile = GetApp()->GetTempFileName();
        if (!DoCapture(strJPGFile, FALSE, pEvent->GetCaptions()))
        {
          pEvent->ResetTimer();
          DeleteFile(strJPGFile);
          break;
        }

        // Store it
        pEvent->SetOrigFile(strJPGFile);
        pEvent->Store();

        // Clean up the mess
        DeleteFile(strJPGFile);
      }
      pEvent = (CStoreEvent*) pMngr->GetNextEvent();
    }
  }

  CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// The About button is pressed

void CDorgemDlg::OnAboutbox()
{
  TRACE("[CDorgemDlg]OnAboutbox()\n");

  CAboutDlg dlg;
  dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// The Capture settings button is pressed

void CDorgemDlg::OnCaptionSettings()
{
  CCaptionSettingsDlg dlg;
  dlg.SetCaptionMngr(GetCaptionMngr());
  dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// The Storage settings button is pressed

void CDorgemDlg::OnStoreSettings()
{
  CStoreSettingsDlg dlg;
  dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// The Compress factor is changed

void CDorgemDlg::OnChangeCompress()
{
  UpdateData();
  GetApp()->WriteProfileInt(szGeneral, szCompress, m_iCompress);
}

/////////////////////////////////////////////////////////////////////////////
//

void CDorgemDlg::OnCapture()
{
  UpdateData();
  GetApp()->WriteProfileInt(szGeneral, szCapture, m_bCapture);
}

/////////////////////////////////////////////////////////////////////////////
// The restore option of the Tray Icon popup menu is selected

void CDorgemDlg::OnRestore()
{
  ShowWindow(SW_SHOWNORMAL);
  if (m_bPreviewShown)
  {
    m_pwndPreview->ShowWindow(SW_SHOWNORMAL);
    m_pwndPreview->SetPreview(TRUE);
  }
  m_pTrayIcon->HideIcon();
}

/////////////////////////////////////////////////////////////////////////////
//

void CDorgemDlg::OnOptions()
{
  COptionsDlg dlg;
  if (dlg.DoModal() != IDOK)
    return;

  m_pAviFile->ReadSettings();
}

/////////////////////////////////////////////////////////////////////////////
// Routine to subtract downsized image from the reference image this will
// give us an image with only the changes showing. It's not quite a simple
// subtraction, we look at each color and look for color changes as well.
// Significantly changed pixels are set to bright white. Result is a b/w
// image with additional blob filtering to look for areas of change
// (marked as red pixels). Each blob is a pixel surrounded by 8 neighbors
//
// An adapted version of the motion detection of 'gspy' a gnome security
// camera application (http://gspy.sourceforge.net), also a GPL application
//
// The function assumes the bitmap is 24 bits RGB
// If the image is larger then 358x288, you are advised to downsize it

BOOL CDorgemDlg::MotionAnalysis(BITMAPINFOHEADER* pbihBMP, BYTE* pSrcBits, BYTE* pRefBits)
{
  if (!m_bUseMotionDetection)
    return(TRUE);

  int iHeight = pbihBMP->biHeight;
  int iWidth = pbihBMP->biWidth;
  int iSize = pbihBMP->biSizeImage;
  BYTE* pResBits = new BYTE[iSize];

  BYTE* pSrc = pSrcBits;    // intensity corrected buffer
  BYTE* pRef = pRefBits;
  BYTE* pRes = pResBits;

  int x, y, red, green, blue;
  BYTE cResult;
  float fResult = 0.0f;
  for (x = 0; x < iWidth; x++)
  {
    for (y = 0; y < iHeight; y++)
    {
      // give the cpu something to do...
      // compute rms diff between all pixels
      // detection of color or intensity changes detected
      red = (*pRef > *pSrc ? *pRef - *pSrc : *pSrc - *pRef);    // red
      pRef++; pSrc++;
      green = (*pRef > *pSrc ? *pRef - *pSrc : *pSrc - *pRef);  // green
      pRef++; pSrc++;
      blue  = (*pRef > *pSrc ? *pRef - *pSrc : *pSrc - *pRef);  // blue
      pRef++; pSrc++;
      cResult = (BYTE) sqrt((double) (red * red + green * green + blue * blue) / 3);
      *pRes++ = cResult;     // make b/w image
      *pRes++ = cResult;
      *pRes++ = cResult;
      fResult += (float) cResult;
    }
  }
  // calculate normalized difference over picture
  fResult = fResult / (float) (iHeight * iWidth);

  // detect and mark pixels that are significantly different
  BYTE cThreshold = 25;
  int iPixCount = 0;
  pSrc = pResBits;
  while (pSrc < pResBits + iSize)
  {
    if (*pSrc > cThreshold)
    {
      iPixCount++;
      *pSrc = 255;        // alarm active areas in white
      *(pSrc + 1) = 255;
      *(pSrc + 2) = 255;
    }
    pSrc += 3;          // only need to look at reds of each pixel
                        // because g and b are set to the same thing
  }

  int iAlarmZoneX1 = iWidth;            // init bounding box of alarm blobs
  int iAlarmZoneX2 = -1;
  int iAlarmZoneY1 = iHeight;
  int iAlarmZoneY2 = -1;

  // To eliminate small changes (wind blown objects) from causing so much
  // havoc, we will try to eliminate false alarms by scanning for blobs
  // A blob is a 3x3 pixel area, all must be 'hot' (white)
  int i, iBlobCount = 0;
  pSrc = pResBits + iWidth * 3;
  while (pSrc < (pResBits + iSize - iWidth * 3))
  {
    for (i = 0; i < 1; i++)
    {
      if (*pSrc < 255) break;                       // central pixel must be lite
      if (*(pSrc - iWidth * 3) < 255) break;        // pixel above must be lite
      if (*(pSrc + iWidth * 3) < 255) break;        // pixel below must be lite
      if (*(pSrc - 3) < 255) break;                 // pixel to left must be lite
      if (*(pSrc + 3) < 255) break;                 // pixel to right must be lite
      if (*(pSrc - iWidth * 3 + 3) < 255) break;    // pixel to NE must be lite
      if (*(pSrc - iWidth * 3 - 3) < 255) break;    // pixel to NW must be lite
      if (*(pSrc + iWidth * 3 + 3) < 255) break;    // pixel to SE must be lite
      if (*(pSrc + iWidth * 3 - 3) < 255) break;    // pixel to SW must be lite

      // mark blob as blob on screen
      *(pSrc + 2) = 0;  // shut off blue
      *(pSrc + 1) = 0;  // shut off green
      iBlobCount++;
      x = ((pSrc - pResBits) % (iWidth * 3)) / 3;
      y = (pSrc - pResBits) / (iWidth * 3);
      if (x < iAlarmZoneX1)        // set first occurance location
        iAlarmZoneX1 = x;
      if (iAlarmZoneX2 < x)        // keep highest occurance
        iAlarmZoneX2 = x;
      if (y < iAlarmZoneY1)        // set first occurance location
        iAlarmZoneY1 = y;
      if (iAlarmZoneY2 < y)        // keep highest occurance
        iAlarmZoneY2 = y;
    }
    pSrc += 3;
  }

  delete [] pResBits;

  return(iBlobCount > 3);
}
