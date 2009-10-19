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
 * @brief Test ARBConfigVenue and ARBConfigVenueList classes.
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2008-01-18 DRC Created empty file
 */

#include "stdafx.h"
#include "TestARB.h"

#include "ARBStructure.h"
#include "ARBConfigVenue.h"
#include "Element.h"


SUITE(TestConfigVenue)
{
	TEST(New)
	{
		TODO_TEST
	//	static ARBConfigVenuePtr New();
	}


	TEST(Clone)
	{
		TODO_TEST
	//	ARBConfigVenuePtr Clone() const;
	}


	TEST(OpEqual)
	{
		TODO_TEST
	//	ARBConfigVenue& operator=(ARBConfigVenue const& rhs);
	}


	TEST(Compare)
	{
		TODO_TEST
	//	bool operator==(ARBConfigVenue const& rhs) const;
	//	bool operator!=(ARBConfigVenue const& rhs) const
	}


	TEST(Clear)
	{
		TODO_TEST
	//	void clear();
	}


	TEST(GenName)
	{
		TODO_TEST
	//	virtual wxString GetGenericName() const
	}


	TEST(Load)
	{
		TODO_TEST
	//	bool Load(
	//			ARBConfig& ioConfig,
	//			ElementNodePtr inTree,
	//			ARBVersion const& inVersion,
	//			ARBErrorCallback& ioCallback);
	}


	TEST(Save)
	{
		TODO_TEST
	//	bool Save(ElementNodePtr ioTree) const;
	}


	TEST(Update)
	{
		TODO_TEST
	//	bool Update(
	//			int indent,
	//			ARBConfigVenuePtr inVenueNew,
	//			wxString& ioInfo);
	}
}


SUITE(TestConfigVenueList)
{
	TEST(Load)
	{
		TODO_TEST
	//	bool Load(
	//			ARBConfig& ioConfig,
	//			ElementNodePtr inTree,
	//			ARBVersion const& inVersion,
	//			ARBErrorCallback& ioCallback);
	}


	TEST(sort)
	{
		TODO_TEST
	//	void sort();
	}


	TEST(VerifyVenue)
	{
		TODO_TEST
	//	bool VerifyVenue(wxString const& inVenue) const
	}


	TEST(VerifyMultiQ)
	{
		TODO_TEST
	//	bool VerifyMultiQ(
	//			wxString const& inVenue,
	//			wxString const& inMultiQ,
	//			bool inUseShortName = false) const;
	}


	TEST(VerifyLevel)
	{
		TODO_TEST
	//	bool VerifyLevel(
	//			wxString const& inVenue,
	//			wxString const& inDivision,
	//			wxString const& inLevel) const;
	}


	TEST(VerifyEvent)
	{
		TODO_TEST
	//	bool VerifyEvent(
	//			wxString const& inVenue,
	//			wxString const& inDivision,
	//			wxString const& inLevel,
	//			wxString const& inEvent,
	//			ARBDate const& inDate) const;
	}


	TEST(FindTitleCompleteName)
	{
		TODO_TEST
	//	bool FindTitleCompleteName(
	//			wxString const& inVenue,
	//			wxString const& inName,
	//			bool bShowInstance,
	//			bool bAbbrevFirst = true,
	//			ARBConfigTitlePtr* outTitle = NULL) const;
	}


	TEST(FindTitle)
	{
		TODO_TEST
	//	bool FindTitle(
	//			wxString const& inVenue,
	//			wxString const& inTitle,
	//			ARBConfigTitlePtr* outTitle = NULL) const;
	}


	TEST(DeleteTitle)
	{
		TODO_TEST
	//	bool DeleteTitle(wxString const& inTitle);
	}


	TEST(Find)
	{
		TODO_TEST
	//	bool FindVenue(
	//			wxString const& inVenue,
	//			ARBConfigVenuePtr* outVenue = NULL) const;
	}


	TEST(FindEvent)
	{
		TODO_TEST
	//	bool FindEvent(
	//			wxString const& inVenue,
	//			wxString const& inEvent,
	//			wxString const& inDivision,
	//			wxString const& inLevel,
	//			ARBDate const& inDate,
	//			ARBConfigEventPtr* outEvent = NULL,
	//			ARBConfigScoringPtr* outScoring = NULL) const;
	}


	TEST(AddDelete)
	{
		TODO_TEST
	//	bool AddVenue(
	//			wxString const& inVenue,
	//			ARBConfigVenuePtr* outVenue = NULL);
	//	bool AddVenue(ARBConfigVenuePtr inVenue);
	//	int DeleteVenue(wxString const& inVenue);
	}
}