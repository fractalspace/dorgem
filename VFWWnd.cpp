/*
Module : VFWWnd.cpp
Purpose: Interface for an MFC class which encapsulates the "AviCap" window class
Created: PJN / 26-09-2000
History: None

Copyright (c) 2000 by PJ Naughter.  
All rights reserved.

*/

/////////////////////////////////  Includes  //////////////////////////////////

#include "stdafx.h"
#include "VFWWnd.h"



//////////////////////////////// Defines / Locals /////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




////////////////////////////// Implementation /////////////////////////////////

IMPLEMENT_DYNAMIC(CAVICapWnd, CWnd)

BEGIN_MESSAGE_MAP(CAVICapWnd, CWnd)
	//{{AFX_MSG_MAP(CAVICapWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAVICapWnd::CAVICapWnd() : CWnd()
{
}

CAVICapWnd::~CAVICapWnd()
{
}

#ifdef _DEBUG
void CAVICapWnd::AssertValid() const
{
  //Just call the base class
	CWnd::AssertValid();
}

void CAVICapWnd::Dump(CDumpContext& dc) const
{
  //Just call the base class
	CWnd::Dump(dc);
}
#endif //_DEBUG

BOOL CAVICapWnd::GetDescription(WORD nIndex, CString& sName, CString sVersion)
{
  TCHAR* pszName = sName.GetBuffer(_MAX_PATH);
  TCHAR* pszVersion = sVersion.GetBuffer(_MAX_PATH);
  BOOL bSuccess = ::capGetDriverDescription(nIndex, pszName, _MAX_PATH, pszVersion, _MAX_PATH);
  sName.ReleaseBuffer();
  sVersion.ReleaseBuffer();
  return bSuccess;
}

//These declarations are need since these functions are not declared in any publically
//available MFC header file
extern void AFXAPI AfxHookWindowCreate(CWnd* pWnd);
extern BOOL AFXAPI AfxUnhookWindowCreate();

BOOL CAVICapWnd::Create(DWORD dwStyle, const CRect& rect, CWnd* pParentWnd, UINT nID)
{
  ASSERT(pParentWnd);

	AfxHookWindowCreate(this);
	HWND hWnd = ::capCreateCaptureWindow(_T(""), dwStyle, rect.left, rect.top, rect.Width(), rect.Height(), 
                                         pParentWnd->GetSafeHwnd(), nID);

#ifdef _DEBUG
	if (hWnd == NULL)
	{
		TRACE1("Warning: AVICap Window creation failed: GetLastError returns 0x%8.8X\n",
			GetLastError());
	}
#endif

	if (!AfxUnhookWindowCreate())
		PostNcDestroy();        // cleanup if CreateWindowEx fails too soon

	if (hWnd == NULL)
		return FALSE;
	ASSERT(hWnd == m_hWnd); // should have been set in send msg hook
	return TRUE;
}

BOOL CAVICapWnd::Abort()
{
  return (BOOL) SendMessage(WM_CAP_ABORT);
}

BOOL CAVICapWnd::Connect(int nIndex)
{
  return (BOOL) SendMessage(WM_CAP_DRIVER_CONNECT, (WPARAM) nIndex);
}

BOOL CAVICapWnd::Disconnect()
{
  return (BOOL) SendMessage(WM_CAP_DRIVER_DISCONNECT);
}

BOOL CAVICapWnd::CaptureSequence()
{
  return (BOOL) SendMessage(WM_CAP_SEQUENCE);
}

BOOL CAVICapWnd::Stop()
{
  return (BOOL) SendMessage(WM_CAP_STOP);
}

BOOL CAVICapWnd::GetStatus(CAPSTATUS& status)
{
  return (BOOL) SendMessage(WM_CAP_GET_STATUS, (WPARAM) sizeof(status), (LPARAM) &status);
}

BOOL CAVICapWnd::GetCaps(CAPDRIVERCAPS& caps)
{
  return (BOOL) SendMessage(WM_CAP_DRIVER_GET_CAPS, (WPARAM) sizeof(caps), (LPARAM) &caps);
}

