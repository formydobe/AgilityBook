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
 * @brief ARBConfigScoring and ARBConfigScoringList class.
 * @author David Connet
 *
 * Revision History
 * @li 2004-09-28 DRC Changed how error reporting is done when loading.
 * @li 2004-02-02 DRC Added VerifyEvent.
 * @li 2003-12-28 DRC Added GetSearchStrings.
 * @li 2003-12-27 DRC Added ValidTo, changed ValidFrom dtd name (was 'Date').
 * @li 2003-12-11 DRC Added ValidFrom, not fully implemented yet.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 */

#include <string>
#include <vector>
#include "ARBBase.h"
#include "ARBConfigDivision.h"
#include "ARBConfigTitlePoints.h"
#include "ARBDate.h"
#include "ARBVector.h"
class ARBConfigEvent;
class ARBErrorCallback;
class ARBVersion;
class Element;

/**
 * Scoring methods for an event.
 */
class ARBConfigScoring : public ARBBase
{
public:
	/**
	 * Types of scoring methods.
	 */
	typedef enum
	{
		eUnknown,			///< Unknown scoring method.
		eFaultsThenTime,	///< By faults then time.
		eFaults100ThenTime,	///< By faults then time (AKC).
		eFaults200ThenTime,	///< By faults then time (UKC).
		eOCScoreThenTime,	///< Open/Closing points then time.
		eScoreThenTime,		///< Points then time.
		eTimePlusFaults		///< Time plus faults.
	} ScoringStyle;

	/**
	 * Translate the enum to a string.
	 */
	static std::string GetScoringStyleStr(ScoringStyle inStyle);

	ARBConfigScoring();
	ARBConfigScoring(ARBConfigScoring const& rhs);
	ARBConfigScoring& operator=(ARBConfigScoring const& rhs);
	bool operator==(ARBConfigScoring const& rhs) const;
	bool operator!=(ARBConfigScoring const& rhs) const;

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
	 * Load a scoring configuration.
	 * @pre inTree is the actual ARBConfigScoring element.
	 * @param inDivisions Configurate division used for verification.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioCallback Error processing callback.
	 * @return Success
	 */
	bool Load(
		ARBConfigDivisionList const& inDivisions,
		Element const& inTree,
		ARBVersion const& inVersion,
		ARBErrorCallback& ioCallback);

	/**
	 * Save a document.
	 * @param ioTree Parent element.
	 * @return Success
	 * @post The ARBConfigOtherPoints element will be created in ioTree.
	 */
	bool Save(Element& ioTree) const;

	/*
	 * Getters/setters.
	 */
	ARBDate const& GetValidFrom() const;
	void SetValidFrom(ARBDate const& inDate);
	ARBDate const& GetValidTo() const;
	void SetValidTo(ARBDate const& inDate);
	std::string GetValidDateString() const;
	std::string const& GetDivision() const;
	void SetDivision(std::string const& inDiv);
	std::string const& GetLevel() const;
	void SetLevel(std::string const& inLevel);
	ScoringStyle GetScoringStyle() const;
	std::string GetScoringStyleStr() const;
	void SetScoringStyle(ARBConfigScoring::ScoringStyle inStyle);
	bool DropFractions() const;
	void SetDropFractions(bool inBool);
	std::string const& GetNote() const;
	void SetNote(std::string const& inNote);
	short GetRequiredOpeningPoints() const;
	void SetRequiredOpeningPoints(short inPoints);
	short GetRequiredClosingPoints() const;
	void SetRequiredClosingPoints(short inPoints);
	bool HasSuperQ() const;
	void SetHasSuperQ(bool inBool);
	bool HasDoubleQ() const;
	void SetHasDoubleQ(bool inBool);
	bool HasMachPts() const;
	void SetHasMachPts(bool inBool);
	ARBConfigTitlePointsList const& GetTitlePoints() const;
	ARBConfigTitlePointsList& GetTitlePoints();

private:
	~ARBConfigScoring();
	ARBDate m_ValidFrom;
	ARBDate m_ValidTo;
	std::string m_Division;
	std::string m_Level;
	ScoringStyle m_Style;
	bool m_bDropFractions;
	std::string m_Note;
	short m_OpeningPts;
	short m_ClosingPts;
	bool m_bSuperQ;
	bool m_bDoubleQ;
	bool m_bMachPts;
	ARBConfigTitlePointsList m_TitlePoints;
};

inline ARBDate const& ARBConfigScoring::GetValidFrom() const
{
	return m_ValidFrom;
}

inline void ARBConfigScoring::SetValidFrom(ARBDate const& inDate)
{
	m_ValidFrom = inDate;
}

inline ARBDate const& ARBConfigScoring::GetValidTo() const
{
	return m_ValidTo;
}

