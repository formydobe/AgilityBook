#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See Licence.txt
 */

/**
 * @file
 * @brief ARBConfigTitlePoints and ARBConfigTitlePointsList class.
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-06-25 DRC Cleaned up reference counting when returning a pointer.
 * @li 2004-09-28 DRC Changed how error reporting is done when loading.
 * @li 2004-03-31 DRC Started adding auto-lifetime point accumulation.
 * @li 2003-12-28 DRC Added GetSearchStrings.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 * @li 2003-07-12 DRC Added as part of file version 5.
 */

#include "ARBBase.h"
#include "ARBTypes.h"

/**
 * Number of title points that can be earned based on number of faults.
 * Some venues have a sliding scale, for instance a clean run in NADAC
 * is worth 10 pts, a run with up to 5 faults earns 5 pts.
 */
class ARBConfigTitlePoints : public ARBBase
{
protected:
	ARBConfigTitlePoints();
	ARBConfigTitlePoints(double inPoints, double inFaults);
	ARBConfigTitlePoints(ARBConfigTitlePoints const& rhs);

public:
	~ARBConfigTitlePoints();
	static ARBConfigTitlePointsPtr New();
	static ARBConfigTitlePointsPtr New(double inPoints, double inFaults);
	ARBConfigTitlePointsPtr Clone() const;

	ARBConfigTitlePoints& operator=(ARBConfigTitlePoints const& rhs);

	bool operator==(ARBConfigTitlePoints const& rhs) const;
	bool operator!=(ARBConfigTitlePoints const& rhs) const
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
	 * Load a title configuration.
	 * @pre inTree is the actual ARBConfigTitlePoints element.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioCallback Error processing callback.
	 * @param ioLifetimePoints Used for migrating older files.
	 * @return Success
	 */
	bool Load(
			ElementNodePtr inTree,
			ARBVersion const& inVersion,
			ARBErrorCallback& ioCallback,
			ARBConfigLifetimePointsList& ioLifetimePoints);

	/**
	 * Save a document.
	 * @param ioTree Parent element.
	 * @return Success
	 * @post The ARBConfigTitlePoints element will be created in ioTree.
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
 * List of ARBConfigTitlePoints objects.
 */
class ARBConfigTitlePointsList : public ARBVector<ARBConfigTitlePointsPtr>
{
public:
	/**
	 * Load a title configuration.
	 * @pre inTree is the actual ARBConfigTitlePoints element.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioCallback Error processing callback.
	 * @param ioLifetimePoints Used for migrating older files.
	 * @return Success
	 */
	bool Load(
			ElementNodePtr inTree,
			ARBVersion const& inVersion,
			ARBErrorCallback& ioCallback,
			ARBConfigLifetimePointsList& ioLifetimePoints);

	/**
	 * Sort the title point objects by faults.
	 */
	void sort();

	/**
	 * Get the number of title points earned based on faults.
	 * @param inFaults Number of faults in the run.
	 * @return Number of titling points.
	 */
	double GetTitlePoints(double inFaults) const;

	/**
	 * Find a points object.
	 * @param inFaults Number of faults to find.
	 * @param outPoints Pointer to found object, NULL if not found.
	 * @return Whether the object was found.
	 */
	bool FindTitlePoints(
			double inFaults,
			ARBConfigTitlePointsPtr* outPoints = NULL) const;

	/**
	 * Add an object.
	 * @param inPoints Number of title points.
	 * @param inFaults Number of faults.
	 * @param outPoints Pointer to new object, NULL if it already exists.
	 * @return Whether the object was added.
	 */
	bool AddTitlePoints(
			double inPoints,
			double inFaults,
			ARBConfigTitlePointsPtr* outPoints = NULL);

	/**
	 * Delete an object.
	 * @param inFaults Delete object with the given faults.
	 * @return Whether object was deleted.
	 */
	bool DeleteTitlePoints(double inFaults);
};