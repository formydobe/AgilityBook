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
 * @brief The classes that make up the dog's information.
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2009-07-05 DRC Fixed sort predicate so it is really stable.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-09-27 DRC Fixed sorting of trials.
 * @li 2005-06-25 DRC Cleaned up reference counting when returning a pointer.
 * @li 2005-01-10 DRC Only sort runs one way, the UI handles everything else.
 * @li 2005-01-01 DRC Renamed MachPts to SpeedPts.
 * @li 2004-09-28 DRC Changed how error reporting is done when loading.
 * @li 2003-12-28 DRC Added GetSearchStrings.
 * @li 2003-12-27 DRC Changed FindEvent to take a date.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 * @li 2003-07-16 DRC Allow the code to keep processing after errors are found.
 * @li 2003-06-23 DRC Added note field to trials.
 */

#include "stdafx.h"
#include "ARBDogTrial.h"
#include <algorithm>

#include "ARBAgilityRecordBook.h"
#include "ARBCalendar.h"
#include "ARBConfig.h"
#include "ARBDate.h"
#include "ARBLocalization.h"
#include "Element.h"

#if defined(_MFC_VER) && defined(_DEBUG)
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////

ARBDogTrialPtr ARBDogTrial::New()
{
	return ARBDogTrialPtr(new ARBDogTrial());
}


ARBDogTrialPtr ARBDogTrial::New(ARBCalendar const& inCal)
{
	return ARBDogTrialPtr(new ARBDogTrial(inCal));
}


ARBDogTrial::ARBDogTrial()
	: m_Location()
	, m_Note()
	, m_Verified(false)
	, m_Clubs()
	, m_Runs()
{
}


ARBDogTrial::ARBDogTrial(ARBCalendar const& inCal)
	: m_Location(inCal.GetLocation())
	, m_Note(inCal.GetNote())
	, m_Verified(false)
	, m_Clubs()
	, m_Runs()
{	
	m_Clubs.AddClub(inCal.GetClub(), inCal.GetVenue());
}


ARBDogTrial::ARBDogTrial(ARBDogTrial const& rhs)
	: m_Location(rhs.m_Location)
	, m_Note(rhs.m_Note)
	, m_Verified(rhs.m_Verified)
	, m_Clubs()
	, m_Runs()
{
	rhs.m_Clubs.Clone(m_Clubs);
	rhs.m_Runs.Clone(m_Runs);
}


ARBDogTrial::~ARBDogTrial()
{
}


ARBDogTrialPtr ARBDogTrial::Clone() const
{
	return ARBDogTrialPtr(new ARBDogTrial(*this));
}


ARBDogTrial& ARBDogTrial::operator=(ARBDogTrial const& rhs)
{
	if (this != &rhs)
	{
		m_Location = rhs.m_Location;
		m_Note = rhs.m_Note;
		m_Verified = rhs.m_Verified;
		rhs.m_Clubs.Clone(m_Clubs);
		rhs.m_Runs.Clone(m_Runs);
	}
	return *this;
}


bool ARBDogTrial::operator==(ARBDogTrial const& rhs) const
{
	return m_Location == rhs.m_Location
		&& m_Note == rhs.m_Note
		&& m_Verified == rhs.m_Verified
		&& m_Clubs == rhs.m_Clubs
		&& m_Runs == rhs.m_Runs;
}


size_t ARBDogTrial::GetSearchStrings(std::set<wxString>& ioStrings) const
{
	size_t nItems = 0;

	if (0 < m_Location.length())
	{
		ioStrings.insert(m_Location);
		++nItems;
	}

	if (0 < m_Note.length())
	{
		ioStrings.insert(m_Note);
		++nItems;
	}

	nItems += m_Clubs.GetSearchStrings(ioStrings);

	return nItems;
}


