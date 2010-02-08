/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See Licence.txt
 */

/**
 * @file
 * @brief Test ARBAgilityRecordBook class
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2008-01-18 DRC Created empty file
 */

#include "stdafx.h"
#include "TestARB.h"

#include "ARBStructure.h"
#include "ARBAgilityRecordBook.h"
#include "Element.h"


SUITE(TestAgilityRecordBook)
{
	TEST(GetCurrentDocVersion)
	{
		TODO_TEST
		//static ARBVersion const& GetCurrentDocVersion();
	}


	TEST(ctor)
	{
		TODO_TEST
	//	ARBAgilityRecordBook();
	}


	TEST(clear)
	{
		TODO_TEST
	//	void clear();
	}


	TEST(Load)
	{
		TODO_TEST
	//	bool Load(
	//		ElementNodePtr inTree,
	//		bool inCalendar,
	//		bool inTraining,
	//		bool inConfig,
	//		bool inInfo,
	//		bool inDogs,
	//		ARBErrorCallback& ioCallback);
	//	bool Load(ElementNodePtr inTree, ARBErrorCallback& ioCallback)
	//	{
	//		return Load(inTree, true, true, true, true, true, ioCallback);
	//	}
	}


	TEST(Save)
	{
		TODO_TEST
	//	bool Save(
	//		ElementNodePtr outTree,
	//		wxString const& inPgmVer,
	//		bool inCalendar,
	//		bool inTraining,
	//		bool inConfig,
	//		bool inInfo,
	//		bool inDogs) const;
	}


	TEST(Default)
	{
		TODO_TEST
	//	void Default();
	}


	TEST(Update)
	{
		TODO_TEST
	//	bool Update(
	//			int indent,
	//			ARBConfig const& inConfigNew,
	//			wxString& ioInfo,
	//			IConfigActionCallback& ioCallBack);
	}


	TEST(GetAllClubNames)
	{
		TODO_TEST
	//	size_t GetAllClubNames(
	//			std::set<wxString>& outClubs,
	//			bool bInfo,
	//			bool bVisibleOnly) const;
	}


	TEST(GetAllTrialLocations)
	{
		TODO_TEST
	//	size_t GetAllTrialLocations(
	//			std::set<wxString>& outLocations,
	//			bool bInfo,
	//			bool bVisibleOnly) const;
	}


	TEST(GetAllEventSubNames)
	{
		TODO_TEST
	//	size_t GetAllEventSubNames(
	//			wxString const& inVenue,
	//			ARBConfigEventPtr inEvent,
	//			std::set<wxString>& outNames) const;
	}


	TEST(GetAllHeights)
	{
		TODO_TEST
	//	size_t GetAllHeights(std::set<wxString>& outHeights) const;
	}


	TEST(GetAllCallNames)
	{
		TODO_TEST
	//	size_t GetAllCallNames(std::set<wxString>& outNames) const;
	}


	TEST(GetAllBreeds)
	{
		TODO_TEST
	//	size_t GetAllBreeds(std::set<wxString>& outBreeds) const;
	}


	TEST(GetAllJudges)
	{
		TODO_TEST
	//	size_t GetAllJudges(
	//			std::set<wxString>& outJudges,
	//			bool bInfo,
	//			bool bVisibleOnly) const;
	}


	TEST(GetAllHandlers)
	{
		TODO_TEST
	//	size_t GetAllHandlers(std::set<wxString>& outHandlers) const;
	}


	TEST(GetAllPartners)
	{
		TODO_TEST
	//	void GetAllPartners(
	//			std::set<wxString>& outPartners,
	//			std::set<wxString>& outDogs) const;
	}


	TEST(GetAllFaultTypes)
	{
		TODO_TEST
	//	size_t GetAllFaultTypes(std::set<wxString>& outFaults) const;
	}
}