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
 *
 * @brief Configuration class.
 * @author David Connet
 *
 * Revision History
 * @li 2004-01-05 DRC Added LongName.
 * @li 2003-12-28 DRC Added GetSearchStrings.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 */

#include <string>
#include "ARBBase.h"
#include "ARBVector.h"
class ARBVersion;
class CElement;

class ARBConfigTitle : public ARBBase
{
public:
	ARBConfigTitle();
	ARBConfigTitle(const ARBConfigTitle& rhs);
	ARBConfigTitle& operator=(const ARBConfigTitle& rhs);
	bool operator==(const ARBConfigTitle& rhs) const;
	bool operator!=(const ARBConfigTitle& rhs) const;
	void clear();

	virtual std::string GetGenericName() const	{return GetNiceName();}
	virtual size_t GetSearchStrings(std::set<std::string>& ioStrings) const;

	bool Load(
		const CElement& inTree,
		const ARBVersion& inVersion,
		std::string& ioErrMsg);
	bool Save(CElement& inTree) const;

	const std::string& GetNiceName() const;
	std::string GetCompleteName(bool bAbbrevFirst = true) const;

	const std::string& GetName() const;
	void SetName(const std::string& inName);
	const std::string& GetLongName() const;
	void SetLongName(const std::string& inName);
	const std::string& GetDescription() const;
	void SetDescription(const std::string& inDesc);

private:
	~ARBConfigTitle();
	std::string m_Name;
	std::string m_LongName;
	std::string m_Desc;
};

inline const std::string& ARBConfigTitle::GetNiceName() const
{
	if (0 == m_LongName.length())
		return m_Name;
	else
		return m_LongName;
}

inline const std::string& ARBConfigTitle::GetName() const
{
	return m_Name;
}

inline void ARBConfigTitle::SetName(const std::string& inName)
{
	m_Name = inName;
}

inline const std::string& ARBConfigTitle::GetLongName() const
{
	return m_LongName;
}

inline void ARBConfigTitle::SetLongName(const std::string& inName)
{
	m_LongName = inName;
}

inline const std::string& ARBConfigTitle::GetDescription() const
{
	return m_Desc;
}

inline void ARBConfigTitle::SetDescription(const std::string& inDesc)
{
	m_Desc = inDesc;
}

/////////////////////////////////////////////////////////////////////////////

class ARBConfigTitleList : public ARBVectorLoad1<ARBConfigTitle>
{
public:
	bool operator==(const ARBConfigTitleList& rhs) const
	{
		return isEqual(rhs);
	}
	bool operator!=(const ARBConfigTitleList& rhs) const
	{
		return !isEqual(rhs);
	}

	const ARBConfigTitle* FindTitle(const std::string& inName) const;
	ARBConfigTitle* FindTitle(const std::string& inName);
	ARBConfigTitle* AddTitle(const std::string& inName);
	ARBConfigTitle* AddTitle(ARBConfigTitle* inTitle);
	bool DeleteTitle(const std::string& inName);
};