bool ARBDogTrial::Load(
		ARBConfig const& inConfig,
		ElementNodePtr inTree,
		ARBVersion const& inVersion,
		ARBErrorCallback& ioCallback)
{
	assert(inTree);
	if (!inTree || inTree->GetName() != TREE_TRIAL)
		return false;
	if (ElementNode::eInvalidValue == inTree->GetAttrib(ATTRIB_TRIAL_VERIFIED, m_Verified))
	{
		ioCallback.LogMessage(Localization()->ErrorInvalidAttributeValue(TREE_TRIAL, ATTRIB_TRIAL_VERIFIED, Localization()->ValidValuesBool()));
		return false;
	}
	for (int i = 0; i < inTree->GetElementCount(); ++i)
	{
		ElementNodePtr element = inTree->GetElementNode(i);
		if (!element)
			continue;
		if (element->GetName() == TREE_LOCATION)
		{
			m_Location = element->GetValue();
		}
		else if (element->GetName() == TREE_NOTE)
		{
			m_Note = element->GetValue();
		}
		else if (element->GetName() == TREE_CLUB)
		{
			// Ignore any errors...
			m_Clubs.Load(inConfig, element, inVersion, ioCallback);
		}
		// Clubs should all be loaded first. We're not going to validate that
		// though. If it's not true, someone has been messing with the file
		// directly and the ARBRunList::Load will fail.
		else if (element->GetName() == TREE_RUN)
		{
			// Ignore any errors...
			m_Runs.Load(inConfig, m_Clubs, element, inVersion, ioCallback);
		}
	}
	SetMultiQs(inConfig);
	m_Runs.sort();
	return true;
}


bool ARBDogTrial::Save(ElementNodePtr ioTree) const
{
	assert(ioTree);
	if (!ioTree)
		return false;
	ElementNodePtr trial = ioTree->AddElementNode(TREE_TRIAL);
	if (m_Verified) // Default is no
		trial->AddAttrib(ATTRIB_TRIAL_VERIFIED, m_Verified);
	if (0 < m_Location.length())
	{
		ElementNodePtr element = trial->AddElementNode(TREE_LOCATION);
		element->SetValue(m_Location);
	}
	if (0 < m_Note.length())
	{
		ElementNodePtr element = trial->AddElementNode(TREE_NOTE);
		element->SetValue(m_Note);
	}
	if (!m_Clubs.Save(trial))
		return false;
	if (!m_Runs.Save(trial))
		return false;
	return true;
}


void ARBDogTrial::SetMultiQs(ARBConfig const& inConfig)
{
	// First clear all settings.
	ARBDate date;
	for (date = m_Runs.GetStartDate();
		date <= m_Runs.GetEndDate();
		++date)
	{
		for (ARBDogRunList::iterator iterRun = m_Runs.begin(); iterRun != m_Runs.end(); ++iterRun)
		{
			(*iterRun)->ClearMultiQs();
		}
	}

	// Now get some needed info...
	ARBDogClubPtr pClub;
	if (!GetClubs().GetPrimaryClub(&pClub))
		return;
	ARBConfigVenuePtr pVenue;
	if (!inConfig.GetVenues().FindVenue(pClub->GetVenue(), &pVenue))
		return;
	if (0 == pVenue->GetMultiQs().size())
		return;

	// Then for each day in the trial, look for multiQs.
	for (date = m_Runs.GetStartDate();
		date <= m_Runs.GetEndDate();
		++date)
	{
		std::vector<ARBDogRunPtr> runs;
		for (ARBDogRunList::iterator iterRun = m_Runs.begin(); iterRun != m_Runs.end(); ++iterRun)
		{
			ARBDogRunPtr pRun = (*iterRun);
			if (pRun->GetDate() == date
			&& pRun->GetQ().Qualified())
			{
				runs.push_back(pRun);
			}
		}
		// Now, see if any combo of these runs matches a multiQ config.
		if (1 < runs.size())
		{
			for (ARBConfigMultiQList::iterator iterM = pVenue->GetMultiQs().begin();
				iterM != pVenue->GetMultiQs().end();
				++iterM)
			{
				ARBConfigMultiQPtr pMultiQ = *iterM;
				std::vector<ARBDogRunPtr> matchedRuns;
				if (pMultiQ->Match(runs, matchedRuns))
				{
					for (std::vector<ARBDogRunPtr>::iterator iter = matchedRuns.begin();
						iter != matchedRuns.end();
						++iter)
					{
						(*iter)->AddMultiQ(pMultiQ);
					}
				}
			}
		}
	}
}


short ARBDogTrial::GetSpeedPoints(
		ARBConfig const& inConfig,
		wxString const& inDiv,
		wxString const& inLevel) const
{
	if (!GetClubs().GetPrimaryClub())
		return 0;
	short speed = 0;
	for (ARBDogRunList::const_iterator iterRun = m_Runs.begin(); iterRun != m_Runs.end(); ++iterRun)
	{
		ARBDogRunPtr pRun = (*iterRun);
		if (pRun->GetDivision() == inDiv
		&& pRun->GetLevel() == inLevel)
		{
			ARBConfigScoringPtr pScoring;
			if (inConfig.GetVenues().FindEvent(
				GetClubs().GetPrimaryClubVenue(),
				pRun->GetEvent(),
				pRun->GetDivision(),
				pRun->GetLevel(),
				pRun->GetDate(),
				NULL,
				&pScoring))
			{
				speed = speed + pRun->GetSpeedPoints(pScoring);
			}
		}
	}
	return speed;
}


