#pragma once

/*
 * Copyright (c) 2002-2003 David Connet.
 * All Rights Reserved.
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
 */

#include <string>
#include "ARBBase.h"
#include "ARBTypes.h"
#include "ARBVector.h"
class ARBConfig;
class ARBConfigVenue;
class CElement;

class ARBDogRegNum : public ARBBase
{
public:
	ARBDogRegNum();
	ARBDogRegNum(const ARBDogRegNum& rhs);
	ARBDogRegNum& operator=(const ARBDogRegNum& rhs);
	bool operator==(const ARBDogRegNum& rhs) const;
	bool operator!=(const ARBDogRegNum& rhs) const;

	virtual std::string GetGenericName() const;

	bool Load(
		const ARBConfig& inConfig,
		const CElement& inTree,
		int inVersion);
	bool Save(CElement& ioTree) const;

	const std::string& GetVenue() const;
	void SetVenue(const std::string& inVenue);
	const std::string& GetNumber() const;
	void SetNumber(const std::string& inNumber);
	const std::string& GetHeight() const;
	void SetHeight(const std::string& inHeight);

private:
	~ARBDogRegNum();
	std::string m_Venue;
	std::string m_Number;
	std::string m_Height;
};

inline const std::string& ARBDogRegNum::GetVenue() const
{
	return m_Venue;
}

inline void ARBDogRegNum::SetVenue(const std::string& inVenue)
{
	m_Venue = inVenue;
}

inline const std::string& ARBDogRegNum::GetNumber() const
{
	return m_Number;
}

inline void ARBDogRegNum::SetNumber(const std::string& inNumber)
{
	m_Number = inNumber;
}

inline const std::string& ARBDogRegNum::GetHeight() const
{
	return m_Height;
}

inline void ARBDogRegNum::SetHeight(const std::string& inHeight)
{
	m_Height = inHeight;
}

/////////////////////////////////////////////////////////////////////////////

class ARBDogRegNumList : public ARBVectorLoad2<ARBDogRegNum>
{
public:
	bool operator==(const ARBDogRegNumList& rhs) const
	{
		return isEqual(rhs);
	}
	bool operator!=(const ARBDogRegNumList& rhs) const
	{
		return !isEqual(rhs);
	}

	void sort(bool inDescending = true);

	int NumRegNumsInVenue(const std::string& inVenue) const;
	int RenameVenue(
		const std::string& inOldVenue,
		const std::string& inNewVenue);
	int DeleteVenue(const std::string& inVenue);

	int DeleteRegNum(
		const std::string& inVenue,
		const std::string& inNumber);

	ARBDogRegNum* AddRegNum(
		const std::string& inVenue,
		const std::string& inNumber);
};
