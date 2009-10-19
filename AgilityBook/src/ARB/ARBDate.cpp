/*
 * Copyright (c) 2002-2009 David Connet. All Rights Reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies, that both the
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of David Connet, dcon Software,
 * AgilityBook, AgilityRecordBook or "Agility Record Book" not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission. David Connet makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * DAVID CONNET DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL DAVID CONNET BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * http://opensource.org
 * Open Source Historical Permission Notice and Disclaimer.
 */

/**
 * @file
 *
 * @brief Date class
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-07-13 DRC Added inForceOutput to GetString.
 * @li 2005-07-05 DRC Added a new formatting option.
 * @li 2004-09-28 DRC Changed how error reporting is done when loading.
 * @li 2004-06-16 DRC Changed ARBDate::GetString to put leadingzero into format.
 * @li 2004-01-04 DRC Added FromString().
 */

#include "stdafx.h"
#include "ARBDate.h"
#include <time.h>

#if defined(_MFC_VER) && defined(_DEBUG)
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// The following public domain code was found on the internet (calendar-1.11)
// SDN stands for serial day number. The author notes it is called this to
// differentiate the julian day (SDN) from a Julian calendar date.
// It has been modified only slightly.

// SdnToGregorian and GregorianToSdn
/* $selId: gregor.c,v 2.0 1995/10/24 01:13:06 lees Exp $
 * Copyright 1993-1995, Scott E. Lee, all rights reserved.
 * Permission granted to use, copy, modify, distribute and sell so long as
 * the above copyright and this permission statement are retained in all
 * copies.  THERE IS NO WARRANTY - USE AT YOUR OWN RISK.
 */

// VALID RANGE
//     4714 B.C. to at least 10000 A.D.
// REFERENCES
//     Conversions Between Calendar Date and Julian Day Number by Robert J.
//     Tantzen, Communications of the Association for Computing Machinery
//     August 1963.  (Also published in Collected Algorithms from CACM,
//     algorithm number 199).

#define SDN_OFFSET			32045
#define DAYS_PER_5_MONTHS	153
#define DAYS_PER_4_YEARS	1461
#define DAYS_PER_400_YEARS	146097

static void SdnToGregorian(
		long int sdn,
		int* pYear,
		int* pMonth,
		int* pDay)
{
	if (sdn <= 0)
	{
		*pYear = 0;
		*pMonth = 0;
		*pDay = 0;
		return;
	}

	long int temp = (sdn + SDN_OFFSET) * 4 - 1;

	/* Calculate the century (year/100). */
	int century = temp / DAYS_PER_400_YEARS;

	/* Calculate the year and day of year (1 <= dayOfYear <= 366). */
	temp = ((temp % DAYS_PER_400_YEARS) / 4) * 4 + 3;
	int year = (century * 100) + (temp / DAYS_PER_4_YEARS);
	int dayOfYear = (temp % DAYS_PER_4_YEARS) / 4 + 1;

	/* Calculate the month and day of month. */
	temp = dayOfYear * 5 - 3;
	int month = temp / DAYS_PER_5_MONTHS;
	int day = (temp % DAYS_PER_5_MONTHS) / 5 + 1;

	/* Convert to the normal beginning of the year. */
	if (month < 10)
		month += 3;
	else
	{
		year += 1;
		month -= 9;
	}

	/* Adjust to the B.C./A.D. type numbering. */
	year -= 4800;
	if (year <= 0)
		year--;

	*pYear = year;
	*pMonth = month;
	*pDay = day;
}


static long int GregorianToSdn(
		int inputYear,
		int inputMonth,
		int inputDay)
{
	int year;
	int month;

	/* check for invalid dates */
	if (inputYear == 0 || inputYear < -4714
	|| inputMonth <= 0 || inputMonth > 12
	|| inputDay <= 0 || inputDay > 31)
	{
		return 0;
	}

	/* check for dates before SDN 1 (Nov 25, 4714 B.C.) */
	if (inputYear == -4714)
	{
		if (inputMonth < 11)
			return 0;
		if (inputMonth == 11 && inputDay < 25)
			return 0;
	}

	/* Make year always a positive number. */
	if (inputYear < 0)
		year = inputYear + 4801;
	else
		year = inputYear + 4800;

	/* Adjust the start of the year. */
	if (inputMonth > 2)
		month = inputMonth - 3;
	else
	{
		month = inputMonth + 9;
		year--;
	}

	return ( ((year / 100) * DAYS_PER_400_YEARS) / 4
		+ ((year % 100) * DAYS_PER_4_YEARS) / 4
		+ (month * DAYS_PER_5_MONTHS + 2) / 5
		+ inputDay
		- SDN_OFFSET );
}

/////////////////////////////////////////////////////////////////////////////

