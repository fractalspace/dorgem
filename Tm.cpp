///////////////////////////////////////////////////////////////////////////
// Tm.cpp: The implementation of the CTM class
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

//
// *** Added some line for Dorgem to implement %G %g %P %V %W ***
//

#include "StdAfx.h"
#include "Tm.h"

#include "Resource.h"  // *** Added for Dorgem ***

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////
//

void CTM::Serialize(CArchive& archive)
{
  CObject::Serialize(archive);

  // Now dump data for this class
  if (archive.IsStoring())
  {
    archive << m_lTime;
    archive << m_iCurrentBetween;
    archive << m_wCurLang;
    archive << m_bUTCTime;
  }
  else
  {
    time_t lTime;
    int iBetween;
    WORD wCurLang;
    BOOL bUTCTime;

    archive >> lTime;
    archive >> iBetween;
    archive >> wCurLang;
    archive >> bUTCTime;

    m_iCurrentBetween = iBetween;
    SetLanguage(wCurLang);
    m_bUTCTime = bUTCTime;
    SetTime(lTime);
  }
}

///////////////////////////////////////////////////////////////////////////
// Initialize the member-variables. Used by constructors

void CTM::InitVars(void)
{
  m_iCurrentBetween = BETWEEN_SECS;
  SetLanguage(LANG_NEUTRAL);
  m_bUTCTime = FALSE;
}

///////////////////////////////////////////////////////////////////////////
//

void CTM::AllocVars(void)
{
  m_pTM = new tm;
  m_stTime = new SYSTEMTIME;
}

///////////////////////////////////////////////////////////////////////////
// Initialize the DaysInMonth-array

void CTM::InitMonthDays(void)
{
  m_iDaysInMonth[January] = 31;
  m_iDaysInMonth[February] = (IsLeap() ? 29 : 28);
  m_iDaysInMonth[March] = 31;
  m_iDaysInMonth[April] = 30;
  m_iDaysInMonth[May] = 31;
  m_iDaysInMonth[June] = 30;
  m_iDaysInMonth[July] = 31;
  m_iDaysInMonth[August] = 31;
  m_iDaysInMonth[September] = 30;
  m_iDaysInMonth[October] = 31;
  m_iDaysInMonth[November] = 30;
  m_iDaysInMonth[December] = 31;
}

///////////////////////////////////////////////////////////////////////////
//

void CTM::CopyValues(const CTM& Copy)
{
  m_iCurrentBetween = Copy.m_iCurrentBetween;
  SetLanguage(Copy.m_wCurLang);
  m_bUTCTime = Copy.m_bUTCTime;
  SetTime(Copy.m_lTime);
}

///////////////////////////////////////////////////////////////////////////
// Constructor

CTM::CTM()
{
  AllocVars();
  GetSystemTime();
  InitVars();
}

///////////////////////////////////////////////////////////////////////////
// Copy-Constructor.

CTM::CTM(const CTM& Copy)
{
  AllocVars();
  CopyValues(Copy);
}

///////////////////////////////////////////////////////////////////////////
// Constructor to set time

CTM::CTM(time_t lTime)
{
  AllocVars();
  SetTime(lTime);
  InitVars();
}

///////////////////////////////////////////////////////////////////////////
// Constructor to set date

CTM::CTM(int iDay, int iMonth, int iYear)
{
  AllocVars();
  GetSystemTime();
  SetDate(iDay, iMonth, iYear);
  InitVars();
}

///////////////////////////////////////////////////////////////////////////
// Constructor to set DOS-time

CTM::CTM(WORD wDosDate, WORD wDosTime)
{
  AllocVars();
  SetTime(wDosDate, wDosTime);
  InitVars();
}

///////////////////////////////////////////////////////////////////////////
// Constructor to set Win32 - FILETIME

CTM::CTM(LPFILETIME ftTime)
{
  AllocVars();
  SetTime(ftTime);
  InitVars();
}

///////////////////////////////////////////////////////////////////////////
// Constructor to set Win32 - SYSTEMTIME

CTM::CTM(LPSYSTEMTIME stTime)
{
  AllocVars();
  SetTime(stTime);
  InitVars();
}

///////////////////////////////////////////////////////////////////////////
// Destructor - Cleaning up the memory used by the class

