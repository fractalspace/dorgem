/*
Module : NTRAY.H
Purpose: Interface for a MFC class to encapsulate Shell_NotifyIcon
Created: PJN / 14-05-1997
History: None


Copyright (c) 1997 - 2003 by PJ Naughter.  (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


////////////////////////////////// Macros ///////////////////////////

#ifndef _NTRAY_H__
#define _NTRAY_H__



/////////////////////////// Includes ////////////////////////////////

#include "hookwnd.h" //If the code fails to compile because of this missing include then you need to download 
                     //the CHookWnd class from my web site and copy the relavent files across




/////////////////////////// Classes /////////////////////////////////

//forward declaration
class CTrayNotifyIcon;


//Class which handles subclassing the top level window and handles the timer
//required for tray icon animation and taskbar creation message
class CTrayIconHooker : public CHookWnd
{
public:
//Constructors / Destructors
  CTrayIconHooker();
  ~CTrayIconHooker();

//Methods
  void Init(CTrayNotifyIcon* pTrayIcon, CWnd* pNotifyWnd);
  void StartUsingAnimation(HICON* phIcons, int nNumIcons, DWORD dwDelay);
  void StopUsingAnimation();
  BOOL UsingAnimatedIcon() const;
  HICON GetCurrentIcon() const;

protected:
  virtual BOOL ProcessWindowMessage(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
  void OnTimer(UINT nIDEvent);
  LRESULT OnTaskbarCreated(WPARAM wParam, LPARAM lParam);

  CTrayNotifyIcon* m_pTrayIcon;
  HICON* m_phIcons;
  int m_nNumIcons;
  UINT m_nTimerID;
  int m_nCurrentIconIndex;
};


//the actual tray notification class wrapper
class CTrayNotifyIcon : public CObject
{
public:
//Enums / Typedefs
#ifndef CTRAYNOTIFYICON_NOWIN2K
  enum BalloonStyle
  {
    Warning,
    Error,
    Info
  };
#endif

//These defines are taken from the Windows 2000 platform SDK, This allows
//the CTrayNotifyIcon to be able to use the Windows 2000 style balloon 
//tooltips etc without having the latest and greatest Platform SDK installed
typedef struct _NOTIFYICONDATA_1
{
  DWORD cbSize;
  HWND hWnd;
  UINT uID;
  UINT uFlags;
  UINT uCallbackMessage;
  HICON hIcon;
  TCHAR szTip[64];
} NOTIFYICONDATA_1;

typedef struct _NOTIFYICONDATA_2
{
  DWORD cbSize;
  HWND hWnd;
  UINT uID;
  UINT uFlags;
  UINT uCallbackMessage;
  HICON hIcon;
  TCHAR szTip[128];
  DWORD dwState;
  DWORD dwStateMask;
  TCHAR szInfo[256];
  union 
  {
    UINT uTimeout;
    UINT uVersion;
  } DUMMYUNIONNAME;
  TCHAR szInfoTitle[64];
  DWORD dwInfoFlags;
} NOTIFYICONDATA_2;

#ifndef NIF_INFO
#define NIF_INFO        0x00000010
#endif

#ifndef NIIF_INFO
#define NIIF_INFO       0x00000001
#endif 

#ifndef NIIF_WARNING
#define NIIF_WARNING    0x00000002
#endif

#ifndef NIIF_ERROR
#define NIIF_ERROR      0x00000003
#endif


//Constructors / Destructors
  CTrayNotifyIcon();
  ~CTrayNotifyIcon();

//Create the tray icon
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, HICON hIcon, UINT nNotifyMessage, UINT uMenuID=0, BOOL bWin2k=FALSE);                  
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, CBitmap* pBitmap, UINT nNotifyMessage, UINT uMenuID=0, BOOL bWin2k=FALSE);
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, HICON* phIcons, int nNumIcons, DWORD dwDelay, UINT nNotifyMessage, UINT uMenuID=0, BOOL bWin2k=FALSE);
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, HICON hIcon, UINT nNotifyMessage, UINT uMenuID=0);                  
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, CBitmap* pBitmap, UINT nNotifyMessage, UINT uMenuID=0);
  BOOL Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, UINT nTimeout, BalloonStyle style, HICON* phIcons, int nNumIcons, DWORD dwDelay, UINT nNotifyMessage, UINT uMenuID=0);

//Sets or gets the Tooltip text
  BOOL SetTooltipText(LPCTSTR pszTooltipText, BOOL bWin2k=FALSE);
  BOOL SetTooltipText(UINT nID, BOOL bWin2k=FALSE);
  CString GetTooltipText() const;

//Sets or gets the icon displayed
  BOOL SetIcon(HICON hIcon);
  BOOL SetIcon(CBitmap* pBitmap);
  BOOL SetIcon(LPCTSTR lpIconName);
  BOOL SetIcon(UINT nIDResource);
  BOOL SetIcon(HICON* phIcons, int nNumIcons, DWORD dwDelay);
  BOOL SetStandardIcon(LPCTSTR lpIconName);
  BOOL SetStandardIcon(UINT nIDResource);
  HICON GetIcon() const;
  BOOL UsingAnimatedIcon() const;

//Sets or gets the window to send notification messages to
  BOOL SetNotificationWnd(CWnd* pNotifyWnd);
  CWnd* GetNotificationWnd() const;

//Modification of the tray icons
	void HideIcon();
	void ShowIcon();
	void RemoveIcon();
	void MoveToExtremeRight();

//Dynamic tray icon support
  HICON BitmapToIcon(CBitmap* pBitmap);
  static BOOL GetDynamicDCAndBitmap(CDC* pDC, CBitmap* pBitmap);

//Modification of the menu to use as the context menu
  void SetMenu(HMENU hMenu);
  CMenu& GetMenu();

//Default handler for tray notification message
  virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);

//Status information
  BOOL IsShowing() const { return !IsHidden(); };
  BOOL IsHidden() const { return m_bHidden; };

  //Sets or gets the Balloon style tooltip settings
  BOOL         SetBalloonDetails(LPCTSTR pszBalloonText, LPCTSTR pszBalloonCaption, BalloonStyle style, UINT nTimeout);
  CString      GetBalloonText() const;
  CString      GetBalloonCaption() const;
  BalloonStyle GetBalloonStyle() const;
  UINT         GetBalloonTimeout() const;

//Helper functions to load tray icon from resources
  static HICON LoadIconResource(LPCTSTR lpIconName);
  static HICON LoadIconResource(UINT nIDResource);

protected:
  NOTIFYICONDATA_2 m_NotifyIconData;
  BOOL             m_bCreated;
  BOOL             m_bHidden;
  CWnd*            m_pNotificationWnd;
  CTrayIconHooker  m_HookWnd;
  CMenu            m_Menu;

  DECLARE_DYNAMIC(CTrayNotifyIcon)

  friend class CTrayIconHooker;
};

#endif //_NTRAY_H__