bool ARBDogTrial::HasVenue(wxString const& inVenue) const
{
	for (ARBDogClubList::const_iterator iter = m_Clubs.begin(); iter != m_Clubs.end(); ++iter)
	{
		if ((*iter)->GetVenue() == inVenue)
			return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool ARBDogTrialList::Load(
		ARBConfig const& inConfig,
		ElementNodePtr inTree,
		ARBVersion const& inVersion,
		ARBErrorCallback& ioCallback)
{
	ARBDogTrialPtr thing(ARBDogTrial::New());
	if (!thing->Load(inConfig, inTree, inVersion, ioCallback))
		return false;
	push_back(thing);
	return true;
}


class SortTrials
{
public:
	SortTrials(bool bDescending) : m_bDescending(bDescending) {}
	bool operator()(ARBDogTrialPtr one, ARBDogTrialPtr two) const
	{
		int iCompare = 0;
		if (0 < one->GetRuns().size()
		&& 0 < two->GetRuns().size())
		{
			ARBDate d1 = (*(one->GetRuns().begin()))->GetDate();
			ARBDate d2 = (*(two->GetRuns().begin()))->GetDate();
			if (d1 < d2)
				iCompare = -1;
			else if (d1 > d2)
				iCompare = 1;
		}
		else if (0 < one->GetRuns().size())
			iCompare = 1;
		else if (0 < two->GetRuns().size())
			iCompare = -1;
		if (0 == iCompare)
		{
			if (one->GetGenericName() < two->GetGenericName())
				iCompare = -1;
			else if (one->GetGenericName() > two->GetGenericName())
				iCompare = 1;
			else
			{
				ARBDogClubPtr club1;
				one->GetClubs().GetPrimaryClub(&club1);
				wxString name1, venue1;
				if (club1)
				{
					name1 = club1->GetName();
					venue1 = club1->GetVenue();
				}
				ARBDogClubPtr club2;
				two->GetClubs().GetPrimaryClub(&club2);
				wxString name2, venue2;
				if (club2)
				{
					name2 = club2->GetName();
					venue2 = club2->GetVenue();
				}
				if (name1 < name2)
					iCompare = -1;
				else if (name1 > name2)
					iCompare = 1;
				else if (venue1 < venue2)
					iCompare = -1;
				else if (venue1 > venue2)
					iCompare = 1;
			}
		}
		return m_bDescending ?  iCompare < 0 : iCompare > 0;
	}
private:
	bool m_bDescending;
};


void ARBDogTrialList::sort(bool inDescending)
{
	if (2 > size())
		return;
	std::stable_sort(begin(), end(), SortTrials(inDescending));
}


int ARBDogTrialList::NumTrialsInVenue(wxString const& inVenue) const
{
	int count = 0;
	for (const_iterator iter = begin(); iter != end(); ++iter)
	{
		for (ARBDogClubList::const_iterator iterClub = (*iter)->GetClubs().begin(); iterClub != (*iter)->GetClubs().end(); ++iterClub)
		{
			if ((*iterClub)->GetVenue() == inVenue)
			{
				++count;
				// We're counting the number of trials, not clubs.
				break;
			}
		}
	}
	return count;
}


int ARBDogTrialList::RenameVenue(
		wxString const& inOldVenue,
		wxString const& inNewVenue)
{
	int count = 0;
	for (iterator iter = begin(); iter != end(); ++iter)
	{
		for (ARBDogClubList::iterator iterClub = (*iter)->GetClubs().begin(); iterClub != (*iter)->GetClubs().end(); ++iterClub)
		{
			if ((*iterClub)->GetVenue() == inOldVenue)
			{
				(*iterClub)->SetVenue(inNewVenue);
				++count;
			}
		}
	}
	return count;
}


int ARBDogTrialList::DeleteVenue(wxString const& inVenue)
{
	wxString venue(inVenue);
	int count = 0;
	for (iterator iter = begin(); iter != end(); )
	{
		for (ARBDogClubList::iterator iterClub = (*iter)->GetClubs().begin(); iterClub != (*iter)->GetClubs().end(); )
		{
			if ((*iterClub)->GetVenue() == venue)
			{
				iterClub = (*iter)->GetClubs().erase(iterClub);
				++count;
			}
			else
				++iterClub;
		}
		if (0 == (*iter)->GetClubs().size())
			iter = erase(iter);
		else
			++iter;
	}
	return count;
}


int ARBDogTrialList::NumOtherPointsInUse(wxString const& inOther) const
{
	int count = 0;
	for (const_iterator iter = begin(); iter != end(); ++iter)
	{
		for (ARBDogRunList::const_iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); ++iterRun)
		{
			count += (*iterRun)->NumOtherPointsInUse(inOther);
		}
	}
	return count;
}


int ARBDogTrialList::RenameOtherPoints(
		wxString const& inOldOther,
		wxString const& inNewOther)
{
	int count = 0;
	for (const_iterator iter = begin(); iter != end(); ++iter)
	{
		for (ARBDogRunList::const_iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); ++iterRun)
		{
			count += (*iterRun)->RenameOtherPoints(inOldOther, inNewOther);
		}
	}
	return count;
}