CTM::~CTM()
{
  delete m_pTM;
  delete m_stTime;
}

///////////////////////////////////////////////////////////////////////////
// Overloads of the '=' operator

CTM& CTM::operator=(const CTM& Copy)
{
  CopyValues(Copy);
  return(*this);
}

CTM& CTM::operator=(const time_t tTime)
{
  SetTime(tTime);
  return(*this);
}

CTM& CTM::operator=(const LPFILETIME ftTime)
{
  SetTime(ftTime);
  return(*this);
}

CTM& CTM::operator=(const LPSYSTEMTIME stTime)
{
  SetTime(stTime);
  return(*this);
}

///////////////////////////////////////////////////////////////////////////
// Overload of '==' operator

int CTM::operator==(CTM ChkTime) const
{
  if (m_bUTCTime == ChkTime.m_bUTCTime)
    return(m_lTime == ChkTime.m_lTime);

  CTM tmThis = *this;
  tmThis.ToUTC();
  CTM tmCheck = ChkTime;
  tmCheck.ToUTC();
  return(tmThis.m_lTime == tmCheck.m_lTime);
}

///////////////////////////////////////////////////////////////////////////
// Overload of '<' operator

int CTM::operator<(CTM ChkTime) const
{
  if (m_bUTCTime == ChkTime.m_bUTCTime)
    return(m_lTime < ChkTime.m_lTime);

  CTM tmThis = *this;
  tmThis.ToUTC();
  CTM tmCheck = ChkTime;
  tmCheck.ToUTC();
  return(tmThis.m_lTime < tmCheck.m_lTime);
}

///////////////////////////////////////////////////////////////////////////
// Overload of '<=' operator

int CTM::operator<=(CTM ChkTime) const
{
  if (m_bUTCTime == ChkTime.m_bUTCTime)
    return(m_lTime <= ChkTime.m_lTime);

  CTM tmThis = *this;
  tmThis.ToUTC();
  CTM tmCheck = ChkTime;
  tmCheck.ToUTC();
  return(tmThis.m_lTime <= tmCheck.m_lTime);
}

///////////////////////////////////////////////////////////////////////////
// Overload of '>=' operator

int CTM::operator>=(CTM ChkTime) const
{
  if (m_bUTCTime == ChkTime.m_bUTCTime)
    return(m_lTime >= ChkTime.m_lTime);

  CTM tmThis = *this;
  tmThis.ToUTC();
  CTM tmCheck = ChkTime;
  tmCheck.ToUTC();
  return(tmThis.m_lTime >= tmCheck.m_lTime);
}

///////////////////////////////////////////////////////////////////////////
// Overload of '>' operator

int CTM::operator>(CTM ChkTime) const
{
  if (m_bUTCTime == ChkTime.m_bUTCTime)
    return(m_lTime > ChkTime.m_lTime);

  CTM tmThis = *this;
  tmThis.ToUTC();
  CTM tmCheck = ChkTime;
  tmCheck.ToUTC();
  return(tmThis.m_lTime > tmCheck.m_lTime);
}

///////////////////////////////////////////////////////////////////////////
// Needed because of static buffer used by localtime()

void CTM::GetTime(time_t lTime, tm* pTM)
{
  tm* pTimeM = localtime(&lTime);
  ASSERT(pTimeM != NULL);
  memcpy(pTM, pTimeM, sizeof(tm));
}

///////////////////////////////////////////////////////////////////////////
// Set the date to 'iDay iMonth iYear'. If iMonth or iYear aren't
// set, they remain the same

void CTM::SetDate(int iDay, int iMonth /*=-1*/, int iYear /*=-1*/)
{
  struct tm tmTemp;
  memcpy(&tmTemp, m_pTM, sizeof(tm));

  tmTemp.tm_mday = iDay;
  if (iMonth != -1)
    tmTemp.tm_mon = iMonth;
  if (iYear != -1)
    tmTemp.tm_year = iYear;

  time_t lResult = mktime(&tmTemp);
  if (lResult != time_t(-1))
    SetTime(lResult);
}

///////////////////////////////////////////////////////////////////////////
// Set the time to lTime. Very useful with standard C-time routines.
//
// Used by *all* the other SetTime-routines. So adjustments here must
// be usefull for ALL SetTime-routines.
//

