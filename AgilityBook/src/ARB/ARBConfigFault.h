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
 * @brief ARBConfigFault and ARBConfigFaultList class.
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
class CElement;

/**
 * Pre-loaded Fault types.
 */
class ARBConfigFault : public ARBBase
{
public:
	ARBConfigFault();
	ARBConfigFault(const ARBConfigFault& rhs);
	ARBConfigFault& operator=(const ARBConfigFault& rhs);
	bool operator==(const ARBConfigFault& rhs) const;
	bool operator!=(const ARBConfigFault& rhs) const;

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
	 * Load a fault.
	 * @pre inTree is the actual ARBConfigFault element.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioErrMsg Accumulated error messages.
	 * @return Success
	 */
	bool Load(
		const CElement& inTree,
		const ARBVersion& inVersion,
		std::string& ioErrMsg);

	/**
	 * Save a document.
	 * @param ioTree Parent element.
	 * @return Success
	 * @post The ARBConfigFault element will be created in ioTree.
	 */
	bool Save(CElement& ioTree) const;

	/*
	 * Getters/setters.
	 */
	const std::string& GetName() const;
	void SetName(const std::string& inName);

private:
	~ARBConfigFault();
	std::string m_Name;
};

inline std::string ARBConfigFault::GetGenericName() const
{
	return m_Name;
}

inline const std::string& ARBConfigFault::GetName() const
{
	return m_Name;
}

inline void ARBConfigFault::SetName(const std::string& inName)
{
	m_Name = inName;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * List of ARBConfigFault objects.
 */
class ARBConfigFaultList : public ARBVectorLoad1<ARBConfigFault>
{
public:
	bool operator==(const ARBConfigFaultList& rhs) const
	{
		return isEqual(rhs);
	}
	bool operator!=(const ARBConfigFaultList& rhs) const
	{
		return !isEqual(rhs);
	}

	/**
	 * Find a fault.
	 * @param inName Name of fault to find.
	 * @return Pointer to found object, NULL if not found.
	 * @post Returned pointer is not ref counted, do <b><i>not</i></b> release.
	 */
	const ARBConfigFault* FindFault(const std::string& inName) const;

	/**
	 * Add a fault.
	 * @param inName Fault to add.
	 * @return Pointer to object, NULL if name already exists or is empty.
	 * @post Returned pointer is not ref counted, do <b><i>not</i></b> release.
	 *       The pointer is added to the list and its ref count is incremented.
	 */
	ARBConfigFault* AddFault(const std::string& inName);

	/**
	 * Delete the fault.
	 * @param inName Name of fault to delete.
	 * @return Whether fault was deleted.
	 */
	bool DeleteFault(const std::string& inName);
};