int ARBDogTrialList::DeleteOtherPoints(wxString const& inOther)
{
	int count = 0;
	for (const_iterator iter = begin(); iter != end(); ++iter)
	{
		for (ARBDogRunList::const_iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); ++iterRun)
		{
			count += (*iterRun)->DeleteOtherPoints(inOther);
		}
	}
	return count;
}


int ARBDogTrialList::NumMultiHostedTrialsInDivision(
		ARBConfig const& inConfig,
		wxString const& inVenue,
		wxString const& inDiv) const
{
	int count = 0;
	for (const_iterator iter = begin(); iter != end(); ++iter)
	{
		if ((*iter)->GetClubs().FindVenue(inVenue)
		&& 1 < (*iter)->GetClubs().size())
		{
			int nDivCount = 0;
			for (ARBDogClubList::const_iterator iterClub = (*iter)->GetClubs().begin(); iterClub != (*iter)->GetClubs().end(); ++iterClub)
			{
				ARBConfigVenuePtr pVenue;
				if (inConfig.GetVenues().FindVenue((*iterClub)->GetVenue(), &pVenue))
				{
					if (pVenue->GetDivisions().FindDivision(inDiv))
						++nDivCount;
				}
			}
			if (1 < nDivCount)
				++count;
		}
	}
	return count;
}


int ARBDogTrialList::NumRunsInDivision(
		ARBConfigVenuePtr inVenue,
		wxString const& inDiv) const
{
	int count = 0;
	for (const_iterator iter = begin(); iter != end(); ++iter)
	{
		if ((*iter)->GetClubs().FindVenue(inVenue->GetName()))
		{
			for (ARBDogRunList::const_iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); ++iterRun)
			{
				if ((*iterRun)->GetDivision() == inDiv)
					++count;
			}
		}
	}
	return count;
}


int ARBDogTrialList::RenameDivision(
		ARBConfigVenuePtr inVenue,
		wxString const& inOldDiv,
		wxString const& inNewDiv)
{
	int count = 0;
	for (iterator iter = begin(); iter != end(); ++iter)
	{
		if ((*iter)->GetClubs().FindVenue(inVenue->GetName()))
		{
			for (ARBDogRunList::iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); ++iterRun)
			{
				if ((*iterRun)->GetDivision() == inOldDiv)
				{
					++count;
					(*iterRun)->SetDivision(inNewDiv);
				}
			}
		}
	}
	return count;
}


int ARBDogTrialList::DeleteDivision(
		ARBConfig const& inConfig,
		wxString const& inVenue,
		wxString const& inDiv)
{
	wxString venue(inVenue);
	wxString div(inDiv);
	int count = 0;
	for (iterator iter = begin(); iter != end(); )
	{
		if ((*iter)->GetClubs().FindVenue(venue))
		{
			int nDivCount = 0;
			// First, should we delete the run? If there are multiple hosting
			// clubs we need to look for the division name within each.
			for (ARBDogClubList::const_iterator iterClub = (*iter)->GetClubs().begin(); iterClub != (*iter)->GetClubs().end(); ++iterClub)
			{
				ARBConfigVenuePtr pVenue;
				if (inConfig.GetVenues().FindVenue((*iterClub)->GetVenue(), &pVenue))
				{
					if (pVenue->GetDivisions().FindDivision(div))
						++nDivCount;
				}
			}
			// If more than one hosting club has this division, no need to go
			// across the runs - we're not going to delete anything!
			if (1 == nDivCount)
			{
				for (ARBDogRunList::iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); )
				{
					if ((*iterRun)->GetDivision() == div)
					{
						++count;
						iterRun = (*iter)->GetRuns().erase(iterRun);
					}
					else
						++iterRun;
				}
			}
		}
		if (0 == (*iter)->GetRuns().size())
			iter = erase(iter);
		else
			++iter;
	}
	return count;
}


