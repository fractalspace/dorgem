/*
Module : NTRAY.CPP
Purpose: implementation for a MFC class to encapsulate Shell_NotifyIcon
Created: PJN / 14-05-1997
History: PJN / 25-11-1997 : Addition of the following
                            1. HideIcon(), ShowIcon() & MoveToExtremeRight 
                            2. Support for animated tray icons
         PJN / 23-06-1998 : Class now supports the new Taskbar Creation Notification 
                            message which comes with IE 4. This allows the tray icon
                            to be recreated whenever the explorer restarts (Crashes!!)
         PJN / 22-07-1998 : 1. Code now compiles cleanly at warning level 4
                            2. Code is now UNICODE enabled + build configurations are 
                            provided
                            3. The documentation for the class has been updated
         PJN / 27-01-1999 : Code first tries to load a 16*16 icon before loading the 32*32
                            version. This removes the blurryness which was previously occuring
         PJN / 28-01-1999 : Fixed a number of level 4 warnings which were occurring.
         PJN / 09-05-1999 : Fixed a problem as documented in KB article "PRB: Menus for 
                            Notification Icons Do Not Work Correctly", Article ID: Q135788 
         PJN / 15-05-1999 : 1. Now uses the author's hookwnd class. This prevents the need to 
                            create the two hidden windows namely CTrayRessurectionWnd and
                            CTrayTimerWnd
                            2. Code now compiles cleanly at warning level 4
                            3. General code tidy up and rearrangement
                            4. Added numerous ASSERT's to improve the code's robustness
                            5. Added functions to allow context menu to be customized
         PJN / 01-01-2001 : 1. Now includes copyright message in the source code and documentation. 
                            2. Fixed problem where the window does not get focus after double clicking 
                            on the tray icon
                            3. Now fully supports the Windows 2000 balloon style tooltips
                            4. Fixed a off by one problem in some of the ASSERT's
                            5. Fixed problems with Unicode build configurations
                            6. Provided Win2k specific build configurations
         PJN / 10-02-2001 : 1. Now fully supports creation of 2 tray icons at the same time
         PJN / 13-06-2001 : 1. Now removes windows hook upon call to RemoveIcon
         PJN / 26-08-2001 : 1. Fixed memory leak in RemoveIcon.
                            2. Fixed GPF in RemoveIcon by removing call to Unhook
         PJN / 28-08-2001 : 1. Added support for direct access to the System Tray's HDC. This allows
                            you to generate an icon for the tray on the fly using GDI calls. The idea
                            came from the article by Jeff Heaton in the April issue of WDJ. Also added
                            are overriden Create methods to allow you to easily costruct a dynamic
                            tray icon given a BITMAP instead of an ICON.
         PJN / 21-03-2003   1. Fixed icon resource leaks in SetIcon(LPCTSTR lpIconName) and 
                            SetIcon(UINT nIDResource). Thanks to Egor Pervouninski for reporting this.
                            2. Fixed unhooking of the tray icon when the notification window is being
                            closed.
         PJN / 31-03-2003   1. Now uses V1.05 of my Hookwnd class
         PJN / 02-04-2003   1. Now uses v1.06 of my Hookwnd class
                            2. Fixed a bug in the sample app for this class where the hooks should
                            have been created as global instances rather than as member variables of
                            the mainframe window. This ensures that the hooks remain valid even after
                            calling DefWindowProc on the mainframe.
           

Copyright (c) 1997 - 2003 by PJ Naughter.  (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/

/////////////////////////////////  Includes  //////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ntray.h"



/////////////////////////////////  Macros /////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



///////////////////////////////// Implementation //////////////////////////////

const UINT wm_TaskbarCreated = RegisterWindowMessage(_T("TaskbarCreated"));

CTrayIconHooker::CTrayIconHooker()
{
  //Set all of our variables to defaults
  m_pTrayIcon = NULL;
  m_phIcons = NULL;
  m_nNumIcons = 0;
  m_nTimerID = 0;
  m_nCurrentIconIndex = 0;
}

CTrayIconHooker::~CTrayIconHooker()
{
  StopUsingAnimation();
}

void CTrayIconHooker::Init(CTrayNotifyIcon* pTrayIcon, CWnd* pNotifyWnd)
{
  //Validate our parameters
  ASSERT(!IsHooked());
  ASSERT(pTrayIcon); //must have a valid tray notify instance
  ASSERT(pNotifyWnd && ::IsWindow(pNotifyWnd->GetSafeHwnd()));

  //Hook the top level frame of the notify window in preference 
  //to the notify window itself. This will ensure that we get
  //the taskbar created message
  CWnd* pTopLevelWnd = pNotifyWnd->GetTopLevelFrame();
  if (pTopLevelWnd)
    Hook(pTopLevelWnd); 
  else
    Hook(pNotifyWnd);

  m_pTrayIcon = pTrayIcon;
}

void CTrayIconHooker::StartUsingAnimation(HICON* phIcons, int nNumIcons, DWORD dwDelay)
{
  //Validate our parameters
  ASSERT(nNumIcons >= 2); //must be using at least 2 icons if you are using animation
  ASSERT(phIcons);        //array of icon handles must be valid
  ASSERT(dwDelay);        //must be non zero timer interval
  ASSERT(m_pTrayIcon);

  //Stop the animation if already started  
  StopUsingAnimation();

  //Hive away all the values locally
  ASSERT(m_phIcons == NULL);
  m_phIcons = new HICON[nNumIcons];
  for (int i=0; i<nNumIcons; i++)
    m_phIcons[i] = phIcons[i];
  m_nNumIcons = nNumIcons;

  //Start up the timer 
  m_nTimerID = SetTimer(m_hWnd, m_pTrayIcon->m_NotifyIconData.uID, dwDelay, NULL);
}

void CTrayIconHooker::StopUsingAnimation()
{
  //Kill the timer
  if (m_nTimerID)
  {
    if (::IsWindow(m_hWnd))
      KillTimer(m_hWnd, m_nTimerID);
    m_nTimerID = 0;
  }
 
  //Free up the memory
  if (m_phIcons)
  {
    delete [] m_phIcons;
    m_phIcons = NULL;
  }

  //Reset the other animation related variables
  m_nCurrentIconIndex = 0;
  m_nNumIcons = 0;
}

BOOL CTrayIconHooker::UsingAnimatedIcon() const
{
  return (m_nNumIcons != 0);
}

HICON CTrayIconHooker::GetCurrentIcon() const 
{ 
  ASSERT(UsingAnimatedIcon());
  ASSERT(m_phIcons);
  return m_phIcons[m_nCurrentIconIndex]; 
}

BOOL CTrayIconHooker::ProcessWindowMessage(HWND /*hWnd*/, UINT nMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
  ASSERT(m_pTrayIcon);
  lResult = 0;
  BOOL bHandled = FALSE;

  if (nMsg == wm_TaskbarCreated)
  {
    lResult = OnTaskbarCreated(wParam, lParam);
    bHandled = FALSE;
  }
  else
  {
    switch (nMsg)
    {
      case WM_TIMER: 
      {
        if (wParam == m_pTrayIcon->m_NotifyIconData.uID)  //It's our timer
        {
          OnTimer(wParam); 
          bHandled = TRUE;
        }
        break;
      }
      case WM_DESTROY: 
      {
        m_pTrayIcon->RemoveIcon();
        break;
      }
      default: 
      {
        break;
      }
    }
  }

  return bHandled;
}

