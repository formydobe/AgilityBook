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
 * @brief ARBConfigSubLevel and ARBConfigSubLevelList class.
 * @author David Connet
 *
 * Revision History
 * @li 2003-12-28 DRC Added GetSearchStrings.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 */

#include <string>
#include "ARBBase.h"
#include "ARBVector.h"
class ARBVersion;
class Element;

/**
 * Sublevel, allows a level to be split into A/B groupings.
 */
class ARBConfigSubLevel : public ARBBase
{
public:
	ARBConfigSubLevel();
	ARBConfigSubLevel(const ARBConfigSubLevel& rhs);
	ARBConfigSubLevel& operator=(const ARBConfigSubLevel& rhs);
	bool operator==(const ARBConfigSubLevel& rhs) const;
	bool operator!=(const ARBConfigSubLevel& rhs) const;

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
	 * Load a sublevel configuration.
	 * @pre inTree is the actual ARBConfigSubLevel element.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioErrMsg Accumulated error messages.
	 * @return Success
	 */
	bool Load(
		const Element& inTree,
		const ARBVersion& inVersion,
		std::string& ioErrMsg);

	/**
	 * Save a document.
	 * @param ioTree Parent element.
	 * @return Success
	 * @post The ARBConfigSubLevel element will be created in ioTree.
	 */
	bool Save(Element& ioTree) const;

	/*
	 * Getters/setters.
	 */
	const std::string& GetName() const;
	void SetName(const std::string& inName);

private:
	~ARBConfigSubLevel();
	std::string m_Name;
};

inline std::string ARBConfigSubLevel::GetGenericName() const
{
	return m_Name;
}

inline const std::string& ARBConfigSubLevel::GetName() const
{
	return m_Name;
}

inline void ARBConfigSubLevel::SetName(const std::string& inName)
{
	m_Name = inName;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * List of ARBConfigSubLevel objects.
 */
class ARBConfigSubLevelList : public ARBVectorLoad1<ARBConfigSubLevel>
{
public:
	bool operator==(const ARBConfigSubLevelList& rhs) const
	{
		return isEqual(rhs);
	}
	bool operator!=(const ARBConfigSubLevelList& rhs) const
	{
		return !isEqual(rhs);
	}

	/**
	 * Find a sublevel.
	 * @param inName Name of sublevel to find.
	 * @return Whether name exists.
	 */
	bool FindSubLevel(const std::string& inName) const;

	/**
	 * Find a sublevel.
	 * @param inName Name of sublevel to find.
	 * @return Pointer to object, NULL if not found.
	 * @post Returned pointer is not ref counted, do <b><i>not</i></b> release.
	 */
	ARBConfigSubLevel* AddSubLevel(const std::string& inName);

	/**
	 * Delete a sublevel.
	 * @param inName Name of sublevel to delete.
	 * @return Whether sublevel was deleted or not.
	 */
	bool DeleteSubLevel(const std::string& inName);
};
