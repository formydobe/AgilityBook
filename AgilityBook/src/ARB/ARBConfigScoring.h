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
 * @brief ARBConfigScoring and ARBConfigScoringList class.
 * @author David Connet
 *
 * Revision History
 * @li 2005-12-04 DRC Added support for NADAC bonus titling points.
 * @li 2005-06-25 DRC Cleaned up reference counting when returning a pointer.
 * @li 2005-01-01 DRC Renamed MachPts to SpeedPts.
 * @li 2004-12-18 DRC Added a time fault multiplier.
 * @li 2004-11-15 DRC Added time fault computation on T+F.
 * @li 2004-09-28 DRC Changed how error reporting is done when loading.
 * @li 2004-02-02 DRC Added VerifyEvent.
 * @li 2003-12-28 DRC Added GetSearchStrings.
 * @li 2003-12-27 DRC Added ValidTo, changed ValidFrom dtd name (was 'Date').
 * @li 2003-12-11 DRC Added ValidFrom, not fully implemented yet.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 */

#include "ARBBase.h"
#include "ARBConfigDivision.h"
#include "ARBConfigLifetimePoints.h"
#include "ARBConfigTitlePoints.h"
#include "ARBDate.h"
#include "ARBTypes.h"
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
	static ARBString GetScoringStyleStr(ScoringStyle inStyle);

protected:
	ARBConfigScoring();
	ARBConfigScoring(ARBConfigScoring const& rhs);

public:
	~ARBConfigScoring();
	static ARBConfigScoringPtr New();
	ARBConfigScoringPtr Clone() const;

	ARBConfigScoring& operator=(ARBConfigScoring const& rhs);

	bool operator==(ARBConfigScoring const& rhs) const;
	bool operator!=(ARBConfigScoring const& rhs) const;

	/**
	 * Get the generic name of this object.
	 * @return The generic name of this object.
	 */
	virtual ARBString GetGenericName() const;

	/**
	 * Get all the strings to search in this object.
	 * @param ioStrings Accumulated list of strings to be used during a search.
	 * @return Number of strings accumulated in this object.
	 */
	virtual size_t GetSearchStrings(std::set<ARBString>& ioStrings) const;

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

	/**
	 * Determine if this method is valid on the given date.
	 * @param inDate Date to check, if not valid, this method is valid.
	 */
	bool IsValidOn(ARBDate inDate) const;

	/*
	 * Getters/setters.
	 */
	ARBDate const& GetValidFrom() const;
	void SetValidFrom(ARBDate const& inDate);
	ARBDate const& GetValidTo() const;
	void SetValidTo(ARBDate const& inDate);
	ARBString const& GetDivision() const;
	void SetDivision(ARBString const& inDiv);
	ARBString const& GetLevel() const;
	void SetLevel(ARBString const& inLevel);
	ScoringStyle GetScoringStyle() const;
	ARBString GetScoringStyleStr() const;
	void SetScoringStyle(ARBConfigScoring::ScoringStyle inStyle);
	bool DropFractions() const; ///< Only valid for F/T, T+F
	void SetDropFractions(bool inBool); ///< Only valid for F/T, T+F
	bool QsMustBeClean() const; ///< Only valid for T+F
	void SetQsMustBeClean(bool inBool); ///< Only valid for T+F
	bool ComputeTimeFaultsUnder() const;
	void SetComputeTimeFaultsUnder(bool inBool);
	bool ComputeTimeFaultsOver() const;
	void SetComputeTimeFaultsOver(bool inBool);
	short TimeFaultMultiplier() const;
	void SetTimeFaultMultiplier(short inMultiplier);
	short GetRequiredOpeningPoints() const; ///< Only valid for point-based
	void SetRequiredOpeningPoints(short inPoints); ///< Only valid for point-based
	short GetRequiredClosingPoints() const; ///< Only valid for point-based
	void SetRequiredClosingPoints(short inPoints); ///< Only valid for point-based
	ARBString const& GetNote() const;
	void SetNote(ARBString const& inNote);
	bool HasSuperQ() const;
	void SetHasSuperQ(bool inBool);
	bool HasSpeedPts() const;
	void SetHasSpeedPts(bool inBool);
	bool HasBonusPts() const;
	void SetHasBonusPts(bool inBool);
	ARBConfigTitlePointsList const& GetTitlePoints() const;
	ARBConfigTitlePointsList& GetTitlePoints();
	ARBConfigLifetimePointsList const& GetLifetimePoints() const;
	ARBConfigLifetimePointsList& GetLifetimePoints();

	/**
	 * Obsolete, used only for converting old files.
	 * This information is now contained in the venue (see ARBConfigMultiQ).
	 */
	bool ConvertDoubleQ() const;