BOOL CAVICapWnd::GetName(CString& sName)
{
  TCHAR* pszName = sName.GetBuffer(_MAX_PATH);
  BOOL bSuccess = (BOOL) SendMessage(WM_CAP_DRIVER_GET_NAME, _MAX_PATH, (LPARAM) pszName);
  sName.ReleaseBuffer();
  return bSuccess;
}

BOOL CAVICapWnd::GetVersion(CString& sVersion)
{
  TCHAR* pszVersion = sVersion.GetBuffer(_MAX_PATH);
  BOOL bSuccess = (BOOL) SendMessage(WM_CAP_DRIVER_GET_NAME, _MAX_PATH, (LPARAM) pszVersion);
  sVersion.ReleaseBuffer();
  return bSuccess;
}

BOOL CAVICapWnd::SetOverlay(BOOL bOverlay)
{
  return (BOOL) SendMessage(WM_CAP_SET_OVERLAY, (WPARAM) bOverlay);
}

BOOL CAVICapWnd::SetPreview(BOOL bPreview)
{
  return (BOOL) SendMessage(WM_CAP_SET_PREVIEW, (WPARAM) bPreview);
}

BOOL CAVICapWnd::HasOverlay()
{
  CAPDRIVERCAPS caps;
  return GetCaps(caps) && caps.fHasOverlay;
}

BOOL CAVICapWnd::IsOverlay()
{
  CAPSTATUS status;
  return GetStatus(status) && status.fOverlayWindow;
}

BOOL CAVICapWnd::IsPreview()
{
  CAPSTATUS status;
  return GetStatus(status) && status.fLiveWindow;
}

BOOL CAVICapWnd::SetPreviewRate(WORD wPreviewRate)
{
  return (BOOL) SendMessage(WM_CAP_SET_PREVIEWRATE, (WPARAM) wPreviewRate);
}

BOOL CAVICapWnd::SetScale(BOOL bScale)
{
  return (BOOL) SendMessage(WM_CAP_SET_SCALE, (WPARAM) bScale);
}

BOOL CAVICapWnd::SetScroll(const CPoint& point)
{
  return (BOOL) SendMessage(WM_CAP_SET_SCROLL, (WPARAM) &point);
}

BOOL CAVICapWnd::HasVideoOutputDlg()
{
  CAPDRIVERCAPS caps;
  return GetCaps(caps) && caps.fHasDlgVideoDisplay;
}

BOOL CAVICapWnd::ShowVideoOutputDlg()
{
  return (BOOL) SendMessage(WM_CAP_DLG_VIDEODISPLAY);
}

BOOL CAVICapWnd::HasVideoFormatDlg()
{
  CAPDRIVERCAPS caps;
  return GetCaps(caps) && caps.fHasDlgVideoFormat;
}

BOOL CAVICapWnd::ShowVideoFormatDlg()
{
  return (BOOL) SendMessage(WM_CAP_DLG_VIDEOFORMAT);
}

BOOL CAVICapWnd::HasVideoSourceDlg()
{
  CAPDRIVERCAPS caps;
  return GetCaps(caps) && caps.fHasDlgVideoSource;
}

BOOL CAVICapWnd::ShowVideoSourceDlg()
{
  return (BOOL) SendMessage(WM_CAP_DLG_VIDEOSOURCE);
}

BOOL CAVICapWnd::ShowCompressionDlg()
{
  return (BOOL) SendMessage(WM_CAP_DLG_VIDEOCOMPRESSION);
}

BOOL CAVICapWnd::GetAudioFormat(WAVEFORMATEX*& pWaveFormat, DWORD& dwSize)
{
  BOOL bSuccess = FALSE;       //Assume the worst
  ASSERT(pWaveFormat == NULL); //Must be called with NULL pointer as we 
                               //will allocate the memory for it

  dwSize = (DWORD) SendMessage(WM_CAP_GET_AUDIOFORMAT);
  if (dwSize)
  {
    pWaveFormat = (WAVEFORMATEX*) new BYTE[dwSize];
    bSuccess = (SendMessage(WM_CAP_GET_AUDIOFORMAT, dwSize, (LPARAM) pWaveFormat) != 0);
  } 
  return bSuccess;
}

BOOL CAVICapWnd::SetAudioFormat(WAVEFORMATEX*	pWaveFormat)
{
  return (BOOL) SendMessage(WM_CAP_SET_AUDIOFORMAT, sizeof(WAVEFORMATEX), (LPARAM) pWaveFormat);
}

