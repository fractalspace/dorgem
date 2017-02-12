/*
Module : VFWWnd.h
Purpose: Interface for an MFC class which encapsulates the "AviCap" window class

Copyright (c) 2000 by PJ Naughter.  
All rights reserved.

*/



////////////////////////////////// Macros / Defines  ///////////////////////

#ifndef __VFWWND_H__
#define __VFWWND_H__

///////////////////////////////// Includes /////////////////////////////////

#ifndef _INC_VFW
#include <vfw.h>
#endif //_INC_VFW

#ifndef _INC_MMREG
#include <mmreg.h>
#endif //_INC_MMREG

#pragma comment(lib, "vfw32.lib")



//Class which implements the "AViCap" Windows class

class CAVICapWnd : public CWnd
{
public:
//Constructors / Destructors
	CAVICapWnd();
	virtual ~CAVICapWnd();

//Basic Capture Operations
  static BOOL GetDescription(WORD nIndex, CString& sName, CString sVersion); //CapGetDriverDescription
	BOOL Create(DWORD dwStyle, const CRect& rect, CWnd* pParentWnd, UINT nID); //CapCreateCaptureWindow
	BOOL Abort();                                                              //WM_CAP_ABORT
	BOOL Connect(int nIndex);                                                  //WM_CAP_DRIVER_CONNECT
	BOOL Disconnect();                                                         //WM_CAP_DRIVER_DISCONNECT
	BOOL CaptureSequence();                                                    //WM_CAP_SEQUENCE
	BOOL Stop();                                                               //WM_CAP_STOP
	BOOL GetStatus(CAPSTATUS& status);                                         //WM_CAP_GET_STATUS
  BOOL GetSetup(CAPTUREPARMS& params);                                       //WM_CAP_GET_SEQUENCE_SETUP
  BOOL SetSetup(CAPTUREPARMS& params);                                       //WM_CAP_SET_SEQUENCE_SETUP

//Capture Windows
  BOOL GetCaps(CAPDRIVERCAPS& caps);    																		 //WM_CAP_DRIVER_GET_CAPS
  BOOL GetName(CString& sName);                                              //WM_CAP_DRIVER_GET_NAME
  BOOL GetVersion(CString& sVersion);																				 //WM_CAP_DRIVER_GET_VERSION

//Capture Driver Preview and overlay Modes
  BOOL SetOverlay(BOOL bOverlay);																						 //WM_CAP_SET_OVERLAY
  BOOL HasOverlay();
  BOOL IsOverlay();
	BOOL SetPreview(BOOL bPreview);																						 //WM_CAP_SET_PREVIEW
  BOOL IsPreview();
  BOOL SetPreviewRate(WORD wPreviewRate);                                    //WM_CAP_SET_PREVIEWRATE 
  BOOL SetScale(BOOL bScale);                                                //WM_CAP_SET_SCALE 
  BOOL SetScroll(const CPoint& point);                                       //WM_CAP_SET_SCROLL 

//Capture Driver Video Dialog Boxes
  BOOL ShowVideoCompressionDlg();                                            //WM_CAP_DLG_VIDEOCOMPRESSION
  BOOL ShowVideoOutputDlg();                                                 //WM_CAP_DLG_VIDEODISPLAY 
  BOOL ShowVideoFormatDlg();                                                 //WM_CAP_DLG_VIDEOFORMAT
  BOOL ShowVideoSourceDlg();                                                 //WM_CAP_DLG_VIDEOSOURCE
  BOOL ShowCompressionDlg();                                                 //WM_CAP_DLG_VIDEOCOMPRESSION   
  BOOL HasVideoOutputDlg();
  BOOL HasVideoFormatDlg();
  BOOL HasVideoSourceDlg();
  

//Audio / Video Format
	BOOL GetAudioFormat(WAVEFORMATEX*& pWaveFormat, DWORD& dwSize);						 //WM_CAP_GET_AUDIOFORMAT	
	BOOL SetAudioFormat(WAVEFORMATEX*	pWaveFormat);														 //WM_CAP_SET_AUDIOFORMAT
  BOOL SetAudioFormat(PCMWAVEFORMAT* pWaveFormat);		      							   //WM_CAP_SET_AUDIOFORMAT
	BOOL GetVideoFormat(BITMAPINFO*& pBitmapInfo);														 //WM_CAP_GET_VIDEOFORMAT
  BOOL SetVideoFormat(BITMAPINFO* pBitmapInfo);                              //WM_CAP_SET_VIDEOFORMAT

//Capture File and Buffers
  BOOL FileAllocate(DWORD dwSize);                                           //WM_CAP_FILE_ALLOCATE
  BOOL GetCaptureFile(CString& sFilename);                                   //WM_CAP_FILE_GET_CAPTURE_FILE
  BOOL SaveAs(const CString& sFilename);                                     //WM_CAP_FILE_SAVEAS 
  BOOL SetCaptureFile(const CString& sFilename);                             //WM_CAP_SET_CAPTURE_FILE

//Directly using Capture Data
  BOOL CaptureNoFile();                                                      //WM_CAP_SEQUENCE_NOFILE

//Capture from MCI Device
  BOOL SetMCIDevice(const CString& sName);                                   //WM_CAP_SET_MCI_DEVICE

//Manual Frame Capture
  BOOL CaptureSingleFrame();                                                 //WM_CAP_SINGLE_FRAME
  BOOL CaptureSingleFrameOpen();                                             //WM_CAP_SINGLE_FRAME_OPEN
  BOOL CaptureSingleFrameClose();                                            //WM_CAP_SINGLE_FRAME_CLOSE

//Still-Image Capture
  BOOL EditCopy();                                                           //WM_CAP_EDIT_COPY
  BOOL SaveDib(const CString& sFilename);                                    //WM_CAP_FILE_SAVEDIB
  BOOL GrabFrame();                                                          //WM_CAP_GRAB_FRAME
  BOOL GrabFrameNoStop();                                                    //WM_CAP_GRAB_FRAME_NOSTOP

//Advanced Capture Options
  BOOL  SetInfoChunk(const CAPINFOCHUNK* pInfoChunk);                        //WM_CAP_FILE_SET_INFOCHUNK
  DWORD GetUserData();                                                       //WM_CAP_GET_USER_DATA
  BOOL  SetUserData(DWORD dwData);                                           //WM_CAP_SET_USER_DATA

//Working with Palettes
  BOOL AutoCreatePalette(int nFrames, int nColors);                          //WM_CAP_PAL_AUTOCREATE
  BOOL ManualCreatePalette(BOOL bGrab, int nColors);                         //WM_CAP_PAL_MANUALCREATE
  BOOL OpenPalette(const CString& sFilename);                                //WM_CAP_PAL_OPEN 
  BOOL PastePalette();                                                       //WM_CAP_PAL_PASTE
  BOOL SavePalette(const CString& sFilename);                                //WM_CAP_PAL_SAVE
  
//Callback Functions
  BOOL EnableCapControlCallback();                                           //WM_CAP_SET_CALLBACK_CAPCONTROL
  BOOL RemoveCapControlCallback();                                           
  BOOL EnableErrorCallback();                                                //WM_CAP_SET_CALLBACK_ERROR 
  BOOL RemoveErrorCallback();                                                
  BOOL EnableFrameCallback();                                                //WM_CAP_SET_CALLBACK_FRAME   
  BOOL RemoveFrameCallback();                                                
  BOOL EnableStatusCallback();                                               //WM_CAP_SET_CALLBACK_STATUS
  BOOL RemoveStatusCallback();                                               
  BOOL EnableVideoStreamCallback();                                          //WM_CAP_SET_CALLBACK_VIDEOSTREAM 
  BOOL RemoveVideoStreamCallback();                                          
  BOOL EnableWaveStreamCallback();                                           //WM_CAP_SET_CALLBACK_WAVESTREAM 
  BOOL RemoveWaveStreamCallback();                                           
  BOOL EnableYieldCallback();                                                //WM_CAP_SET_CALLBACK_YIELD
  BOOL RemoveYieldCallback();                                                

//Debug / Assert help
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CAVICapWnd)
  //}}AFX_MSG

	DECLARE_MESSAGE_MAP()
  DECLARE_DYNAMIC(CAVICapWnd)

	//{{AFX_VIRTUAL(CAVICapWnd)
	//}}AFX_VIRTUAL