void CTM::SetTime(time_t lTime)
{
  m_lTime = lTime;
  GetTime(m_lTime, m_pTM);
  InitMonthDays();

  m_stTime->wYear = WORD(m_pTM->tm_year + 1900);
  m_stTime->wMonth = WORD(m_pTM->tm_mon + 1);
  m_stTime->wDay = (WORD)m_pTM->tm_mday;
  m_stTime->wDayOfWeek = (WORD)m_pTM->tm_wday;
  m_stTime->wHour = (WORD)m_pTM->tm_hour;
  m_stTime->wMinute = (WORD)m_pTM->tm_min;
  m_stTime->wSecond = (WORD)m_pTM->tm_sec;
  m_stTime->wMilliseconds = 0;
}

///////////////////////////////////////////////////////////////////////////
// Set the time to 'iHour iMin iSec'.

void CTM::SetTime(int iHour, int iMin, int iSec /*=0*/)
{
  struct tm tmTemp;
  memcpy(&tmTemp, m_pTM, sizeof(tm));
  tmTemp.tm_hour = iHour;
  tmTemp.tm_min = iMin;
  tmTemp.tm_sec = iSec;

  time_t lResult = mktime(&tmTemp);
  if (lResult != time_t(-1))
    SetTime(lResult);
}

///////////////////////////////////////////////////////////////////////////
//

void CTM::SetTime(WORD wDosDate, WORD wDosTime)
{
  struct tm tmTemp;
  tmTemp.tm_hour = wDosTime >> 11;
  tmTemp.tm_min = (wDosTime & ~0xF800) >> 5;
  tmTemp.tm_sec = (wDosTime & ~0xFFE0) << 1;

  tmTemp.tm_year = (wDosDate >> 9) + 80;
  tmTemp.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
  tmTemp.tm_mday = wDosDate & ~0xFFE0;

  time_t lResult = mktime(&tmTemp);
  if (lResult != time_t(-1))
    SetTime(lResult);
}

///////////////////////////////////////////////////////////////////////////
//

void CTM::SetTime(LPFILETIME ftTime)
{
  SYSTEMTIME stTime;
  FileTimeToSystemTime(ftTime, &stTime);
  SetTime(&stTime);
}

///////////////////////////////////////////////////////////////////////////
//

void CTM::SetTime(LPSYSTEMTIME stTime)
{
  struct tm tmTemp;
  tmTemp.tm_hour = stTime->wHour;
  tmTemp.tm_min = stTime->wMinute;
  tmTemp.tm_sec = stTime->wSecond;

  tmTemp.tm_year = stTime->wYear - 1900;
  tmTemp.tm_mon = stTime->wMonth - 1;
  tmTemp.tm_mday = stTime->wDay;

  time_t lResult = mktime(&tmTemp);
  if (lResult != time_t(-1))
    SetTime(lResult);
}

///////////////////////////////////////////////////////////////////////////
//

void CTM::SetLanguage(WORD wLang)
{
  if (wLang != LANG_CURRENT)
  {
    m_wCurLang = wLang;
    m_lcid = MAKELCID(m_wCurLang, SORT_DEFAULT);
  }
}

///////////////////////////////////////////////////////////////////////////
// Is the current year a leap-year. No century-check is done because
// tm-struct can't get into a problem (It can't get to 2100).

BOOL CTM::IsLeap(void)
{
  return(m_pTM->tm_year % 4 == 0);
}

///////////////////////////////////////////////////////////////////////////
// Decode the Relative SystemTime format.
//
// wDay defines which wDayOfWeek is used for time change:
// - 1..4 identify the 1st through 4th occurrence of wDayOfWeek in wMonth
// - 5 identifies the last occurrence in the month

CTM CTM::DecodeRelativeFormat(SYSTEMTIME& sys)
{
  CTM tm;
  tm.SetDate(Day(), (int) sys.wMonth - 1);

  int iDay = 0;
  switch (sys.wDay)
  {
    case 1: case 2:
    case 3: case 4:
    {
      iDay = tm.First(sys.wDayOfWeek);
      for (int i = 1; i < sys.wDay; i++)
        iDay += 7;
      break;
    }
    case 5:
      iDay = tm.Last(sys.wDayOfWeek);
      break;
  }

  tm.SetDate(iDay);
  tm.SetTime((int) sys.wHour, (int) sys.wMinute);

  return(tm);
}

