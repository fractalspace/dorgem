///////////////////////////////////////////////////////////////////////////
//
// Tm.h
//   The definition of the CTM class, a wrapper class around the struct tm
//
// Copyright / Usage Details:
//
//   Written and Copyright by Frank Fesevur, 1995-2001
//
//   You are allowed to include the source code in any product
//   (commercial, shareware, freeware or otherwise) when your product
//   is released in binary form. You are allowed to modify the source
//   code in any way you want except you cannot modify the copyright
//   details at the top of each module. If you want to distribute source
//   code with your application, then you are only allowed to distribute
//   versions released by the author. This is to maintain a single
//   distribution point for the source code.
//
// Version History:
//
//   17-Aug-1995: First release
//   24-Mar-1996: Added SetTime and constructors for DosDate, FILETIME,
//                SYSTEMTIME
//   14-Jun-1997: Added a MFC compatible Serialize()
//   19-Nov-1997: Dropped support for WIN16 and DOS
//   06-Apr-1998: Added the 'GetLocaleInfo()' support for localized names
//                Therefore dropped own arrays with localized names for
//                month and day names
//   12-Nov-1999: Added FullYear()
//   03-Oct-2000: Fixed a bug in the '%a' and '%A' of the Format()
//   05-Nov-2000: Added Easter(), ToUTC(), ToLocal()
//   17-Nov-2000: Added CopyValues() to help the copy-routines
//   09-Jan-2001: Used system for 'T' and 't' formats, removed 'R', 'r'
//                and 'D' formats and added 'F' and 'f' formats.
//                Also added 'q' format.
//   06-May-2001: Release to the public as Version 2.0
//   17-Jul-2001: Fixed bug in DayName()
//   14-Sep-2001: Build a real IsDst(). GetUTCBias() is now based upon this.
//                Operators '==', '<', '>', '<=', '>=' are UTC-aware
//                Minor rebuild of Format and SPrintAdd
//   21-Oct-2001: Added bFull parameter to DayName() and MonthName()
//   13-Nov-2001: Added test to SetLanguage() when LANG_CURRENT is used, nothing happens
//   04-Dec-2001: Added '%p' and '%h' to Format()
//
// Note / Still to do:
//
//   - The Between() and Add() have never been really tested :-(
//   - '==', '<', '>', '<=', '>=' operators are not 'Between'-aware
//   - Calculations (and testing) of the other types of WeekNr's
//   - Improving Between - calculations
//
//   - The SetTime functions are based upon the mktime(). For this
//     function the tm_isdst of important, but it is only based upon
//     US DST rules. To change this, it requires a major restructuring of
//     the class. It must become based upon the SYSTEMTIME.
//

#ifndef __TM_H__
#define __TM_H__

#define January         0
#define February        1
#define March           2
#define April           3
#define May             4
#define June            5
#define July            6
#define August          7
#define September       8
#define October         9
#define November       10
#define December       11

#define Sunday          0
#define Monday          1
#define Tuesday         2
#define Wednesday       3
#define Thursday        4
#define Friday          5
#define Saturday        6

#define BETWEEN_CURRENT 0
#define BETWEEN_MONTHS  1
#define BETWEEN_DAYS    2
#define BETWEEN_HOURS   3
#define BETWEEN_MINS    4
#define BETWEEN_SECS    5

#define WEEK_JAN1       0 // Week containing 1-Jan
#define WEEK_FULL       1 // First full week following 1-Jan
#define WEEK_DAY4       2 // First week containing at least four days
#define WEEK_USE_SYSTEM 3

#define MAXWDAY         7
#define MAXMONTH       12
#define LANG_CURRENT 0xff

class CTM: public CObject
{
public:
  CTM();
  CTM(time_t);
  CTM(const CTM&);
  CTM(int, int = -1, int = -1);
  CTM(WORD, WORD);
  CTM(LPFILETIME);
  CTM(LPSYSTEMTIME);

  ~CTM();

  CTM& operator=(const CTM&);
  CTM& operator=(const time_t);
  CTM& operator=(const LPFILETIME);
  CTM& operator=(const LPSYSTEMTIME);

