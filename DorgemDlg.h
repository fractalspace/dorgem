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
// DorgemDlg.h : header file

#ifndef __DORGEMDLG_H__
#define __DORGEMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAVICapWnd;
class CPreviewDlg;
class CTrayNotifyIcon;
class CWriteAVI;
class CInstanceChecker;
class CCaptionManager;

#include "Tm.h"

/////////////////////////////////////////////////////////////////////////////
// CDorgemDlg dialog

class CDorgemDlg : public CDialog
{
// Construction
public:
  CDorgemDlg(CWnd* pParent = NULL); // standard constructor
  ~CDorgemDlg();

// Dialog Data
  //{{AFX_DATA(CDorgemDlg)
  enum { IDD = IDD_DORGEM_DIALOG };
  CSpinButtonCtrl m_spinCompress;
  CComboBox m_cmbDevices;
  UINT  m_iCompress;
  BOOL  m_bCapture;
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDorgemDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDorgemDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnPreview();
  afx_msg void OnSelChangeDevices();
  afx_msg void OnCaptureNow();
  afx_msg void OnDevDisplay();
  afx_msg void OnDevFormat();
  afx_msg void OnDevSource();
  afx_msg void OnDestroy();
  afx_msg void OnHelp();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnAboutbox();
  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
  afx_msg void OnCaptionSettings();
  afx_msg void OnStoreSettings();
  afx_msg void OnChangeCompress();
  afx_msg void OnRestore();
  afx_msg void OnOptions();
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg void OnCapture();
  //}}AFX_MSG
  afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

public:
  CInstanceChecker* m_pInstanceChecker;

  void ReadSettings();
  BOOL DoCapture(CString, BOOL, CCaptionManager*);
  void SetStatusBar(LPCSTR = NULL);

protected:
  HICON             m_hIcon;                  // The icon
  CTrayNotifyIcon*  m_pTrayIcon;              // The tray icon
  CWriteAVI*        m_pAviFile;               // The AVI file
  CAVICapWnd*       m_pwndPreview;            // The preview window
  CPreviewDlg*      m_dlgPreview;             // The dialog around the preview window
  BOOL              m_bPreviewShown;          // Is the preview window shown
  DWORD             m_dwPreviewRate;          // The refresh rate of the preview window
  int               m_iDeviceID;              // The ID of the connected device
  BOOL              m_bUseMotionDetection;    // Use the motion detection?
  BYTE*             m_pPrevBits;              // The previous image (used by motion detection)
  BOOL              m_bReconnect;             // Reconnect device if no motion is detected
  int               m_iReconnectTime;         // Reconnect time-out
  CTM               m_tmLastMotion;           // Last time the camera has detected motion

  BOOL ConnectDevice();
  void EnableItems(BOOL);
  void ResizePreview(UINT, UINT);
  void PutOverlays(CCaptionManager*, HDC, LONG, LONG);
  BOOL WriteJPEG(LPCSTR, BYTE*, LONG, LONG);

  void OverlayText(HDC, int, int);
  void OverlayBitmap(HDC, int, int);
  void TranspBlt(HDC, int, int, int, int, HDC, COLORREF);
  BOOL MotionAnalysis(BITMAPINFOHEADER*, BYTE*, BYTE*);
  BOOL ShowMsgBoxes();

};

inline CDorgemDlg* GetMainWnd()    { return((CDorgemDlg*) GetApp()->m_pMainWnd); };

#endif // __DORGEMDLG_H__