BOOL CAVICapWnd::SetAudioFormat(PCMWAVEFORMAT* pWaveFormat)
{
  return (BOOL) SendMessage(WM_CAP_SET_AUDIOFORMAT, sizeof(PCMWAVEFORMAT), (LPARAM) pWaveFormat);
}

BOOL CAVICapWnd::GetVideoFormat(BITMAPINFO*& pBitmapInfo)
{
  BOOL bSuccess = FALSE;       //Assume the worst
  ASSERT(pBitmapInfo == NULL); //Must be called with NULL pointer as we 
                               //will allocate the memory for it

  WORD wSize = (WORD) SendMessage(WM_CAP_GET_VIDEOFORMAT);
  if (wSize)
  {
    pBitmapInfo = (BITMAPINFO*) new BYTE[wSize];
    bSuccess = (SendMessage(WM_CAP_GET_VIDEOFORMAT, wSize, (LPARAM) pBitmapInfo) != 0);
  } 
  return bSuccess;
}

BOOL CAVICapWnd::SetVideoFormat(BITMAPINFO* pBitmapInfo)
{
  return (BOOL) SendMessage(WM_CAP_SET_VIDEOFORMAT, sizeof(BITMAPINFO), (LPARAM) pBitmapInfo); //may have a problem here
}

BOOL CAVICapWnd::FileAllocate(DWORD dwSize)
{
  return (BOOL) SendMessage(WM_CAP_FILE_ALLOCATE, 0, (LPARAM) dwSize);
}

BOOL CAVICapWnd::GetCaptureFile(CString& sFilename)
{
  TCHAR* pszFilename = sFilename.GetBuffer(_MAX_PATH);
  BOOL bSuccess = (BOOL) SendMessage(WM_CAP_FILE_GET_CAPTURE_FILE, _MAX_PATH, (LPARAM) pszFilename);
  sFilename.ReleaseBuffer();
  return bSuccess;
}

BOOL CAVICapWnd::SaveAs(const CString& sFilename)
{
  return (BOOL) SendMessage(WM_CAP_FILE_SAVEAS, 0, (LPARAM) (LPCTSTR) sFilename);
}

BOOL CAVICapWnd::SetCaptureFile(const CString& sFilename)
{
  return (BOOL) SendMessage(WM_CAP_FILE_SET_CAPTURE_FILE, 0, (LPARAM) (LPCTSTR) sFilename);
}

BOOL CAVICapWnd::CaptureNoFile()
{
  return (BOOL) SendMessage(WM_CAP_SEQUENCE_NOFILE);
}

BOOL CAVICapWnd::SetMCIDevice(const CString& sName)
{
  return (BOOL) SendMessage(WM_CAP_SET_MCI_DEVICE, 0, (LPARAM) (LPCTSTR) sName);
}

BOOL CAVICapWnd::CaptureSingleFrame()
{
  return (BOOL) SendMessage(WM_CAP_SINGLE_FRAME);
}

BOOL CAVICapWnd::CaptureSingleFrameOpen()
{
  return (BOOL) SendMessage(WM_CAP_SINGLE_FRAME_OPEN);
}

BOOL CAVICapWnd::CaptureSingleFrameClose()
{
  return (BOOL) SendMessage(WM_CAP_SINGLE_FRAME_CLOSE);
}

BOOL CAVICapWnd::EditCopy()
{
  return (BOOL) SendMessage(WM_CAP_EDIT_COPY);
}

BOOL CAVICapWnd::SaveDib(const CString& sFilename)
{
  return (BOOL) SendMessage(WM_CAP_FILE_SAVEDIB, 0, (LPARAM) (LPCTSTR) sFilename);
}

BOOL CAVICapWnd::GrabFrame()
{
  return (BOOL) SendMessage(WM_CAP_GRAB_FRAME);  
}

BOOL CAVICapWnd::GrabFrameNoStop()
{
  return (BOOL) SendMessage(WM_CAP_GRAB_FRAME_NOSTOP);  
}

BOOL CAVICapWnd::SetInfoChunk(const CAPINFOCHUNK* pInfoChunk)
{
  return (BOOL) SendMessage(WM_CAP_FILE_SET_INFOCHUNK, 0, (LPARAM) pInfoChunk);  
}