  int operator==(CTM) const;
  int operator!=(CTM Chk) const  { return( !(*this == Chk) ); };
  int operator<(CTM) const;
  int operator<=(CTM) const;
  int operator>=(CTM) const;
  int operator>(CTM) const;

  operator time_t() const        { return(m_lTime); };
  operator tm*() const           { return(m_pTM); };
  operator LPSYSTEMTIME() const  { return(m_stTime); };

  BOOL IsEmpty()          { return(m_lTime == 0L); };
  void Empty()            { SetTime(0L); };

  void GetSystemTime()    { SetTime(time(NULL)); };

  void SetDate(int, int = -1, int = -1);
  void SetTime(int, int, int = 0);
  void SetTime(time_t);
  void SetTime(WORD, WORD);
  void SetTime(LPFILETIME);
  void SetTime(LPSYSTEMTIME);

  int Hour()     { return(m_pTM->tm_hour);      };  // Hours        (0-23)
  int Min()      { return(m_pTM->tm_min);       };  // Minutes      (0-59)
  int Sec()      { return(m_pTM->tm_sec);       };  // Seconds      (0-59)
  int MDay()     { return(m_pTM->tm_mday);      };  // Day of month (1-31)
  int Day()      { return(m_pTM->tm_mday);      };  // Day of month (1-31)
  int Mon()      { return(m_pTM->tm_mon);       };  // Month        (0-11)
  int Month()    { return(m_pTM->tm_mon+1);     };  // Month        (1-12)
  int Year()     { return(m_pTM->tm_year);      };  // Year - 1900
  int FullYear() { return(m_pTM->tm_year+1900); };  // Year
  int WDay()     { return(m_pTM->tm_wday);      };  // Day of week  (Sunday=0)
  int YDay()     { return(m_pTM->tm_yday);      };  // Day of year  (0-365)

  BOOL IsDst();                   // Is DST active
  BOOL IsLeap();                  // Is it a leap-year
  void ToUTC();                   // Convert to UTC
  void ToLocal();                 // Convert to local time

  int First(int = Sunday);           // First 'iWDay (Sunday=0)' of current month
  int Last(int = Sunday);            // Last 'iWDay (Sunday=0)' of current month
  int WeekNr(int =WEEK_USE_SYSTEM);  // Weeknumber of the current date
  int DaysInMonth() { return(m_iDaysInMonth[m_pTM->tm_mon]); };

  void Add(long, int = BETWEEN_CURRENT);
  CTM  Next(long, int = BETWEEN_CURRENT);
  long Between(CTM, int = BETWEEN_CURRENT);  // Nr of 'Betw' between Cur and Other
  BOOL Between(CTM, CTM);         // Is the current time between two times
  CTM  Easter();                  // Easter of the current year
  long GetBetweenOffset(int);

  void SetLanguage(WORD);
  CString MonthName(int = -1, WORD = LANG_CURRENT, BOOL = TRUE); // The name of the month
  CString DayName(int = -1, WORD = LANG_CURRENT, BOOL = TRUE);   // The name of the weekday

  CString Format(LPCSTR);      // %d %m etc

  virtual void Serialize(CArchive&);

protected:
  time_t m_lTime;                // time_t member
  struct tm* m_pTM;              // struct tm member
  LPSYSTEMTIME m_stTime;         // SYSTEMTIME member
  int m_iDaysInMonth[MAXMONTH];  // Array containing the #days in the month
  int  m_iCurrentBetween;        // Current Between Type
  WORD m_wCurLang;               // Current Language
  LCID m_lcid;
  BOOL m_bUTCTime;               // Used UTC or LocalTime
  int  m_iFormatMode;

  void AllocVars();
  void CopyValues(const CTM&);
  char* Format(int);
  void GetTime(time_t, tm*);
  void InitMonthDays();
  void InitVars();
  void SPrintAdd(CString&, LPCSTR, ...);

  CString GetLocalize(LCTYPE, int = 0, WORD = LANG_CURRENT);
  long GetUTCBias();
  CTM DecodeRelativeFormat(SYSTEMTIME&);

};

#endif  // __TM_H__
