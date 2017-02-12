/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Dorgem - Webcam Capture application                                    //
//  Copyright (C) 2001-2003 Frank Fesevur                                  //
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
// StoreEvent.h : header file

#ifndef __STOREEVENT_H__
#define __STOREEVENT_H__

#define STORE_NONE    0   // Be carefull changing these numbers!
#define STORE_FILE    1   // They are saved in the registry
#define STORE_FTP     2
#define STORE_EMAIL   3
#define STORE_PROGRAM 4
#define STORE_HTTP    5

#define LOG_ERROR     0
#define LOG_INFO      3
#define LOG_DEBUG     5

#define DEFAULT_INTERVAL  30

#include "DorgemEvent.h"

class CRegKey;
class CCaptionManager;

/////////////////////////////////////////////////////////////////////////////
//

class CStoreEvent : public CDorgemEvent
{
public:
  CStoreEvent();
  virtual ~CStoreEvent();

  virtual BOOL NeedStore();
  virtual BOOL ReadRegistry(CRegKey*);
  virtual BOOL WriteRegistry(CRegKey*);

  // Pure virtual function
  virtual BOOL Store() = 0;

  BOOL    IsBusy()                { return(m_bBusy);              };
  BOOL    HasCaptions()           { return(m_pCaptions != NULL);  };
  time_t  GetInterval()           { return(m_tInterval);          };
  LPCSTR  GetErrorMsg()           { return(m_strErrorMsg);        };
  UINT    GetLogLevel()           { return(m_uLogLevel);          };
  CString GetRegCaption()         { return(m_strRegCaption);      };
  BOOL    ResetTime()             { return(m_bResetTime);         };
  void    GetInterval(time_t&, int&);
  void    DeleteCaptions();
  CCaptionManager* GetCaptions();

  void SetOrigFile(LPCSTR sz)           { m_strOrigFile = sz;     };
  void SetInterval(time_t t)            { m_tInterval = t;        };
  void SetInterval(int, int);
  void SetEnabled(BOOL b)               { m_bEnable = b;          };
  void SetLogLevel(UINT u)              { m_uLogLevel = u;        };
  void SetRegCaption(LPCSTR sz)         { m_strRegCaption = sz;   };
  void SetCaptions(CCaptionManager* p)  { m_pCaptions = p;        };
  void SetLogFile(CString, BOOL =FALSE);

  void ResetTime(BOOL b)                { m_bResetTime = b;       };
  void ResetTimer();

protected:

  void Log(UINT, LPCSTR);
  BOOL OpenLogFile(BOOL);

  BOOL             m_bBusy;         // Is this event busy
  time_t           m_tLastUpdate;   // When was the file last stored
  BOOL             m_bResetTime;    // Reset time if storage failes
  time_t           m_tInterval;     // What is the interval (in seconds)
  CString          m_strOrigFile;   // The temporary JPEG file
  CString          m_strErrorMsg;   // The last error message
  CString          m_strLogFile;    // The name of the log file
  UINT             m_uLogLevel;     // Current level of logging
  CStdioFile       m_fileLog;       // Log file
  CCaptionManager* m_pCaptions;     // The captions for this event
  CString          m_strRegCaption; // The registry path to the captions

};

void AFXAPI DDV_StoreEventUnique(CDataExchange* pDX, CString const& strValue);

#endif // __STOREEVENT_H__
