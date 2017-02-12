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
// CaptionText.cpp : implementation file

#include "StdAfx.h"
#include "CaptionEvent.h"
#include "CaptionText.h"
#include "Dorgem.h"

#include "Tm.h"
#include "RegKey.h"
#include "RegistryEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

CCaptionText::CCaptionText()
{
  m_iType = CAPTION_TXT;
  m_plfFont = NULL;
  m_bAbsolute = FALSE;
  m_wLang = LANG_NEUTRAL;
  m_uMaxLength = 0;
  m_uRotate = 0;
  m_colBack = DEFAULT_BACK_COLOR;
  m_colFore = DEFAULT_FORE_COLOR;
  m_bTransparent = FALSE;
  m_uShadow = SHADOW_NONE;
}

CCaptionText::~CCaptionText()
{
  if (m_plfFont != NULL)
    delete [] m_plfFont;
}

/////////////////////////////////////////////////////////////////////////////
//

void CCaptionText::SetFont(LOGFONT* plfFont)
{
  // Resetting the font
  if (plfFont == NULL)
  {
    if (m_plfFont != NULL)
      delete [] m_plfFont;
    m_plfFont = NULL;
    return;
  }

  // Do we need to allocated memory?
  if (m_plfFont == NULL)
    m_plfFont = new LOGFONT;

  // Copy the LOGFONT
  CopyMemory(m_plfFont, plfFont, sizeof(LOGFONT));
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionText::ReadRegistry(CRegKey* pKey)
{
  ASSERT(pKey != NULL);

  TRACE("[CaptionText]ReadRegistry()\n");

  CCaptionEvent::ReadRegistry(pKey);

  m_strCaption = pKey->GetStringValue(szText);
  m_strFilename = pKey->GetStringValue(szFile);
  m_wLang = (WORD) pKey->GetDWORDValue(szLanguage, LANG_NEUTRAL);
  m_colBack = pKey->GetDWORDValue(szBackColor, DEFAULT_BACK_COLOR);
  m_colFore = pKey->GetDWORDValue(szForeColor, DEFAULT_FORE_COLOR);
  m_bTransparent = pKey->GetDWORDValue(szTransparent, FALSE);
  m_uShadow = pKey->GetDWORDValue(szShadow, SHADOW_NONE);
  m_uMaxLength = pKey->GetDWORDValue(szMaxLength, 0);
  m_uRotate = pKey->GetDWORDValue(szRotate, 0);

  // Get the font information from the registry. If a font already has
  // been loaded, first clean up that memory
  if (m_plfFont != NULL)
    delete [] m_plfFont;
  UINT uBytes;
  if (pKey->GetBinaryValue(szFont, (LPBYTE*) &m_plfFont, &uBytes))
    ASSERT(uBytes == sizeof(LOGFONT));
  else
    m_plfFont = NULL;

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CCaptionText::WriteRegistry(CRegKey* pKey)
{
  TRACE("[CCaptionText]WriteRegistry()\n");

  CCaptionEvent::WriteRegistry(pKey);

  // Store the information
  CRegKey reg;
  reg.Create(*pKey, m_strName);
  reg.SetStringValue(szText, m_strCaption);
  reg.SetStringValue(szFile, m_strFilename);
  reg.SetDWORDValue(szLanguage, m_wLang);
  reg.SetDWORDValue(szBackColor, m_colBack);
  reg.SetDWORDValue(szForeColor, m_colFore);
  reg.SetDWORDValue(szTransparent, m_bTransparent);
  reg.SetDWORDValue(szShadow, m_uShadow);
  reg.SetDWORDValue(szMaxLength, m_uMaxLength);
  reg.SetDWORDValue(szRotate, m_uRotate);

  if (m_plfFont != NULL)
    reg.SetBinaryValue(szFont, (LPBYTE) m_plfFont, sizeof(LOGFONT));

  reg.Close();

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// Put the caption text on the bitmap

BOOL CCaptionText::Overlay(HDC hDC, int iBmpX, int iBmpY)
{
  CString strText;
  if (m_strFilename.IsEmpty())
  {
    // Construct the time related text
    CTM tmNow;
    tmNow.SetLanguage(m_wLang);
    strText = tmNow.Format(m_strCaption);
  }
  else
  {
    // Read the first line of the text file
    CStdioFile file;
    if (file.Open(m_strFilename, CFile::modeRead))
    {
      file.ReadString(strText);
      file.Close();
    }
  }

  // Is the overlay text set?
  if (strText.IsEmpty())
    return(FALSE);

  // Maximize the length of the text
  if (m_uMaxLength != 0)
    strText = strText.Left(m_uMaxLength);

  // First of all, set the font
  HFONT hFont = NULL, hOldFont = NULL;
  if (m_plfFont != NULL)
  {
    // Specify the rotation angle
    m_plfFont->lfEscapement = m_plfFont->lfOrientation = m_uRotate * 10;

    // Create the font
    hFont = CreateFontIndirect(m_plfFont);
    ASSERT(hFont != NULL);
    hOldFont = (HFONT) SelectObject(hDC, hFont);
  }

  // Calculate the position
  int iLen = strText.GetLength();
  int iPosX = 0, iPosY = 0;
  UINT uAlign = 0;
  if (m_bAbsolute)
  {
    iPosX = m_iPosHor;
    iPosY = m_iPosVer;
  }
  else
  {
    switch (m_iPosHor)
    {
      case POS_LEFT:    iPosX = 0;          uAlign |= TA_LEFT;      break;
      case POS_CENTER:  iPosX = iBmpX / 2;  uAlign |= TA_CENTER;    break;
      case POS_RIGHT:   iPosX = iBmpX;      uAlign |= TA_RIGHT;     break;
    }
    switch (m_iPosVer)
    {
      case POS_TOP:     iPosY = 0;          uAlign |= TA_TOP;       break;
      case POS_CENTER:  iPosY = iBmpY / 2;  uAlign |= TA_BASELINE;  break;
      case POS_BOTTOM:  iPosY = iBmpY;      uAlign |= TA_BOTTOM;    break;
    }
  }
  SetTextAlign(hDC, uAlign);

  // Set the colors
  if (m_bTransparent)
  {
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, m_colBack);

    switch (m_uShadow)
    {
      case SHADOW_FULL:
      {
        int i, j, iArr[3] = { -1, 0, 1 };
        for (i = 0; i < 3; i++)
          for (j = 0; j < 3; j++)
            TextOut(hDC, iPosX + iArr[i], iPosY + iArr[j], strText, iLen);
        break;
      }
      case SHADOW_DROP:
        TextOut(hDC, iPosX + 1, iPosY + 1, strText, iLen);
        break;
    }
  }
  else
  {
    SetBkMode(hDC, OPAQUE);
    SetBkColor(hDC, m_colBack);
  }
  SetTextColor(hDC, m_colFore);

  // Finally draw the real text
  TextOut(hDC, iPosX, iPosY, strText, iLen);

  // Clean up the mess
  if (hFont != NULL)
  {
    SelectObject(hDC, hOldFont);
    DeleteObject(hFont);
  }

  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// pDC : pointer to your device-context
// str : the text
// rect: the rectangle
// nOptions: can be a combination of ETO_CLIPPED and ETO_OPAQUE
//           (see documentation of ExtTextOut for more details)

/*
void CCaptionText::DrawRotatedText(CDC* pDC, const CString str, CRect rect, UINT nOptions)
{
  // convert angle to radian
  double pi = 3.141592654;
  double radian = pi * 2 / 360 * m_uRotate;

  // get the center of a not-rotated text
  CSize TextSize = pDC->GetTextExtent(str);
  CPoint center;
  center.x = TextSize.cx / 2;
  center.y = TextSize.cy / 2;

  // now calculate the center of the rotated text
  CPoint rcenter;
  rcenter.x = long(cos(radian) * center.x - sin(radian) * center.y);
  rcenter.y = long(sin(radian) * center.x + cos(radian) * center.y);

  // finally draw the text and move it to the center of the rectangle
  pDC->SetTextAlign(TA_BASELINE);
  pDC->SetBkMode(TRANSPARENT);
  pDC->ExtTextOut(rect.left + rect.Width() / 2 - rcenter.x,
                  rect.top + rect.Height() / 2 + rcenter.y,
                  nOptions, rect, str, NULL);
}
*/