DWORD CAVICapWnd::GetUserData()
{
  return (DWORD) SendMessage(WM_CAP_GET_USER_DATA);
}

BOOL CAVICapWnd::SetUserData(DWORD dwData)
{
  return (BOOL) SendMessage(WM_CAP_GET_USER_DATA, 0, (LPARAM) dwData);  
}

BOOL CAVICapWnd::AutoCreatePalette(int nFrames, int nColors)
{
  return (BOOL) SendMessage(WM_CAP_PAL_AUTOCREATE, (WPARAM) nFrames, (LPARAM) nColors);  
}

BOOL CAVICapWnd::ManualCreatePalette(BOOL bGrab, int nColors)
{
  return (BOOL) SendMessage(WM_CAP_PAL_AUTOCREATE, (WPARAM) bGrab, (LPARAM) nColors);  
}

BOOL CAVICapWnd::OpenPalette(const CString& sFilename)
{
  return (BOOL) SendMessage(WM_CAP_PAL_OPEN, 0, (LPARAM) (LPCTSTR) sFilename);
}

BOOL CAVICapWnd::PastePalette()
{
  return (BOOL) SendMessage(WM_CAP_PAL_PASTE);
}

BOOL CAVICapWnd::SavePalette(const CString& sFilename)
{
  return (BOOL) SendMessage(WM_CAP_PAL_SAVE, 0, (LPARAM) (LPCTSTR) sFilename);
}

BOOL CAVICapWnd::GetSetup(CAPTUREPARMS& params)
{
  return (BOOL) SendMessage(WM_CAP_GET_SEQUENCE_SETUP, (WPARAM) sizeof(params), (LPARAM) &params);  
}

BOOL CAVICapWnd::SetSetup(CAPTUREPARMS& params)
{
  return (BOOL) SendMessage(WM_CAP_SET_SEQUENCE_SETUP, (WPARAM) sizeof(params), (LPARAM) &params);  
}

LRESULT CAVICapWnd::OnControl(int nState)
{
  //Just TRACE the message, derived classes can do something more interesting
  TRACE(_T("Received CAPCONTROL callback, State=%d\n"), nState);
  return (LRESULT) TRUE;
}

LRESULT CALLBACK CAVICapWnd::_Control(HWND hWnd, int nState)
{
  //Convert from the SDK world to the C++ world
  CAVICapWnd* pWnd = (CAVICapWnd*) CWnd::FromHandle(hWnd);
  ASSERT(pWnd);
  ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CAVICapWnd)));

  //Call the member function
  return pWnd->OnControl(nState);
}

BOOL CAVICapWnd::EnableCapControlCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_CAPCONTROL, 0, (LPARAM) _Control);
}

BOOL CAVICapWnd::RemoveCapControlCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_CAPCONTROL, 0, NULL);
}

LRESULT CAVICapWnd::OnError(int nID, LPCTSTR lpsz)
{
  //Just TRACE the message, derived classes can do something more interesting
  TRACE(_T("Received Error callback, ID=%d, String=%s\n"), nID, lpsz);
  return (LRESULT) TRUE;
}

LRESULT CALLBACK CAVICapWnd::_Error(HWND hWnd, int nID, LPCTSTR lpsz)
{
  //Convert from the SDK world to the C++ world
  CAVICapWnd* pWnd = (CAVICapWnd*) CWnd::FromHandle(hWnd);
  ASSERT(pWnd);
  ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CAVICapWnd)));

  //Call the member function
  return pWnd->OnError(nID, lpsz);
}

BOOL CAVICapWnd::EnableErrorCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_ERROR, 0, (LPARAM) _Error);
}

BOOL CAVICapWnd::RemoveErrorCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_ERROR, 0, NULL);
}

LRESULT CAVICapWnd::OnFrame(LPVIDEOHDR /*lpVHdr*/)
{
  //Just TRACE the message, derived classes can do something more interesting
  TRACE(_T("Received Frame callback\n"));
  return (LRESULT) TRUE;
}

