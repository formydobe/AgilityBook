#pragma once

/*
 * Copyright � 2002-2006 David Connet. All Rights Reserved.
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
 * @brief ARBConfigVenue and ARBConfigVenueList classes.
 * @author David Connet
 *
 * Revision History
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-12-14 DRC Moved 'Titles' to 'Venue'.
 * @li 2005-06-25 DRC Cleaned up reference counting when returning a pointer.
 * @li 2004-09-28 DRC Changed how error reporting is done when loading.
 * @li 2004-02-02 DRC Added VerifyEvent.
 * @li 2003-12-28 DRC Added GetSearchStrings.
 * @li 2003-12-27 DRC Changed FindEvent to take a date.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 */

#include "ARBBase.h"
#include "ARBConfigDivision.h"
#include "ARBConfigEvent.h"
#include "ARBConfigMultiQ.h"
#include "ARBConfigTitle.h"
#include "ARBTypes.h"
class ARBDate;

/**
 * Venue configuration information.
 */
class ARBConfigVenue : public ARBBase
{
protected:
	ARBConfigVenue();
	ARBConfigVenue(ARBConfigVenue const& rhs);

public:
	~ARBConfigVenue();
	static ARBConfigVenuePtr New()
	{
		return ARBConfigVenuePtr(new ARBConfigVenue());
	}
	ARBConfigVenuePtr Clone() const
	{
		return ARBConfigVenuePtr(new ARBConfigVenue(*this));
	}

	ARBConfigVenue& operator=(ARBConfigVenue const& rhs);

	bool operator==(ARBConfigVenue const& rhs) const;
	bool operator!=(ARBConfigVenue const& rhs) const
	{
		return !operator==(rhs);
	}

	/**
	 * Reset the contents of this object and all sub-objects.
	 */
	void clear();

	/**
	 * Get the generic name of this object.
	 * @return The generic name of this object.
	 */
	virtual ARBString GetGenericName() const
	{
		return m_Name;
	}

	/**
	 * Get all the strings to search in this object.
	 * @param ioStrings Accumulated list of strings to be used during a search.
	 * @return Number of strings accumulated in this object.
	 */
	virtual size_t GetSearchStrings(std::set<ARBString>& ioStrings) const
	{
		return 0;
	}

	/**
	 * Load a venue configuration.
	 * @pre inTree is the actual ARBConfigVenue element.
	 * @param ioConfig When migrating an old file, we may be updating the config.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioCallback Error processing callback.
	 * @return Success
	 */
	bool Load(
			ARBConfig& ioConfig,
			Element const& inTree,
			ARBVersion const& inVersion,
			ARBErrorCallback& ioCallback);

	/**
	 * Save a document.
	 * @param ioTree Parent element.
	 * @return Success
	 * @post The ARBConfigDivision element will be created in ioTree.
	 */
	bool Save(Element& ioTree) const;

	/**
	 * Update this configuration from inVenueNew.
	 * @param indent Indentation level for generating messages.
	 * @param inVenueNew Venue to merge.
	 * @param ioInfo Accumulated messages about changes that have happened.
	 * @return Whether or not changes have occurred.
	 */
	bool Update(
			int indent,
			ARBConfigVenuePtr inVenueNew,
			ARBString& ioInfo);

	/*
	 * Getters/setters.
	 */
	ARBString const& GetName() const
	{
		return m_Name;
	}
	void SetName(ARBString const& inName)
	{
		m_Name = inName;
	}
	ARBString const& GetLongName() const
	{
		return m_LongName;
	}
	void SetLongName(ARBString const& inName)
	{
		m_LongName = inName;
	}
	ARBString const& GetDesc() const
	{
		return m_Desc;
	}
	void SetDesc(ARBString const& inDesc)
	{
		m_Desc = inDesc;
	}
	ARBConfigTitleList const& GetTitles() const
	{
		return m_Titles;
	}
	ARBConfigTitleList& GetTitles()
	{
		return m_Titles;
	}
	ARBConfigDivisionList const& GetDivisions() const
	{
		return m_Divisions;
	}
	ARBConfigDivisionList& GetDivisions()
	{
		return m_Divisions;
	}
	ARBConfigEventList const& GetEvents() const
	{
		return m_Events;
	}
	ARBConfigEventList& GetEvents()
	{
		return m_Events;
	}
	ARBConfigMultiQList const& GetMultiQs() const
	{
		return m_MultiQs;
	}
	ARBConfigMultiQList& GetMultiQs()
	{
		return m_MultiQs;
	}

private:
	ARBString m_Name;
	ARBString m_LongName;
	ARBString m_Desc;
	ARBConfigTitleList m_Titles;
	ARBConfigDivisionList m_Divisions;
	ARBConfigEventList m_Events;
	ARBConfigMultiQList m_MultiQs;
};

/////////////////////////////////////////////////////////////////////////////

