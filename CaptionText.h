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
// CaptionText.h : header file

#ifndef __CAPTIONTEXT_H__
#define __CAPTIONTEXT_H__

#define DEFAULT_FORE_COLOR 0xffffff
#define DEFAULT_BACK_COLOR 0x000000

#define SHADOW_NONE   0
#define SHADOW_FULL   1
#define SHADOW_DROP   2

/////////////////////////////////////////////////////////////////////////////
//

class CCaptionText : public CCaptionEvent
{
public:
  CCaptionText();
  ~CCaptionText();

  BOOL ReadRegistry(CRegKey*);
  BOOL WriteRegistry(CRegKey*);

  BOOL Overlay(HDC, int, int);

  CString  GetCaption()       { return(m_strCaption);   };
  CString  GetFilename()      { return(m_strFilename);  };
  WORD     GetLang()          { return(m_wLang);        };
  LOGFONT* GetFont()          { return(m_plfFont);      };
  COLORREF GetBackColor()     { return(m_colBack);      };
  COLORREF GetForeColor()     { return(m_colFore);      };
  BOOL     GetTranparent()    { return(m_bTransparent); };
  UINT     GetShadow()        { return(m_uShadow);      };
  UINT     GetMaxLength()     { return(m_uMaxLength);   };

  void SetCaption(CString s)       { m_strCaption = s;  };
  void SetFilename(CString s)      { m_strFilename = s; };
  void SetLang(WORD w)             { m_wLang = w;       };
  void SetBackColor(COLORREF col)  { m_colBack = col;   };
  void SetForeColor(COLORREF col)  { m_colFore = col;   };
  void SetTranparent(BOOL b)       { m_bTransparent = b;};
  void SetShadow(UINT u)           { m_uShadow = u;     };
  void SetMaxLength(UINT u)        { m_uMaxLength = u;  };
  void SetFont(LOGFONT*);

protected:

  CString  m_strCaption;        // The caption text (can contain % tags)
  WORD     m_wLang;             // The language
  LOGFONT* m_plfFont;           // The font
  COLORREF m_colBack;           // The background color
  COLORREF m_colFore;           // The text color
  BOOL     m_bTransparent;      // Is the text drawn transparent
  UINT     m_uShadow;           // The type of shadow
  UINT     m_uRotate;           // The rotation angle of the text (in whole degrees)
  UINT     m_uMaxLength;        // The maximun length of the text (0 = no limit)
  CString  m_strFilename;       // Filename of the text file to read the caption from

};

#endif // __CAPTIONTEXT_H__