///////////////////////////////////////////////////////////////////////////
// Is Daylight Saving Time active at the current time

BOOL CTM::IsDst()
{
  TIME_ZONE_INFORMATION TZInfo;
  GetTimeZoneInformation(&TZInfo);

  // If TZInfo.DaylightDate.wMonth is zero, DST not implemented.
  if (TZInfo.DaylightDate.wMonth == 0)
    return(FALSE);

  // Initialize as if we had absolute dates.
  CTM tmBegin(&TZInfo.DaylightDate);
  CTM tmEnd(&TZInfo.StandardDate);

  // If wYear is zero, use relative dates.
  if (TZInfo.StandardDate.wYear == 0)
  {
    tmBegin = DecodeRelativeFormat(TZInfo.DaylightDate);
    tmEnd = DecodeRelativeFormat(TZInfo.StandardDate);
  }

  if (TZInfo.DaylightDate.wMonth < TZInfo.StandardDate.wMonth)
    return(m_lTime >= tmBegin && m_lTime < tmEnd);  // For Northern Hemisphere...
  else
    return(m_lTime < tmEnd || m_lTime >= tmBegin);
}

///////////////////////////////////////////////////////////////////////////
// Returns the first 'iWDay' of the current month

int CTM::First(int iWDay)
{
  return((m_pTM->tm_mday + 6 + iWDay - m_pTM->tm_wday) % 7 + 1);
}

///////////////////////////////////////////////////////////////////////////
// Returns the last 'iWDay' of the current month

int CTM::Last(int iWDay)
{
  int LastWDay = First(iWDay) + 21;

  while (LastWDay + 7 <= m_iDaysInMonth[m_pTM->tm_mon])
    LastWDay += 7;
  return(LastWDay);
}

///////////////////////////////////////////////////////////////////////////
// What is the current weeknumber. Only 'WEEK_DAY4' is implemented *and*
// tested yet.

int CTM::WeekNr(int iType)
{
  if (iType == WEEK_USE_SYSTEM)
    iType = atoi(GetLocalize(LOCALE_IFIRSTWEEKOFYEAR));

  int iWeekDay1Jan; // Day that 1 January is on in the current year (Sun=0)
  int iWeek1Jan;    // WeekNr of 1 January of the current year

  iWeekDay1Jan = (m_pTM->tm_wday - m_pTM->tm_yday) % 7;
  if (iWeekDay1Jan <= 0)
    iWeekDay1Jan += 7;

  switch (iType)
  {
    case WEEK_JAN1:
      return((m_pTM->tm_yday + iWeekDay1Jan) / 7);

    case WEEK_FULL:
      break;

    case WEEK_DAY4:
      if (iWeekDay1Jan < 5)            // Mon, Tue, Wed of Thu
        iWeek1Jan = 1;
      else                             // Fri, Sat of Sun
        iWeek1Jan = 0;
      return(iWeek1Jan + (m_pTM->tm_yday + iWeekDay1Jan - 1) / 7);
  }
  return(-1);
}

///////////////////////////////////////////////////////////////////////////
// Returns the offset of Between

long CTM::GetBetweenOffset(int iBetween)
{
  if (iBetween == BETWEEN_CURRENT)
    iBetween = m_iCurrentBetween;

  switch (iBetween)
  {
    case BETWEEN_DAYS:
      return(86400L);
    case BETWEEN_HOURS:
      return(3600L);
    case BETWEEN_MINS:
      return(60L);
    case BETWEEN_SECS:
      return(1L);
  }
  return(1L);
}

///////////////////////////////////////////////////////////////////////////
//

void CTM::Add(long lNr, int iBetween)
{
  if (lNr == 0L)
    return;

  time_t lTime;
  if (iBetween == BETWEEN_MONTHS)
  {
    struct tm t;
    memcpy(&t, m_pTM, sizeof(struct tm));
    t.tm_mon += (int) lNr;
    while (t.tm_mon < 0)
    {
      t.tm_mon += 12;
      t.tm_year--;
    }
    while (t.tm_mon > 11)
    {
      t.tm_mon -= 12;
      t.tm_year++;
    }
    lTime = mktime(&t);
  }
  else
    lTime = m_lTime + (lNr * GetBetweenOffset(iBetween));

  SetTime(lTime);
}