/**
 * List of ARBConfigVenue objects.
 */
class ARBConfigVenueList : public ARBVector<ARBConfigVenuePtr>
{
public:
	/**
	 * Load a venue configuration.
	 * @pre inTree is the actual ARBConfigVenue element.
	 * @param ioConfig When migrating an old file, we may be updating the config.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioCallback Error processing callback.
	 * @return Success
	 */
	bool Load(
			ARBConfig& ioConfig,
			Element const& inTree,
			ARBVersion const& inVersion,
			ARBErrorCallback& ioCallback);

	/**
	 * Sort the list by name.
	 */
	void sort();

	/**
	 * Verify a venue exists.
	 * @param inVenue Venue to verify.
	 * @return Venue exists.
	 */
	bool VerifyVenue(ARBString const& inVenue) const
	{
		return FindVenue(inVenue);
	}

	/**
	 * Verify a multiQ exists.
	 * @param inVenue Venue to verify.
	 * @param inMultiQ MultiQ to verify.
	 * @param inUseShortName Search by shortname instead of longname.
	 * @return MultiQ exists.
	 */
	bool VerifyMultiQ(
			ARBString const& inVenue,
			ARBString const& inMultiQ,
			bool inUseShortName = false) const;

	/**
	 * Verify a level exists.
	 * @param inVenue Venue level exists in.
	 * @param inDivision Division level exists in.
	 * @param inLevel Level to verify.
	 * @return Level exists.
	 */
	bool VerifyLevel(
			ARBString const& inVenue,
			ARBString const& inDivision,
			ARBString const& inLevel) const;

	/**
	 * Verify an event exists.
	 * @param inVenue Venue event exists in.
	 * @param inDivision Division event exists in.
	 * @param inLevel Level event exists in.
	 * @param inEvent Event to verify.
	 * @param inDate Date for matching methods.
	 * @return Event exists.
	 */
	bool VerifyEvent(
			ARBString const& inVenue,
			ARBString const& inDivision,
			ARBString const& inLevel,
			ARBString const& inEvent,
			ARBDate const& inDate) const;

	/**
	 * Find a title by the complete name.
	 * This api is used to fix a problem introduced in v1.0.0.8.
	 * @param inVenue Venue title is in.
	 * @param inName Complete name of title to find.
	 * @param bAbbrevFirst Name is before or after Longname.
	 * @param outTitle Pointer to found object, NULL if not found.
	 * @return Whether the object was found.
	 */
	bool FindTitleCompleteName(
			ARBString const& inVenue,
			ARBString const& inName,
			bool bAbbrevFirst = true,
			ARBConfigTitlePtr* outTitle = NULL) const;

	/**
	 * Find a title.
	 * @param inVenue Venue title is in.
	 * @param inTitle Name of title to find.
	 * @param outTitle Pointer to found object, NULL if not found.
	 * @return Whether the object was found.
	 */
	bool FindTitle(
			ARBString const& inVenue,
			ARBString const& inTitle,
			ARBConfigTitlePtr* outTitle = NULL) const;

	/**
	 * Delete a title.
	 * @param inTitle Name of title to delete.
	 * @return Whether title was deleted.
	 */
	bool DeleteTitle(ARBString const& inTitle);

	/**
	 * Find the named venue.
	 * @param inVenue Venue to find.
	 * @param outVenue Object that was found.
	 * @return Whether the object was found.
	 */
	bool FindVenue(
			ARBString const& inVenue,
			ARBConfigVenuePtr* outVenue = NULL) const;

	/**
	 * Add a venue.
	 * @param inVenue Name of venue to add.
	 * @param outVenue Pointer to new object, NULL if name already exists or is empty.
	 * @return Whether the object was added.
	 */
	bool AddVenue(
			ARBString const& inVenue,
			ARBConfigVenuePtr* outVenue = NULL);

	/**
	 * Add a venue.
	 * @param inVenue Venue to add.
	 * @return Whether the object was added.
	 */
	bool AddVenue(ARBConfigVenuePtr inVenue);

	/**
	 * Delete a venue.
	 * @param inVenue Name of venue to delete.
	 * @return Number of venues deleted (0 or 1).
	 */
	int DeleteVenue(ARBString const& inVenue);

	/**
	 * Find an event.
	 * @param inVenue Venue event is in.
	 * @param inEvent Event name
	 * @param inDivision Division name
	 * @param inLevel True Level name (sublevel)
	 * @param inDate Date for requested scoring.
	 * @param outScoring Pointer to object, NULL if not found.
	 * @return Whether the object was found.
	 */
	bool FindEvent(
			ARBString const& inVenue,
			ARBString const& inEvent,
			ARBString const& inDivision,
			ARBString const& inLevel,
			ARBDate const& inDate,
			ARBConfigScoringPtr* outScoring = NULL) const;
};