#ifdef _DEBUG
void CTrayIconHooker::OnTimer(UINT nIDEvent)
#else
void CTrayIconHooker::OnTimer(UINT /*nIDEvent*/)  //Just to avoid a compiler warning 
#endif                                            //when being built for release
{
  ASSERT(nIDEvent == m_nTimerID);

  //increment the icon index
  ++m_nCurrentIconIndex;
  m_nCurrentIconIndex = m_nCurrentIconIndex % m_nNumIcons;

  //update the tray icon
  m_pTrayIcon->m_NotifyIconData.uFlags = NIF_ICON;
  m_pTrayIcon->m_NotifyIconData.hIcon = m_phIcons[m_nCurrentIconIndex];
  Shell_NotifyIcon(NIM_MODIFY, (PNOTIFYICONDATA) &m_pTrayIcon->m_NotifyIconData);
}

LRESULT CTrayIconHooker::OnTaskbarCreated(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
  ASSERT(m_pTrayIcon);

  //Refresh the tray icon if necessary
  if (m_pTrayIcon->IsShowing())
  {
    m_pTrayIcon->HideIcon();
    m_pTrayIcon->ShowIcon();
  }

  return 0L;
}




IMPLEMENT_DYNAMIC(CTrayNotifyIcon, CObject)

CTrayNotifyIcon::CTrayNotifyIcon()
{
  ZeroMemory(&m_NotifyIconData, sizeof(NOTIFYICONDATA_2));
  m_bCreated = FALSE;
  m_bHidden = FALSE;
  m_pNotificationWnd = NULL;
}

