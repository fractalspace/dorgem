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
// CaptionBitmap.h : header file

#ifndef __CAPTIONBITMAP_H__
#define __CAPTIONBITMAP_H__

#define OVL_TYPE_MULTIPLY  0
#define OVL_TYPE_TRANS     1
#define OVL_TYPE_COPY      2

/////////////////////////////////////////////////////////////////////////////
//

class CCaptionBitmap : public CCaptionEvent
{
public:
  CCaptionBitmap();

  BOOL ReadRegistry(CRegKey*);
  BOOL WriteRegistry(CRegKey*);

  BOOL Overlay(HDC, int, int);

  CString   GetBitmap()                 { return(m_strBMP);         };
  int       GetOverlayType()            { return(m_iOverlayType);   };
  COLORREF  GetTransparentColor()       { return(m_colTransparent); };

  void SetBitmap(CString s)             { m_strBMP = s;             };
  void SetOverlayType(int i)            { m_iOverlayType = i;       };
  void SetTransparentColor(COLORREF c)  { m_colTransparent = c;     };

protected:

  void TranspBlt(HDC, int, int, int, int, HDC);

  CString  m_strBMP;
  int      m_iOverlayType;
  COLORREF m_colTransparent;

};

#endif // __CAPTIONBITMAP_H__
