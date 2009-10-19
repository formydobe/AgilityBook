#pragma once

/*
 * Copyright (c) 2004-2009 David Connet. All Rights Reserved.
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
 * @brief ARBConfigLifetimePoints and ARBConfigLifetimePointsList class.
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-06-25 DRC Cleaned up reference counting when returning a pointer.
 * @li 2004-10-06 DRC Added as part of file version 10.
 */

#include "ARBBase.h"
#include "ARBTypes.h"

/**
 * Number of lifetime points that can be earned based on number of faults.
 * Some venues have a sliding scale, for instance a clean run in NADAC
 * is worth 10 pts, a run with up to 5 faults earns 5 pts.
 *
 * This class works very much like ARBConfigTitlePoints. (In fact, lifetime
 * points used to be titling points until it was discovered that some venues
 * award different values [CPE].)
 */
class ARBConfigLifetimePoints : public ARBBase
{
protected:
	ARBConfigLifetimePoints();
	ARBConfigLifetimePoints(double inPoints, double inFaults);
	ARBConfigLifetimePoints(ARBConfigLifetimePoints const& rhs);

public:
	~ARBConfigLifetimePoints();
	static ARBConfigLifetimePointsPtr New();
	static ARBConfigLifetimePointsPtr New(double inPoints, double inFaults);
	ARBConfigLifetimePointsPtr Clone() const;

	ARBConfigLifetimePoints& operator=(ARBConfigLifetimePoints const& rhs);

	bool operator==(ARBConfigLifetimePoints const& rhs) const;
	bool operator!=(ARBConfigLifetimePoints const& rhs) const
	{
		return !operator==(rhs);
	}

	/**
	 * Get the generic name of this object.
	 * @return The generic name of this object.
	 */
	virtual wxString GetGenericName() const;

	/**
	 * Get all the strings to search in this object.
	 * @param ioStrings Accumulated list of strings to be used during a search.
	 * @return Number of strings accumulated in this object.
	 */
	virtual size_t GetSearchStrings(std::set<wxString>& ioStrings) const
	{
		return 0;
	}

	/**
	 * Load a lifetime configuration.
	 * @pre inTree is the actual ARBConfigLifetimePoints element.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioCallback Error processing callback.
	 * @return Success
	 */
	bool Load(
			ElementNodePtr inTree,
			ARBVersion const& inVersion,
			ARBErrorCallback& ioCallback);

	/**
	 * Save a document.
	 * @param ioTree Parent element.
	 * @return Success
	 * @post The ARBConfigLifetimePoints element will be created in ioTree.
	 */
	bool Save(ElementNodePtr ioTree) const;

	/*
	 * Setters/getters.
	 */
	double GetPoints() const
	{
		return m_Points;
	}
	void SetPoints(double inPoints)
	{
		m_Points = inPoints;
	}
	double GetFaults() const
	{
		return m_Faults;
	}
	// There is no SetFaults since this needs to be a unique key in the list.

private:
	double m_Points;
	double m_Faults;
};

/////////////////////////////////////////////////////////////////////////////

/**
 * List of ARBConfigLifetimePoints objects.
 */
class ARBConfigLifetimePointsList : public ARBVector<ARBConfigLifetimePointsPtr>
{
public:
	/**
	 * Load the information from XML (the tree).
	 * @pre inTree is the actual T element.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioCallback Error processing callback.
	 * @return Success
	 */
	bool Load(
			ElementNodePtr inTree,
			ARBVersion const& inVersion,
			ARBErrorCallback& ioCallback);

	/**
	 * Sort the lifetime point objects by faults.
	 */
	void sort();

	/**
	 * Get the number of lifetime title points earned based on faults.
	 * @param inFaults Number of faults in the run.
	 * @return Number of lifetime titling points.
	 */
	double GetLifetimePoints(double inFaults) const;

	/**
	 * Find a points object.
	 * @param inFaults Number of faults to find.
	 * @param outPoints Pointer to found object, NULL if not found.
	 * @return Whether the object was found.
	 */
	bool FindLifetimePoints(
			double inFaults,
			ARBConfigLifetimePointsPtr* outPoints = NULL) const;

	/**
	 * Add an object.
	 * @param inPoints Number of lifetime points.
	 * @param inFaults Number of faults.
	 * @param outPoints Pointer to new object, NULL if it already exists.
	 * @return Whether the object was added.
	 */
	bool AddLifetimePoints(
			double inPoints,
			double inFaults,
			ARBConfigLifetimePointsPtr* outPoints = NULL);

	/**
	 * Delete an object.
	 * @param inFaults Delete object with the given faults.
	 * @return Whether object was deleted.
	 */
	bool DeleteLifetimePoints(double inFaults);
};