CTrayNotifyIcon::~CTrayNotifyIcon()
{
  RemoveIcon();
}

void CTrayNotifyIcon::HideIcon()
{
  ASSERT(m_bCreated);
	if (!m_bHidden) 
  {
		m_NotifyIconData.uFlags = NIF_ICON;
		Shell_NotifyIcon(NIM_DELETE, (PNOTIFYICONDATA) &m_NotifyIconData);
		m_bHidden = TRUE;
	}
}

void CTrayNotifyIcon::ShowIcon()
{
  ASSERT(m_bCreated);
	if (m_bHidden) 
  {
		m_NotifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		Shell_NotifyIcon(NIM_ADD, (PNOTIFYICONDATA) &m_NotifyIconData);
		m_bHidden = FALSE;
	}
}

void CTrayNotifyIcon::RemoveIcon()
{
  if (m_bCreated)
  {
    m_NotifyIconData.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, (PNOTIFYICONDATA) &m_NotifyIconData);

    m_HookWnd.StopUsingAnimation();
    m_HookWnd.Unhook();
    m_bCreated = FALSE;
  }
}

void CTrayNotifyIcon::MoveToExtremeRight()
{
  HideIcon();
  ShowIcon();
}

void CTrayNotifyIcon::SetMenu(HMENU hMenu)
{
  //Validate our parameters
  ASSERT(hMenu);

  m_Menu.DestroyMenu();
  m_Menu.Attach(hMenu);

  CMenu* pSubMenu = m_Menu.GetSubMenu(0);
  if (!pSubMenu) 
    ASSERT(FALSE); //Your menu resource has been designed incorrectly
}

CMenu& CTrayNotifyIcon::GetMenu()
{
  return m_Menu;
}

BOOL CTrayNotifyIcon::Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, HICON hIcon, UINT nNotifyMessage, UINT uMenuID, BOOL bWin2k)
{
  //Validate our parameters
  ASSERT(pNotifyWnd && ::IsWindow(pNotifyWnd->GetSafeHwnd()));
  if (bWin2k)
    ASSERT(_tcslen(pszTooltipText) < 128);
  else
    ASSERT(_tcslen(pszTooltipText) < 64);
  ASSERT(hIcon); 
  ASSERT(nNotifyMessage >= WM_USER); //Make sure we avoid conflict with other messages

  //Load up the menu resource which is to be used as the context menu
  if (!m_Menu.LoadMenu(uMenuID == 0 ? uID : uMenuID))
  {
    ASSERT(FALSE);
    return FALSE;
  }
  CMenu* pSubMenu = m_Menu.GetSubMenu(0);
  if (!pSubMenu) 
  {
    ASSERT(FALSE); //Your menu resource has been designed incorrectly
    return FALSE;
  }
  // Make first menu item the default (bold font)
  ::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

  //Install the hook
  m_HookWnd.Init(this, pNotifyWnd);

  //Call the Shell_NotifyIcon function
  m_pNotificationWnd = pNotifyWnd;
  if (bWin2k)
    m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA_2);
  else
    m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA_1);
  m_NotifyIconData.hWnd = pNotifyWnd->GetSafeHwnd();
  m_NotifyIconData.uID = uID;
  m_NotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  m_NotifyIconData.uCallbackMessage = nNotifyMessage;
  m_NotifyIconData.hIcon = hIcon;
  _tcscpy(m_NotifyIconData.szTip, pszTooltipText);
  m_bCreated = Shell_NotifyIcon(NIM_ADD, (PNOTIFYICONDATA) &m_NotifyIconData);

  return m_bCreated;
}

