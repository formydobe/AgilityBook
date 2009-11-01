/*
 * Copyright (c) 2008-2009 David Connet. All Rights Reserved.
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
 * @brief Test ARBDouble functions
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2008-01-12 DRC Created
 */

#include "stdafx.h"
#include "TestARB.h"

#include "ARBTypes.h"


static wxString FormNumber(wxString const& d1, wxString const& dec, wxString const& d2)
{
	wxString val;
	val << d1 << dec << d2;
	return val;
}


static void RunDblTests()
{
	wxString decimalPt = wxLocale::GetInfo(wxLOCALE_DECIMAL_POINT, wxLOCALE_CAT_NUMBER);

	// ARBDouble always strips 0s unless prec ==2, unless =".00"
	double p = 3.14159265358979323846;
	wxString s = ARBDouble::ToString(p, 2, ARBDouble::eCurrent);
	CHECK(FormNumber(wxT("3"), decimalPt, wxT("14")) == s);
	s = ARBDouble::ToString(p, 4, ARBDouble::eCurrent);
	CHECK(FormNumber(wxT("3"), decimalPt, wxT("1416")) == s);

	p = 2.1;
	s = ARBDouble::ToString(p, 0, ARBDouble::eCurrent);
	CHECK(FormNumber(wxT("2"), decimalPt, wxT("1")) == s);
	s = ARBDouble::ToString(p, 2, ARBDouble::eCurrent);
	CHECK(FormNumber(wxT("2"), decimalPt, wxT("10")) == s);
	s = ARBDouble::ToString(p, 3, ARBDouble::eCurrent);
	CHECK(FormNumber(wxT("2"), decimalPt, wxT("1")) == s);

	p = 2;
	s = ARBDouble::ToString(p, 0, ARBDouble::eCurrent);
	CHECK(wxT("2") == s);
	s = ARBDouble::ToString(p, 1, ARBDouble::eCurrent);
	CHECK(wxT("2") == s);
	s = ARBDouble::ToString(p, 2, ARBDouble::eCurrent);
	CHECK(wxT("2") == s);
}


SUITE(TestDouble)
{
	TEST(strPrecUS)
	{
		wxLocale locale(wxLANGUAGE_ENGLISH_US);
		RunDblTests();
	}


	TEST(strPrecFR)
	{
		wxLocale locale(wxLANGUAGE_FRENCH);
		RunDblTests();
	}


	TEST(equal)
	{
		double p1 = 3.14159265;
		double p2 = 3.141592;
		double p3 = 3.141592657;
		CHECK(ARBDouble::equal(p1, p2, 0.00001));
		CHECK(!ARBDouble::equal(p1, p2, 0.00000001));
		CHECK(ARBDouble::equal(p1, p3, 0.00000001));
	}
}
