#pragma once

/*
 * Copyright � 2002-2004 David Connet. All Rights Reserved.
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
 * @brief ARBConfigEvent and ARBConfigEventList classes.
 * @author David Connet
 *
 * Revision History
 * @li 2004-02-02 DRC Added VerifyEvent.
 * @li 2003-12-28 DRC Added GetSearchStrings.
 * @li 2003-12-27 DRC Changed FindEvent to take a date.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 */

#include <string>
#include "ARBBase.h"
#include "ARBConfigDivision.h"
#include "ARBConfigScoring.h"
#include "ARBVector.h"
class ARBVersion;
class Element;

/**
 * Event configuration information.
 */
class ARBConfigEvent : public ARBBase
{
public:
	ARBConfigEvent();
	ARBConfigEvent(const ARBConfigEvent& rhs);
	ARBConfigEvent& operator=(const ARBConfigEvent& rhs);
	bool operator==(const ARBConfigEvent& rhs) const;
	bool operator!=(const ARBConfigEvent& rhs) const;

	/**
	 * Get the generic name of this object.
	 * @return The generic name of this object.
	 */
	virtual std::string GetGenericName() const;

	/**
	 * Get all the strings to search in this object.
	 * @param ioStrings Accumulated list of strings to be used during a search.
	 * @return Number of strings accumulated in this object.
	 */
	virtual size_t GetSearchStrings(std::set<std::string>& ioStrings) const;

	/**
	 * Load a event configuration.
	 * @pre inTree is the actual ARBConfigEvent element.
	 * @param inDivisions Used to verify division/level during load.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioErrMsg Accumulated error messages.
	 * @return Success
	 */
	bool Load(
		const ARBConfigDivisionList& inDivisions,
		const Element& inTree,
		const ARBVersion& inVersion,
		std::string& ioErrMsg);

	/**
	 * Save a document.
	 * @param ioTree Parent element.
	 * @return Success
	 * @post The ARBConfigEvent element will be created in ioTree.
	 */
	bool Save(Element& ioTree) const;

	/**
	 * Update this configuration from inEventNew.
	 * @param indent Indentation level for generating messages.
	 * @param inEventNew Event to merge.
	 * @param ioInfo Accumulated messages about changes that have happened.
	 * @return Whether or not changes have occurred.
	 */
	bool Update(int indent, const ARBConfigEvent* inEventNew, std::string& ioInfo);

	/**
	 * Find all the scoring methods that match.
	 * @param inDivision Division event is in.
	 * @param inLevel Level (NOT sublevel) event is in.
	 * @param inTitlePoints Only return scoring methods that have title points.
	 * @param outList List of scoring methods found.
	 * @return Number of items found.
	 */
	size_t FindAllEvents(
		const std::string& inDivision,
		const std::string& inLevel,
		bool inTitlePoints,
		std::vector<const ARBConfigScoring*>& outList) const;

	/**
	 * Verify a scoring method exists.
	 * @param inDivision Division event is in.
	 * @param inLevel Level event is in.
	 * @return true if FindAllEvents() > 0.
	 */
	bool VerifyEvent(
		const std::string& inDivision,
		const std::string& inLevel) const;

	/**
	 * Find an event.
	 * @param inDivision Division event exists in.
	 * @param inLevel Level (NOT sublevel) event exists in.
	 * @param inDate Date for requested scoring.
	 * @return Pointer to object, NULL if not found.
	 * @post Returned pointer is not ref counted, do <b><i>not</i></b> release.
	 */
	const ARBConfigScoring* FindEvent(
		const std::string& inDivision,
		const std::string& inLevel,
		const ARBDate& inDate) const;

	/*
	 * Getters/setters.
	 */
	const std::string& GetName() const;
	void SetName(const std::string& inName);
	const std::string& GetDesc() const;
	void SetDesc(const std::string& inDesc);
	bool HasPartner() const;
	void SetHasPartner(bool inHas);
	const ARBConfigScoringList& GetScorings() const;
	ARBConfigScoringList& GetScorings();

private:
	~ARBConfigEvent();
	std::string m_Name;
	std::string m_Desc;
	bool m_bHasPartner;
	ARBConfigScoringList m_Scoring;
};

inline std::string ARBConfigEvent::GetGenericName() const
{
	return m_Name;
}