HICON CTrayNotifyIcon::BitmapToIcon(CBitmap* pBitmap)
{
  //Validate our parameters
  ASSERT(pBitmap);

  //Get the width and height of a small icon
  int w = GetSystemMetrics(SM_CXSMICON);
  int h = GetSystemMetrics(SM_CYSMICON);

  //Create a 0 mask
  int nMaskSize = h*(w/8);
  unsigned char* pMask = new unsigned char[nMaskSize];
  ZeroMemory(pMask, nMaskSize);

  //Create a mask bitmap
  CBitmap maskBitmap;
  BOOL bSuccess = maskBitmap.CreateBitmap(w, h, 1, 1, pMask);

  //Free up the heap memory now that we have created the mask bitmap
  delete [] pMask;

  //Handle the error
  if (!bSuccess)
    return NULL;

  //Create an ICON base on the bitmap just created
  ICONINFO iconInfo;
  iconInfo.fIcon = TRUE;
  iconInfo.xHotspot = 0;
  iconInfo.yHotspot = 0;
  iconInfo.hbmMask = maskBitmap;
  iconInfo.hbmColor = *pBitmap; 
  return CreateIconIndirect(&iconInfo); 
}

BOOL CTrayNotifyIcon::Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, CBitmap* pBitmap, UINT nNotifyMessage, UINT uMenuID, BOOL bWin2k)
{
  //Convert the bitmap to an ICON
  HICON hIcon = BitmapToIcon(pBitmap);

  //Pass the buck to the other function to do the work
  BOOL bSuccess = Create(pNotifyWnd, uID, pszTooltipText, hIcon, nNotifyMessage, uMenuID, bWin2k);
      
  //Free up all the resources now that we are finished with them
  DestroyIcon(hIcon);

  return bSuccess;
}

BOOL CTrayNotifyIcon::Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, HICON* phIcons, int nNumIcons, DWORD dwDelay, UINT nNotifyMessage, UINT uMenuID, BOOL bWin2k)
{
  //Validate our parameters
  ASSERT(phIcons);
  ASSERT(nNumIcons >= 2); //must be using at least 2 icons if you are using animation
  ASSERT(dwDelay);

  //let the normal Create function do its stuff
  BOOL bSuccess = Create(pNotifyWnd, uID, pszTooltipText, phIcons[0], nNotifyMessage, uMenuID, bWin2k);

  //tell the hook class to do the animation
  m_HookWnd.StartUsingAnimation(phIcons, nNumIcons, dwDelay);

  return bSuccess;
}

BOOL CTrayNotifyIcon::Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, HICON hIcon, UINT nNotifyMessage, UINT uMenuID)
{
  //Validate our parameters
  ASSERT(pNotifyWnd && ::IsWindow(pNotifyWnd->GetSafeHwnd()));
  ASSERT(_tcslen(pszTooltipText) < 128);
  ASSERT(_tcslen(pszBalloonText) < 256);
  ASSERT(_tcslen(pszBalloonCaption) < 64);
  ASSERT(hIcon); 
  ASSERT(nNotifyMessage >= WM_USER); //Make sure we avoid conflict with other messages

  //Load up the menu resource which is to be used as the context menu
  if (!m_Menu.LoadMenu(uMenuID == 0 ? uID : uMenuID))
  {
    ASSERT(FALSE);
    return FALSE;
  }
  CMenu* pSubMenu = m_Menu.GetSubMenu(0);
  if (!pSubMenu) 
  {
    ASSERT(FALSE); //Your menu resource has been designed incorrectly
    return FALSE;
  }
  // Make first menu item the default (bold font)
  ::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

  //Install the hook
  m_HookWnd.Init(this, pNotifyWnd);

  //Call the Shell_NotifyIcon function
  m_pNotificationWnd = pNotifyWnd;
  m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA_2);
  m_NotifyIconData.hWnd = pNotifyWnd->GetSafeHwnd();
  m_NotifyIconData.uID = uID;
  m_NotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
  m_NotifyIconData.uCallbackMessage = nNotifyMessage;
  m_NotifyIconData.hIcon = hIcon;
  _tcscpy(m_NotifyIconData.szTip, pszTooltipText);
  _tcscpy(m_NotifyIconData.szInfo, pszBalloonText);
  _tcscpy(m_NotifyIconData.szInfoTitle, pszBalloonCaption);
  m_NotifyIconData.uTimeout = nTimeout;
  switch (style)
  {
    case Warning:
    {
      m_NotifyIconData.dwInfoFlags = NIIF_WARNING;
      break;
    }
    case Error:
    {
      m_NotifyIconData.dwInfoFlags = NIIF_ERROR;
      break;
    }
    case Info:
    {
      m_NotifyIconData.dwInfoFlags = NIIF_INFO;
      break;
    }
    default:
    {
      ASSERT(FALSE);
      break;
    }
  }

  m_bCreated = Shell_NotifyIcon(NIM_ADD, (PNOTIFYICONDATA) &m_NotifyIconData);
  return m_bCreated;
}