private:
	ARBDate m_ValidFrom;
	ARBDate m_ValidTo;
	ARBString m_Division;
	ARBString m_Level;
	ScoringStyle m_Style;
	bool m_bDropFractions;
	bool m_bCleanQ;
	bool m_bTimeFaultsUnder;
	bool m_bTimeFaultsOver;
	short m_TimeFaultMultiplier;
	ARBString m_Note;
	short m_OpeningPts;
	short m_ClosingPts;
	bool m_bSuperQ;
	bool m_bDoubleQ;
	bool m_bSpeedPts;
	bool m_bBonusPts;
	ARBConfigTitlePointsList m_TitlePoints;
	ARBConfigLifetimePointsList m_LifePoints;
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

inline ARBString const& ARBConfigScoring::GetDivision() const
{
	return m_Division;
}

inline void ARBConfigScoring::SetDivision(ARBString const& inDiv)
{
	m_Division = inDiv;
}

inline ARBString const& ARBConfigScoring::GetLevel() const
{
	return m_Level;
}

inline void ARBConfigScoring::SetLevel(ARBString const& inLevel)
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

inline bool ARBConfigScoring::QsMustBeClean() const
{
	return m_bCleanQ;
}

inline void ARBConfigScoring::SetQsMustBeClean(bool inBool)
{
	m_bCleanQ = inBool;
}

inline bool ARBConfigScoring::ComputeTimeFaultsUnder() const
{
	return m_bTimeFaultsUnder;
}

inline void ARBConfigScoring::SetComputeTimeFaultsUnder(bool inBool)
{
	m_bTimeFaultsUnder = inBool;
}

inline bool ARBConfigScoring::ComputeTimeFaultsOver() const
{
	return m_bTimeFaultsOver;
}

inline void ARBConfigScoring::SetComputeTimeFaultsOver(bool inBool)
{
	m_bTimeFaultsOver = inBool;
}

inline short ARBConfigScoring::TimeFaultMultiplier() const
{
	return m_TimeFaultMultiplier;
}

inline void ARBConfigScoring::SetTimeFaultMultiplier(short inMultiplier)
{
	m_TimeFaultMultiplier = inMultiplier;
	if (0 >= m_TimeFaultMultiplier)
		m_TimeFaultMultiplier = 1;
}

inline ARBString const& ARBConfigScoring::GetNote() const
{
	return m_Note;
}

inline void ARBConfigScoring::SetNote(ARBString const& inNote)
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

inline bool ARBConfigScoring::HasSpeedPts() const
{
	return m_bSpeedPts;
}

inline void ARBConfigScoring::SetHasSpeedPts(bool inBool)
{
	m_bSpeedPts = inBool;
}

inline bool ARBConfigScoring::HasBonusPts() const
{
	return m_bBonusPts;
}

inline void ARBConfigScoring::SetHasBonusPts(bool inBool)
{
	m_bBonusPts = inBool;
}

inline ARBConfigTitlePointsList const& ARBConfigScoring::GetTitlePoints() const
{
	return m_TitlePoints;
}

inline ARBConfigTitlePointsList& ARBConfigScoring::GetTitlePoints()
{
	return m_TitlePoints;
}

inline ARBConfigLifetimePointsList const& ARBConfigScoring::GetLifetimePoints() const
{
	return m_LifePoints;
}

inline ARBConfigLifetimePointsList& ARBConfigScoring::GetLifetimePoints()
{
	return m_LifePoints;
}

inline bool ARBConfigScoring::ConvertDoubleQ() const
{
	return m_bDoubleQ;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * List of ARBConfigScoring objects.
 */
class ARBConfigScoringList : public ARBVector<ARBConfigScoringPtr>
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
	 * @param inDate Date for matching methods.
	 * @param inTitlePoints Only return scoring methods that have title points.
	 * @param outList List of scoring methods found.
	 * @return Number of items found.
	 */
	size_t FindAllEvents(
			ARBString const& inDivision,
			ARBString const& inLevel,
			ARBDate const& inDate,
			bool inTitlePoints,
			ARBVector<ARBConfigScoringPtr>& outList) const;

	/**
	 * Find an event.
	 * @param inDivision Division event exists in.
	 * @param inLevel Level (NOT sublevel) event exists in.
	 * @param inDate Date for requested scoring.
	 * @param outEvent Pointer to object, NULL if not found.
	 * @return Whether the object was found.
	 */
	bool FindEvent(
			ARBString const& inDivision,
			ARBString const& inLevel,
			ARBDate const& inDate,
			ARBConfigScoringPtr* outEvent = NULL) const;

	/**
	 * Verify a scoring method exists.
	 * @param inDivision Division event is in.
	 * @param inLevel Level event is in.
	 * @param inDate Date for requested scoring.
	 * @return true if FindAllEvents() > 0.
	 */
	bool VerifyEvent(
			ARBString const& inDivision,
			ARBString const& inLevel,
			ARBDate const& inDate) const;

	/**
	 * Create a new scoring method.
	 * @return Pointer to a new method.
	 */
	ARBConfigScoringPtr AddScoring();
};