inline const std::string& ARBConfigEvent::GetName() const
{
	return m_Name;
}

inline void ARBConfigEvent::SetName(const std::string& inName)
{
	m_Name = inName;
}

inline const std::string& ARBConfigEvent::GetDesc() const
{
	return m_Desc;
}

inline void ARBConfigEvent::SetDesc(const std::string& inDesc)
{
	m_Desc = inDesc;
}

inline bool ARBConfigEvent::HasPartner() const
{
	return m_bHasPartner;
}

inline void ARBConfigEvent::SetHasPartner(bool inHas)
{
	m_bHasPartner = inHas;
}

inline const ARBConfigScoringList& ARBConfigEvent::GetScorings() const
{
	return m_Scoring;
}

inline ARBConfigScoringList& ARBConfigEvent::GetScorings()
{
	return m_Scoring;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * List of ARBConfigEvent objects.
 */
class ARBConfigEventList : public ARBVector<ARBConfigEvent>
{
public:
	/**
	 * Load an event configuration.
	 * @pre inTree is the actual ARBConfigEvent element.
	 * @param inDivisions Configurate division used for verification.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioErrMsg Accumulated error messages.
	 * @return Success
	 */
	bool Load(
		const ARBConfigDivisionList& inDivisions,
		const Element& inTree,
		const ARBVersion& inVersion,
		std::string& ioErrMsg);

	bool operator==(const ARBConfigEventList& rhs) const
	{
		return isEqual(rhs);
	}
	bool operator!=(const ARBConfigEventList& rhs) const
	{
		return !isEqual(rhs);
	}

	/**
	 * Verify an event exists.
	 * @param inEvent Event to verify.
	 * @param inDivision Division event exists in.
	 * @param inLevel Level event exists in.
	 * @return Event exists.
	 */
	bool VerifyEvent(
		const std::string& inEvent,
		const std::string& inDivision,
		const std::string& inLevel) const;

	/**
	 * Find an event.
	 * @param inEvent Event to find.
	 * @param inDivision Division event exists in.
	 * @param inLevel Level event exists in.
	 * @param inDate Date for requested scoring.
	 * @return Pointer to object, NULL if not found.
	 * @post Returned pointer is not ref counted, do <b><i>not</i></b> release.
	 */
	const ARBConfigScoring* FindEvent(
		const std::string& inEvent,
		const std::string& inDivision,
		const std::string& inLevel,
		const ARBDate& inDate) const;

	/**
	 * Rename a division.
	 * @param inOldDiv Division name being renamed.
	 * @param inNewDiv New division name.
	 * @return Number of ARBConfigScoring items updated.
	 */
	int RenameDivision(
		const std::string& inOldDiv,
		const std::string& inNewDiv);

	/**
	 * Delete a division.
	 * @param inDiv Division name being deleted.
	 * @return Number of ARBConfigScoring items removed.
	 */
	int DeleteDivision(const std::string& inDiv);

	/**
	 * Rename a level.
	 * @param inOldLevel Level name being renamed.
	 * @param inNewLevel New level name.
	 * @return Number of ARBConfigScoring items updated.
	 */
	int RenameLevel(
		const std::string& inOldLevel,
		const std::string& inNewLevel);

	/**
	 * Delete a level.
	 * @param inLevel Level name being deleted.
	 * @return Number of ARBConfigScoring items removed.
	 */
	int DeleteLevel(const std::string& inLevel);

	/**
	 * Find an event.
	 * @param inEvent Name of event to find.
	 * @return Pointer to object, NULL if not found.
	 * @post Returned pointer is not ref counted, do <b><i>not</i></b> release.
	 */
	const ARBConfigEvent* FindEvent(const std::string& inEvent) const;
	ARBConfigEvent* FindEvent(const std::string& inEvent);

	/**
	 * Add an event.
	 * @param inEvent Event to add.
	 * @return Pointer to object, NULL if name already exists or is empty.
	 * @post Returned pointer is not ref counted, do <b><i>not</i></b> release.
	 *       The pointer is added to the list and its ref count is incremented.
	 */
	ARBConfigEvent* AddEvent(ARBConfigEvent* inEvent);

	/**
	 * Delete an event.
	 * @param inEvent Name of event to delete.
	 * @return Whether event was deleted or not.
	 */
	bool DeleteEvent(const std::string& inEvent);
};