BOOL CTrayNotifyIcon::Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, CBitmap* pBitmap, UINT nNotifyMessage, UINT uMenuID)
{
  //Convert the bitmap to an ICON
  HICON hIcon = BitmapToIcon(pBitmap);

  //Pass the buck to the other function to do the work
  BOOL bSuccess = Create(pNotifyWnd, uID, pszTooltipText, pszBalloonText, pszBalloonCaption, nTimeout, style, hIcon, nNotifyMessage, uMenuID);
      
  //Free up all the resources now that we are finished with them
  DestroyIcon(hIcon);

  return bSuccess;
}

BOOL CTrayNotifyIcon::Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, HICON* phIcons, int nNumIcons, DWORD dwDelay, UINT nNotifyMessage, UINT uMenuID)
{
  //Validate our parameters
  ASSERT(phIcons);
  ASSERT(nNumIcons >= 2); //must be using at least 2 icons if you are using animation
  ASSERT(dwDelay);

  //let the normal Create function do its stuff
  BOOL bSuccess = Create(pNotifyWnd, uID, pszTooltipText, pszBalloonText, pszBalloonCaption, nTimeout, style, phIcons[0], nNotifyMessage, uMenuID);

  //tell the hook class to do the animation
  m_HookWnd.StartUsingAnimation(phIcons, nNumIcons, dwDelay);

  return bSuccess;
}

BOOL CTrayNotifyIcon::SetBalloonDetails(LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, BalloonStyle style, UINT nTimeout)
{
  //Validate our parameters
  ASSERT(_tcslen(pszBalloonText) < 256);
  ASSERT(_tcslen(pszBalloonCaption) < 64);

  if (!m_bCreated)
    return FALSE;

  //Call the Shell_NotifyIcon function
  m_NotifyIconData.uFlags = NIF_INFO;
  _tcscpy(m_NotifyIconData.szInfo, pszBalloonText);
  _tcscpy(m_NotifyIconData.szInfoTitle, pszBalloonCaption);
  m_NotifyIconData.uTimeout = nTimeout;
  switch (style)
  {
    case Warning:
    {
      m_NotifyIconData.dwInfoFlags = NIIF_WARNING;
      break;
    }
    case Error:
    {
      m_NotifyIconData.dwInfoFlags = NIIF_ERROR;
      break;
    }
    case Info:
    {
      m_NotifyIconData.dwInfoFlags = NIIF_INFO;
      break;
    }
    default:
    {
      ASSERT(FALSE);
      break;
    }
  }

  return Shell_NotifyIcon(NIM_MODIFY, (PNOTIFYICONDATA) &m_NotifyIconData);
}

CString CTrayNotifyIcon::GetBalloonText() const
{
  CString sText;
  if (m_bCreated)
    sText = m_NotifyIconData.szInfo;

  return sText;
}

CString CTrayNotifyIcon::GetBalloonCaption() const
{
  CString sText;
  if (m_bCreated)
    sText = m_NotifyIconData.szInfoTitle;

  return sText;
}

UINT CTrayNotifyIcon::GetBalloonTimeout() const
{
  UINT nTimeout = 0;
  if (m_bCreated)
    nTimeout = m_NotifyIconData.uTimeout;

  return nTimeout;
}