inline void ARBConfigScoring::SetValidTo(ARBDate const& inDate)
{
	m_ValidTo = inDate;
}

inline std::string const& ARBConfigScoring::GetDivision() const
{
	return m_Division;
}

inline void ARBConfigScoring::SetDivision(std::string const& inDiv)
{
	m_Division = inDiv;
}

inline std::string const& ARBConfigScoring::GetLevel() const
{
	return m_Level;
}

inline void ARBConfigScoring::SetLevel(std::string const& inLevel)
{
	m_Level = inLevel;
}

inline ARBConfigScoring::ScoringStyle ARBConfigScoring::GetScoringStyle() const
{
	return m_Style;
}

inline void ARBConfigScoring::SetScoringStyle(ARBConfigScoring::ScoringStyle inStyle)
{
	m_Style = inStyle;
	if (eOCScoreThenTime != m_Style && eScoreThenTime != m_Style)
		m_OpeningPts = m_ClosingPts = 0;
}

inline bool ARBConfigScoring::DropFractions() const
{
	return m_bDropFractions;
}

inline void ARBConfigScoring::SetDropFractions(bool inBool)
{
	m_bDropFractions = inBool;
}

inline std::string const& ARBConfigScoring::GetNote() const
{
	return m_Note;
}

inline void ARBConfigScoring::SetNote(std::string const& inNote)
{
	m_Note = inNote;
}

inline short ARBConfigScoring::GetRequiredOpeningPoints() const
{
	return m_OpeningPts;
}

inline void ARBConfigScoring::SetRequiredOpeningPoints(short inPoints)
{
	m_OpeningPts = inPoints;
}

inline short ARBConfigScoring::GetRequiredClosingPoints() const
{
	return m_ClosingPts;
}

inline void ARBConfigScoring::SetRequiredClosingPoints(short inPoints)
{
	m_ClosingPts = inPoints;
}

inline bool ARBConfigScoring::HasSuperQ() const
{
	return m_bSuperQ;
}

inline void ARBConfigScoring::SetHasSuperQ(bool inBool)
{
	m_bSuperQ = inBool;
}

inline bool ARBConfigScoring::HasDoubleQ() const
{
	return m_bDoubleQ;
}

inline void ARBConfigScoring::SetHasDoubleQ(bool inBool)
{
	m_bDoubleQ = inBool;
}

inline bool ARBConfigScoring::HasMachPts() const
{
	return m_bMachPts;
}

inline void ARBConfigScoring::SetHasMachPts(bool inBool)
{
	m_bMachPts = inBool;
}

inline ARBConfigTitlePointsList const& ARBConfigScoring::GetTitlePoints() const
{
	return m_TitlePoints;
}

inline ARBConfigTitlePointsList& ARBConfigScoring::GetTitlePoints()
{
	return m_TitlePoints;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * List of ARBConfigScoring objects.
 */
class ARBConfigScoringList : public ARBVector<ARBConfigScoring>
{
public:
	/**
	 * Load a scoring configuration.
	 * @pre inTree is the actual ARBConfigScoring element.
	 * @param inDivisions Configurate division used for verification.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioCallback Error processing callback.
	 * @return Success
	 */
	bool Load(
		ARBConfigDivisionList const& inDivisions,
		Element const& inTree,
		ARBVersion const& inVersion,
		ARBErrorCallback& ioCallback);

	/**
	 * Find all the scoring methods that match.
	 * @param inDivision Division event is in.
	 * @param inLevel Level (NOT sublevel) event is in.
	 * @param inTitlePoints Only return scoring methods that have title points.
	 * @param outList List of scoring methods found.
	 * @return Number of items found.
	 */
	size_t FindAllEvents(
		std::string const& inDivision,
		std::string const& inLevel,
		bool inTitlePoints,
		std::vector<ARBConfigScoring const*>& outList) const;

	/**
	 * Find an event.
	 * @param inDivision Division event exists in.
	 * @param inLevel Level (NOT sublevel) event exists in.
	 * @param inDate Date for requested scoring.
	 * @return Pointer to object, NULL if not found.
	 * @post Returned pointer is not ref counted, do <b><i>not</i></b> release.
	 */
	ARBConfigScoring const* FindEvent(
		std::string const& inDivision,
		std::string const& inLevel,
		ARBDate const& inDate) const;

	/**
	 * Verify a scoring method exists.
	 * @param inDivision Division event is in.
	 * @param inLevel Level event is in.
	 * @return true if FindAllEvents() > 0.
	 */
	bool VerifyEvent(
		std::string const& inDivision,
		std::string const& inLevel) const;

	/**
	 * Create a new scoring method.
	 * @return Pointer to a new method.
	 * @post Returned pointer is not ref counted, do <b><i>not</i></b> release.
	 */
	ARBConfigScoring* AddScoring();
};