// static
ARBDate ARBDate::Today()
{
	ARBDate today;
	today.SetToday();
	return today;
}


static int ParseFields(
		wxString inDate,
		char sep,
		int& val1,
		int& val2,
		int& val3)
{
	int nVals = 0;
	wxString::size_type pos = inDate.find(sep);
	if (wxString::npos != pos)
	{
		val1 = static_cast<short>(tstringUtil::atol(inDate));
		++nVals;
		inDate = inDate.substr(pos+1);
		pos = inDate.find(sep);
		if (wxString::npos != pos)
		{
			val2 = static_cast<short>(tstringUtil::atol(inDate));
			++nVals;
			inDate = inDate.substr(pos+1);
			val3 = static_cast<short>(tstringUtil::atol(inDate));
			++nVals;
		}
	}
	return nVals;
}


// static
ARBDate ARBDate::FromString(
		wxString const& inDate,
		ARBDate::DateFormat inFormat)
{
	ARBDate date;
	wxString value(inDate);
	if (0 < inDate.length())
	{
		int val1 = 0, val2 = 0, val3 = 0;
		int nDash = ParseFields(inDate, '-', val1, val2, val3);
		int nSlash = ParseFields(inDate, '/', val1, val2, val3);
		int yr = 0, mon = 0, day = 0;
		if (0 == inFormat)
		{
			if (3 == nDash)
				inFormat = eDashYYYYMMDD;
			else if (3 == nSlash)
				inFormat = eSlashMMDDYYYY;
		}
		if ((3 == nDash &&
		(eDashMMDDYYYY == inFormat || eDashYYYYMMDD == inFormat || eDashDDMMYYYY == inFormat))
		|| (3 == nSlash &&
		(eSlashMMDDYYYY == inFormat || eSlashYYYYMMDD == inFormat || eSlashDDMMYYYY == inFormat)))
		{

			switch (inFormat)
			{
			case eDashMMDDYYYY:
			case eSlashMMDDYYYY:
				mon = val1;
				day = val2;
				yr = val3;
				break;
			case eDashYYYYMMDD:
			case eSlashYYYYMMDD:
				yr = val1;
				mon = val2;
				day = val3;
				break;
			case eDashDDMMYYYY:
			case eSlashDDMMYYYY:
				day = val1;
				mon = val2;
				yr = val3;
				break;
			}
		}
		date.SetDate(yr, mon, day);
		if (date.IsValid())
		{
			int yr2, mon2, day2;
			date.GetDate(yr2, mon2, day2);
			if (yr != yr2 || mon != mon2 || day != day2)
				date.clear();
		}
	}
	return date;
}


// static
wxString ARBDate::GetValidDateString(
		ARBDate const& inFrom,
		ARBDate const& inTo,
		DateFormat inFormat)
{
	wxString str;
	if (inFrom.IsValid() || inTo.IsValid())
	{
		str += wxT("[");
		if (inFrom.IsValid())
			str += inFrom.GetString(inFormat);
		else
			str += wxT("*");
		str += wxT("-");
		if (inTo.IsValid())
			str += inTo.GetString(inFormat);
		else
			str += wxT("*");
		str += wxT("]");
	}
	return str;
}


ARBDate::ARBDate(time_t inTime)
	: m_Julian(0)
{
	if (0 != inTime)
	{
#if defined(ARB_HAS_SECURE_LOCALTIME)
		struct tm l;
		_localtime64_s(&l, &inTime);
		struct tm* pTime = &l;
#else
		struct tm* pTime = localtime(&inTime);
#endif
		m_Julian = GregorianToSdn(
			pTime->tm_year + 1900,
			pTime->tm_mon + 1,
			pTime->tm_mday);
	}
}


#if defined(ARB_HAS_32_AND_64_BIT_TIMET)
ARBDate::ARBDate(__time64_t inTime)
	: m_Julian(0)
{
	if (0 != inTime)
	{
		struct tm* pTime = _localtime64(&inTime);
		m_Julian = GregorianToSdn(
			pTime->tm_year + 1900,
			pTime->tm_mon + 1,
			pTime->tm_mday);
	}
}
#endif


void ARBDate::SetToday()
{
	time_t t;
	time(&t);
#if defined(ARB_HAS_SECURE_LOCALTIME)
	struct tm l;
	_localtime64_s(&l, &t);
	struct tm* pTime = &l;
#else
	struct tm* pTime = localtime(&t);
#endif
	m_Julian = GregorianToSdn(
		pTime->tm_year + 1900,
		pTime->tm_mon + 1,
		pTime->tm_mday);
}


