/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 *
 * @brief Utility classes
 * @author David Connet
 *
 * Revision History
 * @li 2013-07-17 DRC Moved SanitizeStringForHTML to ARBMisc.
 * @li 2012-12-12 DRC Use fabs instead of abs. Works on Mac too.
 * @li 2012-08-13 DRC Moved ARB_Q to separate file.
 * @li 2012-05-04 DRC Add bAlwaysStripZeros to ARBDouble::ToString.
 * @li 2012-04-10 DRC Based on wx-group thread, use std::string for internal use
 * @li 2009-11-24 DRC Optimize locale usage when reading/writing the ARB file.
 * @li 2009-09-28 DRC Fix abs() on Mac.
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9.
 * @li 2008-06-29 DRC Moved string stuff to ARBString.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-06-25 DRC Removed ARBDouble.
 * @li 2004-09-28 DRC Changed how error reporting is done when loading.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 */

#include "stdafx.h"
#include "ARBCommon/ARBTypes.h"

#include "ARBCommon/Element.h"
#include "ARBCommon/StringUtil.h"
#include <iostream>
#include <math.h>
#include <sstream>
#include <time.h>

#if defined(__WXWINDOWS__)
#include <wx/string.h>
#endif

#if defined(__WXMSW__)
#include <wx/msw/msvcrt.h>
#endif

/////////////////////////////////////////////////////////////////////////////

// Trailing zeros are trimmed unless inPrec=2.
// Then they are only trimmed if all zero (and inPrec=2).
std::wstring ARBDouble::ToString(
		double inValue,
		int inPrec,
		LocaleType eUseDefaultLocale,
		bool bAlwaysStripZeros)
{
#if !defined(__WXWINDOWS__)
	eUseDefaultLocale = eNone;
#endif
	std::wstring retVal;
	wchar_t pt = '.';
	if (eNone == eUseDefaultLocale)
	{
		// When we don't want locales, avoid the wx functions as they use them.
		std::wostringstream str;
		if (0 < inPrec)
			str.precision(inPrec);
		str << std::fixed << inValue;
		retVal = str.str();
	}
#if defined(__WXWINDOWS__)
	else
	{
		wxLocale* locale = NULL;
		if (eDefault == eUseDefaultLocale)
			locale = new wxLocale(wxLANGUAGE_DEFAULT, 0);
		wxString decimalPt = wxLocale::GetInfo(wxLOCALE_DECIMAL_POINT, wxLOCALE_CAT_NUMBER);
		if (0 < decimalPt.length())
			pt = decimalPt.GetChar(0);
		wxString tmp;
		if (0 < inPrec)
			tmp = wxString::Format(L"%.*f", inPrec, inValue);
		else
			tmp = wxString::Format(L"%g", inValue);
		retVal = StringUtil::stringW(tmp);
		delete locale;
	}
#endif
	std::wstring::size_type pos = retVal.find(pt);
	if (std::wstring::npos != pos)
	{
		// Strip trailing zeros iff they are all 0.
		if (2 == inPrec && !bAlwaysStripZeros)
		{
			std::wstring twoZeros;
			twoZeros = pt;
			twoZeros += L"00";
			if (retVal.substr(pos) == twoZeros)
			{
				// Input is ".00", so simplify
				if (0 == pos)
					retVal = L"0";
				// Strip the ".00".
				else
					retVal = retVal.substr(0, pos);
			}
		}
		// Strip all trailing 0s.
		else
		{
			size_t len = retVal.length();
			size_t oldLen = len;
			while (0 < len && retVal[len-1] == '0')
				--len;
			if (0 < len && retVal[len-1] == pt)
				--len;
			if (len != oldLen)
				retVal = retVal.substr(0, len);
		}
	}
	return retVal;
}


// http://groups.google.com/group/comp.lang.c++.moderated/msg/518274ddc6fb8541?hl=en&
bool ARBDouble::equal(
		double const& inVal1,
		double const& inVal2,
		double inPrec)
{
	int mag1, mag2;
	frexp(inVal1, &mag1);
	frexp(inVal2, &mag2);
	if (mag1 != mag2)
		return false;

	double epsilon = ldexp(inPrec, mag1);
	double diff = fabs(inVal1 - inVal2);
	return diff <= epsilon;
}

/////////////////////////////////////////////////////////////////////////////

std::wstring ARBVersion::str() const
{
	std::wostringstream buffer;
	buffer << Major() << L"." << Minor();
	return buffer.str();
}