///////////////////////////////////////////////////////////////////////////
// Returns lNr before (lNr < 0) or after (lNr > 0) today (NOT WELL TESTED)

CTM CTM::Next(long lNr, int iBetween)
{
  CTM Alternate = *this;
  Alternate.Add(lNr, iBetween);
  return(Alternate);
}

///////////////////////////////////////////////////////////////////////////
// To check if the current time between two times

BOOL CTM::Between(CTM tTime1, CTM tTime2)
{
  time_t t1 = tTime1.m_lTime;
  time_t t2 = tTime2.m_lTime;

  // Need to swap the times?
  if (t1 > t2)
  {
    time_t tmp = t1;
    t1 = t2;
    t2 = tmp;
  }

  return((m_lTime >= t1) && (m_lTime <= t2));
}

///////////////////////////////////////////////////////////////////////////
// Returns the number of 'btBetween' between the current and 'Other'
// (NOT WELL TESTED)

long CTM::Between(CTM Other, int iBetween)
{
  return((m_lTime - Other.m_lTime) / GetBetweenOffset(iBetween));
}

///////////////////////////////////////////////////////////////////////////
// Get the time difference between local time and UTC

long CTM::GetUTCBias()
{
  TIME_ZONE_INFORMATION TZInfo;
  GetTimeZoneInformation(&TZInfo);
  LONG lBias = TZInfo.Bias;

  if (IsDst())
    lBias += TZInfo.DaylightBias;
  else
    lBias += TZInfo.StandardBias;

  return(lBias * 60);
}

///////////////////////////////////////////////////////////////////////////
// Convert current time to UTC

void CTM::ToUTC()
{
  if (m_bUTCTime || IsEmpty())
    return;

  SetTime(m_lTime + GetUTCBias());
  m_bUTCTime = TRUE;
}

///////////////////////////////////////////////////////////////////////////
// Convert current time to local time

void CTM::ToLocal()
{
  if (!m_bUTCTime || IsEmpty())
    return;

  SetTime(m_lTime - GetUTCBias());
  m_bUTCTime = FALSE;
}

///////////////////////////////////////////////////////////////////////////
// Return the localized names

CString CTM::GetLocalize(LCTYPE lcType, int i, WORD wLang)
{
  char szStr[_MAX_PATH];

  LCID lcid = (wLang == LANG_CURRENT ? m_lcid : MAKELCID(wLang, SORT_DEFAULT));
  int iRet = GetLocaleInfo(lcid, lcType + i, szStr, _MAX_PATH);
  ASSERT(iRet != 0);
  return(szStr);
}

///////////////////////////////////////////////////////////////////////////
// Display the name of 'iMon'

CString CTM::MonthName(int iMon, WORD wLang, BOOL bFull)
{
  if (iMon == -1)
    iMon = m_pTM->tm_mon;
  ASSERT(iMon >= 0 && iMon < MAXMONTH);

  return(GetLocalize(bFull ? LOCALE_SMONTHNAME1 : LOCALE_SABBREVMONTHNAME1, iMon, wLang));
}

///////////////////////////////////////////////////////////////////////////
// Display the name of 'iWDay'

CString CTM::DayName(int iWDay, WORD wLang, BOOL bFull)
{
  if (iWDay == -1)
    iWDay = m_pTM->tm_wday;
  ASSERT(iWDay >= 0 && iWDay < MAXWDAY);

  return(GetLocalize(bFull ? LOCALE_SDAYNAME1 : LOCALE_SABBREVDAYNAME1,
                                        (iWDay == 0 ? 6 : iWDay - 1), wLang));
}

///////////////////////////////////////////////////////////////////////////
// Add the sprintf-string 'szFormat' to 'szStr'. pStr points to the
// end of the string.

#define MAXSTR 256      // Used for serveral internal strings

void CTM::SPrintAdd(CString& strStr, LPCSTR szFormat, ...)
{
  char szTmp[MAXSTR];
  va_list pArg;

  va_start(pArg, szFormat);
  vsprintf(szTmp, szFormat, pArg);
  va_end(pArg);
  strStr += szTmp;
}

#define FORMAT_D1 0
#define FORMAT_D2 1
#define FORMAT_D3 2
#define FORMAT_D4 3
#define MAXFORMAT 4

