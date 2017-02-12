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
// WriteAVI.cpp : implementation file
//
// This class is based upon the WriteAVI.c sample of Microsoft.
//

#include "StdAfx.h"
#include "Dorgem.h"
#include "WriteAVI.h"

#include "Tm.h"

/////////////////////////////////////////////////////////////////////////////
//

CWriteAVI::CWriteAVI()
{
  m_pFile = NULL;
  m_pStream = NULL;
  m_pCompressed = NULL;
  m_bOK = TRUE;
  ReadSettings();

  CTM tmNow;
  m_iDay = tmNow.Day();

  if (HIWORD(VideoForWindowsVersion()) < 0x010A)
    m_bOK = FALSE;
  else
    AVIFileInit();
}

/////////////////////////////////////////////////////////////////////////////
//

CWriteAVI::~CWriteAVI()
{
  Close();

  if (HIWORD(VideoForWindowsVersion()) >= 0x010A)
    AVIFileExit();
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CWriteAVI::AddFrame(BITMAPINFOHEADER* pbihBMP, VOID* pBits)
{
  if (!m_bUseAVI)
    return(FALSE);

  if (!m_bOK)
    return(FALSE);

  // Is there a new day close the current file, so a new file can created
  CTM tmNow;
  if (m_iDay != tmNow.Day() && m_bChangeDay)
  {
    Close();
    m_iDay = tmNow.Day();
  }

  // Do we need to open the AVI file?
  if (m_pStream == NULL)
  {
    // Open the .AVI file
    if (AVIFileOpen(&m_pFile, tmNow.Format(m_strFilename), OF_WRITE | OF_CREATE, NULL) != AVIERR_OK)
    {
      m_bOK = FALSE;
      return(FALSE);
    }

    // Create the stream
    AVISTREAMINFO StreamInfo;
    ZeroMemory(&StreamInfo, sizeof(StreamInfo));
    StreamInfo.fccType = streamtypeVIDEO;
    StreamInfo.dwScale = 1;
    StreamInfo.dwRate = m_uFramesPerSecond;
    StreamInfo.dwSuggestedBufferSize = pbihBMP->biSizeImage;
    SetRect(&StreamInfo.rcFrame, 0, 0, (int) pbihBMP->biWidth, (int) pbihBMP->biHeight);

    if (AVIFileCreateStream(m_pFile, &m_pStream, &StreamInfo) != AVIERR_OK)
    {
      m_bOK = FALSE;
      return(FALSE);
    }

    // Set the format of the frame to DIB
    if (AVIStreamSetFormat(m_pStream, 0, pbihBMP, pbihBMP->biSize) != AVIERR_OK)
    {
      m_bOK = FALSE;
      return(FALSE);
    }

/*
    AVICOMPRESSOPTIONS opts;
    AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};

    ZeroMemory(&opts, sizeof(opts));
    if (!AVISaveOptions(NULL, 0, 1, &m_pStream, (LPAVICOMPRESSOPTIONS FAR *) &aopts))
    {
      m_bOK = FALSE;
      return(FALSE);
    }

    if (AVIMakeCompressedStream(&psCompressed, m_pStream, &opts, NULL) != AVIERR_OK)
    {
      m_bOK = FALSE;
      return(FALSE);
    }

    if (AVIStreamSetFormat(psCompressed, 0,
               alpbi[0],            // stream format
               alpbi[0]->biSize +   // format size
               alpbi[0]->biClrUsed * sizeof(RGBQUAD)) != AVIERR_OK)
    {
      m_bOK = FALSE;
      return(FALSE);
    }
*/

  }

  // Write the frame
  if (AVIStreamWrite(m_pStream, m_iFrame, 1, pBits, pbihBMP->biSizeImage, AVIIF_KEYFRAME, NULL, NULL) != AVIERR_OK)
  {
    m_bOK = FALSE;
    return(FALSE);
  }

  m_iFrame++;
  return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//

void CWriteAVI::Close()
{
  if (m_pStream != NULL)
  {
    AVIStreamClose(m_pStream);
    m_pStream = NULL;
  }

  if (m_pCompressed != NULL)
  {
    AVIStreamClose(m_pCompressed);
    m_pCompressed = NULL;
  }

  if (m_pFile != NULL)
  {
    AVIFileClose(m_pFile);
    m_pFile = NULL;
  }

  m_bOK = !m_strFilename.IsEmpty();
  m_iFrame = 0;
}

/////////////////////////////////////////////////////////////////////////////
//

void CWriteAVI::SetFilename(LPCTSTR szFilename)
{
  m_strFilename = szFilename;

  if (m_bOK)
    Close();
}

/////////////////////////////////////////////////////////////////////////////
//

void CWriteAVI::ReadSettings()
{
  // Set the filename
  CString strTmp = GetApp()->GetProfileString(szGeneral, szArchiveAVI);
  SetFilename(strTmp);

  // Set the other options
  m_bUseAVI = GetApp()->GetProfileInt(szGeneral, szUseAVI, FALSE);
  m_uFramesPerSecond = GetApp()->GetProfileInt(szGeneral, szFramesPerSecond, 1);
  m_bChangeDay = GetApp()->GetProfileInt(szGeneral, szAVIChangeInterval, FALSE);
}