//Static Methods
  static LRESULT CALLBACK _Control(HWND hWnd, int nState);
  static LRESULT CALLBACK _Error(HWND hWnd, int nID, LPCTSTR lpsz);
  static LRESULT CALLBACK _Frame(HWND hWnd, LPVIDEOHDR lpVHdr);
  static LRESULT CALLBACK _Status(HWND hWnd, int nID, LPCTSTR lpsz);
  static LRESULT CALLBACK _VideoStream(HWND hWnd, LPVIDEOHDR lpVHdr);
  static LRESULT CALLBACK _WaveStream(HWND hWnd, LPWAVEHDR lpWHdr);
  static LRESULT CALLBACK _Yield(HWND hWnd);

//Virtual Functions
  virtual LRESULT OnControl(int nState);
  virtual LRESULT OnError(int nID, LPCTSTR lpsz);
  virtual LRESULT OnFrame(LPVIDEOHDR lpVHdr);
  virtual LRESULT OnStatus(int nID, LPCTSTR lpsz);
  virtual LRESULT OnVideoStream(LPVIDEOHDR lpVHdr);
  virtual LRESULT OnWaveStream(LPWAVEHDR lpWHdr  );
  virtual LRESULT OnYield();

//Member variables
};



#endif //__VFWWND_H__
