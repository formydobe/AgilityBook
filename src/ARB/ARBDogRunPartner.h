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
 * @brief ARBDogRunPartner and ARBDogRunPartnerList classes.
 * @author David Connet
 *
 * Revision History
 * @li 2003-12-28 DRC Added GetSearchStrings.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 * @li 2003-09-08 DRC Changed AddPartner usage.
 */

#include "ARBBase.h"
#include "ARBVector.h"
class ARBConfig;
class ARBVersion;
class Element;

/**
 * Keeps track of partners in pairs runs.
 */
class ARBDogRunPartner : public ARBBase
{
public:
	ARBDogRunPartner();
	ARBDogRunPartner(ARBDogRunPartner const& rhs);
	ARBDogRunPartner& operator=(ARBDogRunPartner const& rhs);
	bool operator==(ARBDogRunPartner const& rhs) const;
	bool operator!=(ARBDogRunPartner const& rhs) const;

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
	 * Load a partner.
	 * @pre inTree is the actual ARBDogRunPartner element.
	 * @param inConfig Configuration for looking up information.
	 * @param inTree XML structure to convert into ARB.
	 * @param inVersion Version of the document being read.
	 * @param ioErrMsg Accumulated error messages.
	 * @return Success
	 */
	bool Load(
		ARBConfig const& inConfig,
		Element const& inTree,
		ARBVersion const& inVersion,
		std::string& ioErrMsg);

	/**
	 * Save a document.
	 * @param ioTree Parent element.
	 * @return Success
	 * @post The ARBDogRun element will be created in ioTree.
	 */
	bool Save(Element& ioTree) const;

	/*
	 * Getters/setters.
	 */
	std::string const& GetHandler() const;
	void SetHandler(std::string const& inHandler);
	std::string const& GetDog() const;
	void SetDog(std::string const& inDog);
	std::string const& GetRegNum() const;
	void SetRegNum(std::string const& inRegNum);

private:
	~ARBDogRunPartner();
	std::string m_Handler;
	std::string m_Dog;
	std::string m_RegNum;
};

inline std::string ARBDogRunPartner::GetGenericName() const
{
	return m_Dog;
}

inline std::string const& ARBDogRunPartner::GetHandler() const
{
	return m_Handler;
}

inline void ARBDogRunPartner::SetHandler(std::string const& inHandler)
{
	m_Handler = inHandler;
}

inline std::string const& ARBDogRunPartner::GetDog() const
{
	return m_Dog;
}

inline void ARBDogRunPartner::SetDog(std::string const& inDog)
{
	m_Dog = inDog;
}

inline std::string const& ARBDogRunPartner::GetRegNum() const
{
	return m_RegNum;
}

inline void ARBDogRunPartner::SetRegNum(std::string const& inRegNum)
{
	m_RegNum = inRegNum;
}

/////////////////////////////////////////////////////////////////////////////

/**
 * List of ARBDogRunPartner objects.
 */
class ARBDogRunPartnerList : public ARBVectorLoad2<ARBDogRunPartner>
{
public:
	/**
	 * Add a partner.
	 * @param inPartner Partner to add.
	 * @return Pointer to object.
	 * @post Returned pointer is not ref counted, do <b><i>not</i></b> release.
	 *       The pointer is added to the list and its ref count is incremented.
	 */
	ARBDogRunPartner* AddPartner(ARBDogRunPartner* inPartner);
};
