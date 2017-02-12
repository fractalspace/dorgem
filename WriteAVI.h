/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2002 Frank Fesevur                                       //
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
// WriteAVI.h : header file

#ifndef __WRITEAVI_H__
#define __WRITEAVI_H__

class CWriteAVI : public CObject
{
public:
  CWriteAVI();
  virtual ~CWriteAVI();

  BOOL AddFrame(BITMAPINFOHEADER* pbihBMP, VOID* pBits);

  void ReadSettings();

  void SetFilename(LPCTSTR szFilename);
  void SetFramesPerSecond(UINT u)         { m_uFramesPerSecond = u; };
  void SetChangeDay(BOOL b)               { m_bChangeDay = b;       };

protected:
  void Close();

  BOOL          m_bUseAVI;          // Do we use the AVI archiving?
  CString       m_strFilename;      // Filename of the AVI file
  int           m_iFrame;           // Current Frame number
  BOOL          m_bOK;              // Is everything going OK
  PAVIFILE      m_pFile;            // The AVI file
  PAVISTREAM    m_pStream;          // The video stream
  PAVISTREAM    m_pCompressed;      // The compressed video stream
  UINT          m_uFramesPerSecond; // Number of frames per second
  int           m_iDay;             // Current day
  BOOL          m_bChangeDay;       // Change to a new file every day?
};

#endif // __WRITEAVI_H__
