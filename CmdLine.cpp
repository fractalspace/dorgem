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
// Implementation of the CCmdLine

#include "StdAfx.h"
#include "Dorgem.h"
#include "CmdLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

CCmdLine::CCmdLine()
{
}

/////////////////////////////////////////////////////////////////////////////
//
// Dorgem recognizes these command line options:
//
//    /c:configname       use the specified config
//    /m                  start minimized
//

void CCmdLine::ParseParam(LPCTSTR szParam, BOOL bFlag, BOOL bLast)
{
  CString strParam = szParam;       // For easier string handling

  TRACE("CmdLine Parameter: %s %d %d\n", szParam, bFlag, bLast);

  if (bFlag)
  {
    if (strParam.IsEmpty())
      return;

    // handling of /c:configname, use the specified config
    if (strParam.Left(1).CompareNoCase("c") == 0)
    {
      int iIndex = strParam.Find(":");
      if (iIndex != -1)
      {
        CString strConfig = strParam.Mid(iIndex + 1);
        if (!strConfig.IsEmpty())
          GetApp()->SetConfig(strConfig);
      }
      return;
    }

    // Handling of /m, start minimized
    if (strParam.CompareNoCase("m") == 0)
    {
      GetApp()->m_bStartMinimized = TRUE;
      return;
    }
  }
}