int ARBDogTrialList::NumLevelsInUse(
		wxString const& inVenue,
		wxString const& inDiv,
		wxString const& inLevel) const
{
	int count = 0;
	for (const_iterator iter = begin(); iter != end(); ++iter)
	{
		if ((*iter)->GetClubs().FindVenue(inVenue))
		{
			for (ARBDogRunList::iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); ++iterRun)
			{
				if ((*iterRun)->GetDivision() == inDiv
				&& (*iterRun)->GetLevel() == inLevel)
					++count;
			}
		}
	}
	return count;
}


int ARBDogTrialList::RenameLevel(
		wxString const& inVenue,
		wxString const& inDiv,
		wxString const& inOldLevel,
		wxString const& inNewLevel)
{
	int count = 0;
	for (const_iterator iter = begin(); iter != end(); ++iter)
	{
		if ((*iter)->GetClubs().FindVenue(inVenue))
		{
			for (ARBDogRunList::iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); ++iterRun)
			{
				if ((*iterRun)->GetDivision() == inDiv
				&& (*iterRun)->GetLevel() == inOldLevel)
				{
					++count;
					(*iterRun)->SetLevel(inNewLevel);
				}
			}
		}
	}
	return count;
}


int ARBDogTrialList::DeleteLevel(
		wxString const& inVenue,
		wxString const& inDiv,
		wxString const& inLevel)
{
	wxString venue(inVenue);
	wxString div(inDiv);
	wxString level(inLevel);
	int count = 0;
	for (iterator iter = begin(); iter != end(); )
	{
		if ((*iter)->GetClubs().FindVenue(venue))
		{
			for (ARBDogRunList::iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); )
			{
				if ((*iterRun)->GetDivision() == div
				&& (*iterRun)->GetLevel() == level)
				{
					++count;
					iterRun = (*iter)->GetRuns().erase(iterRun);
				}
				else
					++iterRun;
			}
		}
		if (0 == (*iter)->GetRuns().size())
			iter = erase(iter);
		else
			++iter;
	}
	return count;
}


int ARBDogTrialList::NumEventsInUse(
		wxString const& inVenue,
		wxString const& inEvent) const
{
	int count = 0;
	for (const_iterator iter = begin(); iter != end(); ++iter)
	{
		if ((*iter)->GetClubs().FindVenue(inVenue))
		{
			for (ARBDogRunList::iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); ++iterRun)
			{
				if ((*iterRun)->GetEvent() == inEvent)
					++count;
			}
		}
	}
	return count;
}


int ARBDogTrialList::RenameEvent(
		wxString const& inVenue,
		wxString const& inOldEvent,
		wxString const& inNewEvent)
{
	int count = 0;
	for (iterator iter = begin(); iter != end(); ++iter)
	{
		if ((*iter)->GetClubs().FindVenue(inVenue))
		{
			for (ARBDogRunList::iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); ++iterRun)
			{
				if ((*iterRun)->GetEvent() == inOldEvent)
				{
					++count;
					(*iterRun)->SetEvent(inNewEvent);
				}
			}
		}
	}
	return count;
}


int ARBDogTrialList::DeleteEvent(
		wxString const& inVenue,
		wxString const& inEvent)
{
	wxString venue(inVenue);
	wxString pEvent(inEvent);
	int count = 0;
	for (iterator iter = begin(); iter != end(); )
	{
		if ((*iter)->GetClubs().FindVenue(venue))
		{
			for (ARBDogRunList::iterator iterRun = (*iter)->GetRuns().begin(); iterRun != (*iter)->GetRuns().end(); )
			{
				if ((*iterRun)->GetEvent() == pEvent)
				{
					++count;
					iterRun = (*iter)->GetRuns().erase(iterRun);
				}
				else
					++iterRun;
			}
		}
		if (0 == (*iter)->GetRuns().size())
			iter = erase(iter);
		else
			++iter;
	}
	return count;
}


bool ARBDogTrialList::AddTrial(ARBDogTrialPtr inTrial)
{
	bool bAdded = false;
	if (inTrial)
	{
		bAdded = true;
		push_back(inTrial);
	}
	return bAdded;
}


bool ARBDogTrialList::DeleteTrial(ARBDogTrialPtr inTrial)
{
	if (inTrial)
	{
		for (iterator iter = begin(); iter != end(); ++iter)
		{
			if ((*iter) && *(*iter) == *inTrial)
			{
				erase(iter);
				return true;
			}
		}
	}
	return false;
}