BOOL CTrayNotifyIcon::SetTooltipText(LPCTSTR pszTooltipText, BOOL bWin2k)
{
  if (!m_bCreated)
    return FALSE;

  if (!bWin2k)
  {
    ASSERT(_tcslen(pszTooltipText) < 64);
    m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA_1);
  }
  else
  {
    ASSERT(_tcslen(pszTooltipText) < 128);
    m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA_2);
  }

  //Call the Shell_NotifyIcon function
  m_NotifyIconData.uFlags = NIF_TIP;
  _tcscpy(m_NotifyIconData.szTip, pszTooltipText);
  return Shell_NotifyIcon(NIM_MODIFY, (PNOTIFYICONDATA) &m_NotifyIconData);
}

BOOL CTrayNotifyIcon::SetTooltipText(UINT nID, BOOL bWin2k)
{
  CString sToolTipText;
  VERIFY(sToolTipText.LoadString(nID));

  //Let the other version of the function handle the rest
  return SetTooltipText(sToolTipText, bWin2k);
}

BOOL CTrayNotifyIcon::SetIcon(CBitmap* pBitmap)
{
  //Convert the bitmap to an ICON
  HICON hIcon = BitmapToIcon(pBitmap);

  //Pass the buck to the other function to do the work
  BOOL bSuccess = SetIcon(hIcon);

  //Destroy the icon now that we are finished with it      
  DestroyIcon(hIcon);

  return bSuccess;
}

BOOL CTrayNotifyIcon::SetIcon(HICON hIcon)
{
  //Validate our parameters
  ASSERT(hIcon);

  if (!m_bCreated)
    return FALSE;

  //Since we are going to use one icon, stop any animation
  m_HookWnd.StopUsingAnimation();

  //Call the Shell_NotifyIcon function
  m_NotifyIconData.uFlags = NIF_ICON;
  m_NotifyIconData.hIcon = hIcon;
  return Shell_NotifyIcon(NIM_MODIFY, (PNOTIFYICONDATA) &m_NotifyIconData);
}

BOOL CTrayNotifyIcon::SetIcon(LPCTSTR lpIconName)
{
  HICON hIcon = LoadIconResource(lpIconName);
  BOOL bSuccess = SetIcon(hIcon);
  DestroyIcon(hIcon);
  return bSuccess;
}

BOOL CTrayNotifyIcon::SetIcon(UINT nIDResource)
{
  HICON hIcon = LoadIconResource(nIDResource);
  BOOL bSuccess = SetIcon(hIcon);
  DestroyIcon(hIcon);
  return bSuccess;
}

BOOL CTrayNotifyIcon::SetStandardIcon(LPCTSTR lpIconName)
{
  HICON hIcon = ::LoadIcon(NULL, lpIconName);
  return SetIcon(hIcon);
}

BOOL CTrayNotifyIcon::SetStandardIcon(UINT nIDResource)
{
  HICON hIcon = ::LoadIcon(NULL, MAKEINTRESOURCE(nIDResource));
  return SetIcon(hIcon);
}

BOOL CTrayNotifyIcon::SetIcon(HICON* phIcons, int nNumIcons, DWORD dwDelay)
{
  //Validate our parameters
  ASSERT(nNumIcons >= 2); //must be using at least 2 icons if you are using animation
  ASSERT(phIcons);
  ASSERT(dwDelay);

  if (!SetIcon(phIcons[0]))
    return FALSE;

  //Install the hook
  m_HookWnd.StartUsingAnimation(phIcons, nNumIcons, dwDelay);

  return TRUE;
}

BOOL CTrayNotifyIcon::SetNotificationWnd(CWnd* pNotifyWnd)
{
  //Validate our parameters
  ASSERT(pNotifyWnd && ::IsWindow(pNotifyWnd->GetSafeHwnd()));

  if (!m_bCreated)
    return FALSE;

  //Call the Shell_NotifyIcon function
  m_pNotificationWnd = pNotifyWnd;
  m_NotifyIconData.hWnd = pNotifyWnd->GetSafeHwnd();
  m_NotifyIconData.uFlags = 0;
  return Shell_NotifyIcon(NIM_MODIFY, (PNOTIFYICONDATA) &m_NotifyIconData);
}

