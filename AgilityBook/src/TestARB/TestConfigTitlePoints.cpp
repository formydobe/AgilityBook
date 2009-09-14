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
 * @brief Test ARBConfigTitlePoints and ARBConfigTitlePointsList class.
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2008-01-18 DRC Created empty file
 */

#include "stdafx.h"
#include "TestARB.h"

#include "ARBStructure.h"
#include "ARBConfigTitlePoints.h"
#include "Element.h"


SUITE(TestConfigTitlePoints)
{
	TEST(New)
	{
		TODO_TEST
	//	static ARBConfigTitlePointsPtr New();
	//	static ARBConfigTitlePointsPtr New(double inPoints, double inFaults);
	}


	TEST(Clone)
	{
		TODO_TEST
	//	ARBConfigTitlePointsPtr Clone() const;
	}


	TEST(OpEqual)
	{
		TODO_TEST
	//	ARBConfigTitlePoints& operator=(ARBConfigTitlePoints const& rhs);
	}


	TEST(Compare)
	{
		TODO_TEST
	//	bool operator==(ARBConfigTitlePoints const& rhs) const;
	//	bool operator!=(ARBConfigTitlePoints const& rhs) const
	}


	TEST(GenName)
	{
		TODO_TEST
	//	virtual wxString GetGenericName() const;
	}


	TEST(Load)
	{
		TODO_TEST
	//	bool Load(
	//			ElementNodePtr inTree,
	//			ARBVersion const& inVersion,
	//			ARBErrorCallback& ioCallback,
	//			ARBConfigLifetimePointsList& ioLifetimePoints);
	}


	TEST(Save)
	{
		TODO_TEST
	//	bool Save(ElementNodePtr ioTree) const;
	}
}


SUITE(TestConfigTitlePointsList)
{
	TEST(Load)
	{
		TODO_TEST
	//	bool Load(
	//			ElementNodePtr inTree,
	//			ARBVersion const& inVersion,
	//			ARBErrorCallback& ioCallback,
	//			ARBConfigLifetimePointsList& ioLifetimePoints);
	}


	TEST(sort)
	{
		TODO_TEST
	//	void sort();
	}


	TEST(GetTitlePoints)
	{
		TODO_TEST
	//	double GetTitlePoints(double inFaults) const;
	}


	TEST(Find)
	{
		TODO_TEST
	//	bool FindTitlePoints(
	//			double inFaults,
	//			ARBConfigTitlePointsPtr* outPoints = NULL) const;
	}


	TEST(AddDelete)
	{
		TODO_TEST
	//	bool AddTitlePoints(
	//			double inPoints,
	//			double inFaults,
	//			ARBConfigTitlePointsPtr* outPoints = NULL);
	//	bool DeleteTitlePoints(double inFaults);
	}
}