#define FORMAT_NORMAL   0
#define FORMAT_ZERO     1
#define FORMAT_FIXED    2

static char *s_szFormats[MAXFORMAT][3] = {
//  Normal   Zero     Fixed
  { "%d",   "%01d",   "%1d" },
  { "%d",   "%02d",   "%2d" },
  { "%d",   "%03d",   "%3d" },
  { "%d",   "%04d",   "%4d" }
};

char* CTM::Format(int i)
{
  return(s_szFormats[i][m_iFormatMode]);
}

///////////////////////////////////////////////////////////////////////////
// Format a string something like the Unix 'date' command.
//
// When time is 05:03 (24-hour)
//  %t = '5:03'
// %0t = '05:03'
// %1t = ' 5:03'

CString CTM::Format(LPCSTR szFormat)
{
  char szTmp[MAXSTR];
  CString strStr;

  for (; *szFormat; szFormat++)
  {
    if (*szFormat == '%' && *(szFormat+1))
    {
      m_iFormatMode = FORMAT_NORMAL;
      szFormat++;
      if (*szFormat >= '0' && *szFormat <= '1' && *(szFormat + 1))
      {
        switch (*szFormat)
        {
          case '0': m_iFormatMode = FORMAT_ZERO; break;
          case '1': m_iFormatMode = FORMAT_FIXED; break;
        }
        szFormat++;
      }
      switch (*szFormat)
      {
        case 'd': // day of month - 1 to 31
          SPrintAdd(strStr, Format(FORMAT_D2), m_pTM->tm_mday);
          break;
        case 'm': // month of year - 1 to 12
          SPrintAdd(strStr, Format(FORMAT_D2), m_pTM->tm_mon + 1);
          break;
        case 'y': // last 2 digits of year - 0 to 99
          SPrintAdd(strStr, Format(FORMAT_D2), m_pTM->tm_year % 100);
          break;
        case 'Y': // full year - 1970 to 2036
          SPrintAdd(strStr, Format(FORMAT_D4), m_pTM->tm_year + 1900);
          break;
        case 'h': // hour - 1 to 12
          SPrintAdd(strStr, Format(FORMAT_D2), m_pTM->tm_hour % 12 == 0 ? 12 : m_pTM->tm_hour % 12);
          break;
        case 'H': // hour - 0 to 23
          SPrintAdd(strStr, Format(FORMAT_D2), m_pTM->tm_hour);
          break;
        case 'M': // minute - 0 to 59
          SPrintAdd(strStr, Format(FORMAT_D2), m_pTM->tm_min);
          break;
        case 'S': // second - 0 to 59
          SPrintAdd(strStr, Format(FORMAT_D2), m_pTM->tm_sec);
          break;
        case 'p': // AM or PM
          strStr += GetLocalize(m_pTM->tm_hour > 12 ? LOCALE_S2359 : LOCALE_S1159);
          break;
        case 'j': // day of year - 1 to 366
          SPrintAdd(strStr, Format(FORMAT_D3), m_pTM->tm_yday);
          break;
        case 'w': // day of week - Sunday = 0
          SPrintAdd(strStr, Format(FORMAT_D1), m_pTM->tm_wday);
          break;
        case 'q': // quarter - 1 to 4
          SPrintAdd(strStr, Format(FORMAT_D1), (m_pTM->tm_mon / 4) + 1);
          break;

        case 'a': // abbreviated weekday - Sun to Sat (in current language)
        case 'A': // weekday - Sunday to Saturday (in current language)
          strStr += GetLocalize(islower(*szFormat) ? LOCALE_SABBREVDAYNAME1 :
                          LOCALE_SDAYNAME1, (m_pTM->tm_wday == 0 ? 6 : m_pTM->tm_wday - 1));
          break;
        case 'o': // abbreviated month - Jan to Dec (in current language)
        case 'O': // month - January to December (in current language)
          strStr += GetLocalize(islower(*szFormat) ? LOCALE_SABBREVMONTHNAME1 :
                          LOCALE_SMONTHNAME1, m_pTM->tm_mon);
          break;
        case 'f': // Short Date - 31-12-2000 (on my Dutch system)
        case 'F': // Long Date - zondag 31 december 2000 (on my Dutch system)
          GetDateFormat(m_lcid, islower(*szFormat) ? DATE_SHORTDATE : DATE_LONGDATE,
                                      m_stTime, NULL, szTmp, MAXSTR);
          strStr += szTmp;
          break;
        case 't': // Short time - 22:45 (on my Dutch system)
        case 'T': // Long time - 22:45:36 (on my Dutch system)
          GetTimeFormat(m_lcid, islower(*szFormat) ? TIME_NOSECONDS : 0,
                                      m_stTime, NULL, szTmp, MAXSTR);
          strStr += szTmp;
          break;
        case 'z': // Time zone difference with UTC
        {
          long lBias = GetUTCBias();
          strStr += (lBias > 0 ? '-' : '+');
          CString strTmp;
          strTmp.Format("%02ld%02ld", labs(lBias) / 3600, labs(lBias) % 3600);
          strStr += strTmp;
          break;
        }

        // *** Added For Dorgem ***
        case 'G': // Date in yyyymmdd format (to be filename safe)
        {
          CString strTmp;
          strTmp.Format("%04d%02d%02d", m_pTM->tm_year + 1900, m_pTM->tm_mon + 1, m_pTM->tm_mday);
          strStr += strTmp;
          break;
        }
        case 'g': // Time in hhmmss format (to be filename safe)
        {
          CString strTmp;
          strTmp.Format("%02d%02d%02d", m_pTM->tm_hour, m_pTM->tm_min, m_pTM->tm_sec);
          strStr += strTmp;
          break;
        }
        case 'P': // Program name
        {
          CString strTmp;
          strTmp.LoadString(IDS_PRODUCT_NAME);
          strStr += strTmp;
          break;
        }
        case 'V': // Program version
        {
          CString strTmp;
          strTmp.LoadString(IDS_PRODUCT_VERSION);
          strStr += strTmp;
          break;
        }
/*
        case 'W':
        {
          // What is playing in Winamp 2.x
          HWND hWnd;
          if ((hWnd = FindWindow("Winamp v1.x", NULL)) != NULL)
          {
            TCHAR szCaption[_MAX_PATH];
            if (GetWindowText(hWnd, szCaption, _MAX_PATH) != 0)
            {
              CString strTmp = szCaption;
              if (strTmp[strTmp.GetLength() - 1] != ']')  // Is there something like [Stopped]
              {
                int iIndex = strTmp.Find(".");
                if (iIndex != -1)
                {
                  strTmp = strTmp.Mid(iIndex + 2);              // Strip the playlist-number
                  strTmp = strTmp.Left(strTmp.GetLength() - 9); // Remove ' - Winamp' at the end
                  strStr += strTmp;
                  break;
                }
              }
            }
          }
          break;
        }
*/
        // *** End Added For Dorgem ***


        default:
          if (m_iFormatMode != FORMAT_NORMAL)
            strStr += *(szFormat - 2);
          strStr += *(szFormat - 1);
        case '%':
          strStr += *szFormat;
          break;
      }
    }
    else
      strStr += *szFormat;
  }
  return(strStr);
}

///////////////////////////////////////////////////////////////////////////
// Determine the date of Easter for any given year
// Public domain by Ed Bernal

CTM CTM::Easter()
{
  int a, b, c, e, g, h, i, k, u, x, z;
  int Month, Day;
  div_t f;

  // Gauss' famous algorithm. I don't know how or why it works,
  // so there's no commenting

  a = m_pTM->tm_year % 19;
  f = div(m_pTM->tm_year, 100);
  b = f.quot;
  c = f.rem;
  f = div(b, 4);
  z = f.quot;
  e = f.rem;
  f = div((8 * b + 13), 25);
  g = f.quot;
  f = div((19 * a + b - z - g + 15), 30);
  h = f.rem;
  f = div((a + 11 * h), 319);
  u = f.quot;
  f = div(c, 4);
  i = f.quot;
  k = f.rem;
  f = div((2 * e + 2 * i - k - h + u + 32), 7);
  x = f.rem;
  f = div((h - u + x + 90), 25);
  Month = f.quot;
  f = div((h - u + x + Month + 19), 32);
  Day = f.rem;

  CTM Alternate = *this;
  Alternate.SetDate(Day, Month - 1);
  return(Alternate);
}