bool ARBDate::SetDate(
		int inYr,
		int inMon,
		int inDay,
		bool bClearOnError)
{
	bool bOk = true;
	long julian = GregorianToSdn(inYr, inMon, inDay);
	int yr, mon, day;
	SdnToGregorian(julian, &yr, &mon, &day);
	if (yr != inYr || mon != inMon || day != inDay)
	{
		bOk = false;
		if (bClearOnError)
			m_Julian = 0;
	}
	else
		m_Julian = julian;
	return bOk;
}


wxString ARBDate::GetString(
	DateFormat inFormat,
	bool inForceOutput) const
{
	if (!inForceOutput && !IsValid())
		return wxT("");
	wxString date;
	int yr = 0;
	int mon = 0;
	int day = 0;
	if (IsValid())
		SdnToGregorian(m_Julian, &yr, &mon, &day);
	switch (inFormat)
	{
	case eDashMMDDYYYY:		///< MM-DD-YYYY
		date = wxString::Format(wxT("%02d-%02d-%04d"), mon, day, yr);
		break;
	case eYYYYMMDD:
		date = wxString::Format(wxT("%04d%02d%02d"), yr, mon, day);
		break;
	default:				///< YYYY-MM-DD or MM/DD/YYYY
	case eSlashMMDDYYYY:	///< MM/DD/YYYY
		date = wxString::Format(wxT("%02d/%02d/%04d"), mon, day, yr);
		break;
	case eDashYYYYMMDD:		///< YYYY-MM-DD
		date = wxString::Format(wxT("%04d-%02d-%02d"), yr, mon, day);
		break;
	case eSlashYYYYMMDD:	///< YYYY/MM/DD
		date = wxString::Format(wxT("%04d/%02d/%02d"), yr, mon, day);
		break;
	case eDashDDMMYYYY:		///< DD-MM-YYYY
		date = wxString::Format(wxT("%02d-%02d-%04d"), day, mon, yr);
		break;
	case eSlashDDMMYYYY:	///< DD/MM/YYYY
		date = wxString::Format(wxT("%02d/%02d/%04d"), day, mon, yr);
		break;
	case eDashMDY:	///< M-D-Y
		date = wxString::Format(wxT("%d-%d-%d"), mon, day, yr);
		break;
	case eSlashMDY:	///< M/D/Y
		date = wxString::Format(wxT("%d/%d/%d"), mon, day, yr);
		break;
	case eDashYMD:	///< Y-M-D
		date = wxString::Format(wxT("%d-%d-%d"), yr, mon, day);
		break;
	case eSlashYMD:	///< Y/M/D
		date = wxString::Format(wxT("%d/%d/%d"), yr, mon, day);
		break;
	case eDashDMY:	///< D-M-Y
		date = wxString::Format(wxT("%d-%d-%d"), day, mon, yr);
		break;
	case eSlashDMY:	///< D/M/Y
		date = wxString::Format(wxT("%d/%d/%d"), day, mon, yr);
		break;
	}
	return date;
}


time_t ARBDate::GetDate() const
{
	time_t t = 0;
	if (0 < m_Julian)
	{
		int yr, mon, day;
		SdnToGregorian(m_Julian, &yr, &mon, &day);
		struct tm tim;
		tim.tm_sec = 0;
		tim.tm_min = 0;
		tim.tm_hour = 0;
		tim.tm_mday = day;
		tim.tm_mon = mon - 1;
		tim.tm_year = yr - 1900;
		tim.tm_wday = 0;
		tim.tm_yday = 0;
		tim.tm_isdst = 0;
		t = mktime(&tim);
	}
	return t;
}


#ifdef WIN32
bool ARBDate::GetDate(SYSTEMTIME& outTime) const
{
	if (0 < m_Julian)
	{
		int yr, mon, day;
		SdnToGregorian(m_Julian, &yr, &mon, &day);
		outTime.wYear = static_cast<WORD>(yr);
		outTime.wMonth = static_cast<WORD>(mon);
		outTime.wDayOfWeek = static_cast<WORD>(GetDayOfWeek());
		outTime.wDay = static_cast<WORD>(day);
		outTime.wHour = 0;
		outTime.wMinute = 0;
		outTime.wSecond = 0;
		outTime.wMilliseconds = 0;
		return true;
	}
	return false;
}
#endif


void ARBDate::GetDate(
		int& outYr,
		int& outMon,
		int& outDay) const
{
	SdnToGregorian(m_Julian, &outYr, &outMon, &outDay);
}


int ARBDate::GetDay() const
{
	int yr, mon, day;
	SdnToGregorian(m_Julian, &yr, &mon, &day);
	return day;
}


int ARBDate::GetMonth() const
{
	int yr, mon, day;
	SdnToGregorian(m_Julian, &yr, &mon, &day);
	return mon;
}


int ARBDate::GetYear() const
{
	int yr, mon, day;
	SdnToGregorian(m_Julian, &yr, &mon, &day);
	return yr;
}