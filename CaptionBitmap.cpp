/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2001 Frank Fesevur                                       //
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
// CaptionBitmap.cpp : implementation file

#include "StdAfx.h"
#include "CaptionEvent.h"
#include "CaptionBitmap.h"
#include "Dorgem.h"

#include "RegKey.h"
#include "RegistryEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

CCaptionBitmap::CCaptionBitmap()
{
  m_iType = CAPTION_BMP;
  m_colTransparent = RGB(255,255,255);
  m_iOverlayType = OVL_TYPE_TRANS;
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionBitmap::ReadRegistry(CRegKey* pKey)
{
  ASSERT(pKey != NULL);

  TRACE("[CaptionBitmap]ReadRegistry()\n");

  CCaptionEvent::ReadRegistry(pKey);

  m_strBMP = pKey->GetStringValue(szFile);
  m_iOverlayType = pKey->GetDWORDValue(szOverlayType, OVL_TYPE_TRANS);
  m_colTransparent = pKey->GetDWORDValue(szTransparent, RGB(255,255,255));

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionBitmap::WriteRegistry(CRegKey* pKey)
{
  TRACE("[CCaptionBitmap]WriteRegistry()\n");

  CCaptionEvent::WriteRegistry(pKey);

  // Store the information
  CRegKey reg;
  reg.Create(*pKey, m_strName);
  reg.SetStringValue(szFile, m_strBMP);
  reg.SetDWORDValue(szOverlayType, m_iOverlayType);
  reg.SetDWORDValue(szTransparent, m_colTransparent);
  reg.Close();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionBitmap::Overlay(HDC hDC, int iBmpX, int iBmpY)
{
  // Load the bitmap
  if (m_strBMP.IsEmpty())
    return(FALSE);

  HBITMAP hBMP = (HBITMAP) LoadImage(NULL, m_strBMP, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
  if (!hBMP)
    return(FALSE);

  HDC hdcMemory = CreateCompatibleDC(hDC);
  HBITMAP hbmpTmp = (HBITMAP) SelectObject(hdcMemory, hBMP);

  // Get the bitmap information
  BITMAP bmp;
  int iRet = GetObject(hBMP, sizeof(BITMAP), &bmp);
  ASSERT(iRet != 0);

  // Calculate the position
  int iPosX = 0, iPosY = 0;
  if (m_bAbsolute)
  {
    iPosX = m_iPosHor;
    iPosY = m_iPosVer;
  }
  else
  {
    switch (m_iPosHor)
    {
      case POS_LEFT:    iPosX = 0;                         break;
      case POS_CENTER:  iPosX = (iBmpX - bmp.bmWidth) / 2; break;
      case POS_RIGHT:   iPosX = iBmpX - bmp.bmWidth;       break;
    }
    switch (m_iPosVer)
    {
      case POS_TOP:     iPosY = 0;                          break;
      case POS_CENTER:  iPosY = (iBmpY - bmp.bmHeight) / 2; break;
      case POS_BOTTOM:  iPosY = iBmpY - bmp.bmHeight;       break;
    }
  }

  // BitBlt it
  DWORD dwOp = 0;
  switch (m_iOverlayType)
  {
    case OVL_TYPE_TRANS:
      TranspBlt(hDC, iPosX, iPosY, bmp.bmWidth, bmp.bmHeight, hdcMemory);
      break;
    case OVL_TYPE_MULTIPLY: dwOp = SRCAND;  break;
    case OVL_TYPE_COPY:     dwOp = SRCCOPY; break;
  }
  if (dwOp != 0)
    BitBlt(hDC, iPosX, iPosY, bmp.bmWidth, bmp.bmHeight, hdcMemory, 0, 0, dwOp);

  // Clean up the mess
  SelectObject(hdcMemory, hbmpTmp);
  DeleteDC(hdcMemory);
  DeleteObject(hBMP);
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Do a transparent BitBlt
// This code is based upon a VB sample found somewhere on the Internet

void CCaptionBitmap::TranspBlt(HDC hdcDest, int iX, int iY,
                               int iWidth, int iHeight, HDC hdcSrc)
{
  // Create monochrome mask and inverse masks
  HDC hdcMonoMask = CreateCompatibleDC(hdcDest);
  HDC hdcMonoInv = CreateCompatibleDC(hdcDest);
  HBITMAP hMonoMask = CreateBitmap(iWidth, iHeight, 1, 1, NULL);
  HBITMAP hMonoInv = CreateBitmap(iWidth, iHeight, 1, 1, NULL);
  HBITMAP hPrevMask = (HBITMAP) SelectObject(hdcMonoMask, hMonoMask);
  HBITMAP hPrevInv = (HBITMAP) SelectObject(hdcMonoInv, hMonoInv);

  // Create keeper DCs and bitmaps
  HDC hdcResultDst = CreateCompatibleDC(hdcDest);
  HDC hdcResultSrc = CreateCompatibleDC(hdcDest);
  HBITMAP hResultDst = CreateCompatibleBitmap(hdcDest, iWidth, iHeight);
  HBITMAP hResultSrc = CreateCompatibleBitmap(hdcDest, iWidth, iHeight);
  HBITMAP hPrevDst = (HBITMAP) SelectObject(hdcResultDst, hResultDst);
  HBITMAP hPrevSrc = (HBITMAP) SelectObject(hdcResultSrc, hResultSrc);

  // Copy src to monochrome mask
  COLORREF colBackOld = SetBkColor(hdcSrc, m_colTransparent);
  BitBlt(hdcMonoMask, 0, 0, iWidth, iHeight, hdcSrc, 0, 0, SRCCOPY);
  SetBkColor(hdcSrc, colBackOld);

  // Create inverse of mask
  BitBlt(hdcMonoInv, 0, 0, iWidth, iHeight, hdcMonoMask, 0, 0, NOTSRCCOPY);

  // Get background
  BitBlt(hdcResultDst, 0, 0, iWidth, iHeight, hdcDest, iX, iY, SRCCOPY);

  // AND with Monochrome mask
  BitBlt(hdcResultDst, 0, 0, iWidth, iHeight, hdcMonoMask, 0, 0, SRCAND);

  // Get overlapper
  BitBlt(hdcResultSrc, 0, 0, iWidth, iHeight, hdcSrc, 0, 0, SRCCOPY);

  // AND with inverse monochrome mask
  BitBlt(hdcResultSrc, 0, 0, iWidth, iHeight, hdcMonoInv, 0, 0, SRCAND);

  // XOR these two
  BitBlt(hdcResultDst, 0, 0, iWidth, iHeight, hdcResultSrc, 0, 0, SRCINVERT);

  // Output results
  BitBlt(hdcDest, iX, iY, iWidth, iHeight, hdcResultDst, 0, 0, SRCCOPY);

  // Clean up
  hMonoMask = (HBITMAP) SelectObject(hdcMonoMask, hPrevMask);
  DeleteObject(hMonoMask);

  hMonoInv = (HBITMAP) SelectObject(hdcMonoInv, hPrevInv);
  DeleteObject(hMonoInv);

  hResultDst = (HBITMAP) SelectObject(hdcResultDst, hPrevDst);
  DeleteObject(hResultDst);

  hResultSrc = (HBITMAP) SelectObject(hdcResultSrc, hPrevSrc);
  DeleteObject(hResultSrc);

  DeleteDC(hdcMonoMask);
  DeleteDC(hdcMonoInv);
  DeleteDC(hdcResultDst);
  DeleteDC(hdcResultSrc);
}