LRESULT CALLBACK CAVICapWnd::_Frame(HWND hWnd, LPVIDEOHDR lpVHdr)
{
  //Convert from the SDK world to the C++ world
  CAVICapWnd* pWnd = (CAVICapWnd*) CWnd::FromHandle(hWnd);
  ASSERT(pWnd);
  ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CAVICapWnd)));

  //Call the member function
  return pWnd->OnFrame(lpVHdr);
}

BOOL CAVICapWnd::EnableFrameCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_FRAME, 0, (LPARAM) _Frame);
}

BOOL CAVICapWnd::RemoveFrameCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_FRAME, 0, NULL);
}

LRESULT CAVICapWnd::OnStatus(int nID, LPCTSTR lpsz)
{
  //Just TRACE the message, derived classes can do something more interesting
  TRACE(_T("Received Status callback, ID=%d, String=%s\n"), nID, lpsz);
  return (LRESULT) TRUE;
}

LRESULT CALLBACK CAVICapWnd::_Status(HWND hWnd, int nID, LPCTSTR lpsz)
{
  //Convert from the SDK world to the C++ world
  CAVICapWnd* pWnd = (CAVICapWnd*) CWnd::FromHandle(hWnd);
  ASSERT(pWnd);
  ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CAVICapWnd)));

  //Call the member function
  return pWnd->OnStatus(nID, lpsz);
}

BOOL CAVICapWnd::EnableStatusCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_STATUS, 0, (LPARAM) _Status);
}

BOOL CAVICapWnd::RemoveStatusCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_STATUS, 0, NULL);
}

LRESULT CAVICapWnd::OnVideoStream(LPVIDEOHDR /*lpVHdr*/)
{
  //Just TRACE the message, derived classes can do something more interesting
  TRACE(_T("Received VideoStream callback\n"));
  return (LRESULT) TRUE;
}

LRESULT CALLBACK CAVICapWnd::_VideoStream(HWND hWnd, LPVIDEOHDR lpVHdr)
{
  //Convert from the SDK world to the C++ world
  CAVICapWnd* pWnd = (CAVICapWnd*) CWnd::FromHandle(hWnd);
  ASSERT(pWnd);
  ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CAVICapWnd)));

  //Call the member function
  return pWnd->OnVideoStream(lpVHdr);
}

BOOL CAVICapWnd::EnableVideoStreamCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_VIDEOSTREAM, 0, (LPARAM) _VideoStream);
}

BOOL CAVICapWnd::RemoveVideoStreamCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_VIDEOSTREAM, 0, NULL);
}

LRESULT CAVICapWnd::OnWaveStream(LPWAVEHDR /*lpWHdr*/)
{
  //Just TRACE the message, derived classes can do something more interesting
  TRACE(_T("Received WaveStream callback\n"));
  return (LRESULT) TRUE;
}

LRESULT CALLBACK CAVICapWnd::_WaveStream(HWND hWnd, LPWAVEHDR lpWHdr)
{
  //Convert from the SDK world to the C++ world
  CAVICapWnd* pWnd = (CAVICapWnd*) CWnd::FromHandle(hWnd);
  ASSERT(pWnd);
  ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CAVICapWnd)));

  //Call the member function
  return pWnd->OnWaveStream(lpWHdr);
}

BOOL CAVICapWnd::EnableWaveStreamCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_WAVESTREAM, 0, (LPARAM) _WaveStream);
}

BOOL CAVICapWnd::RemoveWaveStreamCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_WAVESTREAM, 0, NULL);
}

LRESULT CAVICapWnd::OnYield()
{
  //Just TRACE the message, derived classes can do something more interesting
  TRACE(_T("Received Yield callback\n"));
  return (LRESULT) TRUE;
}

LRESULT CALLBACK CAVICapWnd::_Yield(HWND hWnd)
{
  //Convert from the SDK world to the C++ world
  CAVICapWnd* pWnd = (CAVICapWnd*) CWnd::FromHandle(hWnd);
  ASSERT(pWnd);
  ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CAVICapWnd)));

  //Call the member function
  return pWnd->OnYield();
}

BOOL CAVICapWnd::EnableYieldCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_YIELD, 0, (LPARAM) _Yield);
}

BOOL CAVICapWnd::RemoveYieldCallback()
{
  return (BOOL) SendMessage(WM_CAP_SET_CALLBACK_YIELD, 0, NULL);
}