CString CTrayNotifyIcon::GetTooltipText() const
{
  CString sText;
  if (m_bCreated)
    sText = m_NotifyIconData.szTip;

  return sText;
}

HICON CTrayNotifyIcon::GetIcon() const
{
  HICON hIcon = NULL;
  if (m_bCreated)
  {
    if (UsingAnimatedIcon())
      hIcon = m_HookWnd.GetCurrentIcon();
    else
      hIcon = m_NotifyIconData.hIcon;
  }

  return hIcon;
}

BOOL CTrayNotifyIcon::UsingAnimatedIcon() const
{
  return m_HookWnd.UsingAnimatedIcon();
}

CWnd* CTrayNotifyIcon::GetNotificationWnd() const
{
  return m_pNotificationWnd;
}

LRESULT CTrayNotifyIcon::OnTrayNotification(WPARAM wID, LPARAM lEvent)
{
  //Return quickly if its not for this tray icon
  if (wID != m_NotifyIconData.uID)
    return 0L;

  CMenu* pSubMenu = m_Menu.GetSubMenu(0);
  ASSERT(pSubMenu); //Your menu resource has been designed incorrectly

  if (lEvent == WM_RBUTTONUP)  //Clicking with right button brings up a context menu
  {
    CPoint pos;
    GetCursorPos(&pos);
    ::SetForegroundWindow(m_NotifyIconData.hWnd);  
    ::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, m_NotifyIconData.hWnd, NULL);
    ::PostMessage(m_NotifyIconData.hWnd, WM_NULL, 0, 0);
  } 
  else if (lEvent == WM_LBUTTONDBLCLK) //double click received, the default action is to execute first menu item 
  {
    ::SetForegroundWindow(m_NotifyIconData.hWnd);
    ::SendMessage(m_NotifyIconData.hWnd, WM_COMMAND, pSubMenu->GetMenuItemID(0), 0);
  }

  return 1; // handled
}

HICON CTrayNotifyIcon::LoadIconResource(LPCTSTR lpIconName)
{
  //First try to load the 16*16 icon, if this fails, they fall back on the 32*32 
  HICON hIcon = (HICON) ::LoadImage(AfxGetResourceHandle(), lpIconName, IMAGE_ICON, 16, 16, 0);
  if (hIcon == NULL)
    hIcon = AfxGetApp()->LoadIcon(lpIconName);

  //Return the icon handle
  return hIcon;
}

HICON CTrayNotifyIcon::LoadIconResource(UINT nIDResource)
{
  return LoadIconResource(MAKEINTRESOURCE(nIDResource));
}

BOOL CTrayNotifyIcon::GetDynamicDCAndBitmap(CDC* pDC, CBitmap* pBitmap)
{
  //Validate our parameters
  ASSERT(pDC != NULL);
  ASSERT(pBitmap != NULL);

  //Get the HWND for the desktop
  CWnd* pWndScreen = CWnd::GetDesktopWindow();
  if (pWndScreen == NULL)
    return FALSE;

  //Get the desktop HDC to create a compatible bitmap from
  CDC* pDCScreen = pWndScreen->GetDC();
  if (pDCScreen == NULL)
    return FALSE;

  //Get the width and height of a small icon
  int w = GetSystemMetrics(SM_CXSMICON);
  int h = GetSystemMetrics(SM_CYSMICON);

  //Create an off-screen bitmap that the dynamic tray icon 
  //can be drawn into. (Compatible with the desktop DC).
  BOOL bSuccess = pBitmap->CreateCompatibleBitmap(pDCScreen, w, h);
  if (!bSuccess)
  {
    pWndScreen->ReleaseDC(pDCScreen);
    return FALSE;
  }

  //Get a HDC to the newly created off-screen bitmap
  bSuccess = pDC->CreateCompatibleDC(pDCScreen);
  if (!bSuccess)
  {
    //Release the Screen DC now that we are finished with it
    pWndScreen->ReleaseDC(pDCScreen);

    //Free up the bitmap now that we are finished with it
    pBitmap->DeleteObject();

    return FALSE;
  }

  //Select the bitmap into the offscreen DC
  pDC->SelectObject(pBitmap);

  //Release the Screen DC now that we are finished with it
  pWndScreen->ReleaseDC(pDCScreen);

  return TRUE;
}

