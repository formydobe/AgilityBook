#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief Type definitions.
 * @author David Connet
 *
 * Revision History
 * @li 2012-09-09 DRC Added ARBVectorNoSave.
 * @li 2012-08-13 DRC Moved ARB_Q to separate file.
 * @li 2012-05-04 DRC Add bAlwaysStripZeros to ARBDouble::ToString.
 * @li 2011-08-22 DRC ARBVersion was only using 16 instead of 32bits.
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2009-04-20 DRC Added DNR.
 * @li 2008-06-29 DRC Moved string stuff to ARBString.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-06-25 DRC Removed ARBDouble.
 * @li 2004-09-28 DRC Changed how error reporting is done when loading.
 * @li 2004-04-06 DRC Added op< to ARB_Q.
 * @li 2003-11-26 DRC Changed version number to a complex value.
 * @li 2003-09-01 DRC Added 'operator+=' and 'operator-=' to ARBDouble.
 */

#include <set>
#include <sstream>
#include <string>
#include <vector>

// Forward references
class ARBConfig;

#define ARB_TYPEDEF(name) \
	class name;\
	typedef std::tr1::shared_ptr<name> name##Ptr;
#define ARB_TYPEDEF2(name) \
	class name;\
	typedef std::tr1::weak_ptr<name> name##WPtr;\
	typedef std::tr1::shared_ptr<name> name##Ptr;
#define ARB_TYPEDEF_LIST(name) \
	class name;\
	class name##List;\
	typedef std::tr1::shared_ptr<name> name##Ptr;

ARB_TYPEDEF(ARBConfigActionDeleteDivision)
ARB_TYPEDEF(ARBConfigActionDeleteEvent)
ARB_TYPEDEF(ARBConfigActionDeleteLevel)
ARB_TYPEDEF(ARBConfigActionDeleteMultiQ)
ARB_TYPEDEF(ARBConfigActionDeleteOtherPoints)
ARB_TYPEDEF(ARBConfigActionDeleteTitle)
ARB_TYPEDEF(ARBConfigActionDeleteVenue)
ARB_TYPEDEF(ARBConfigActionRenameDivision)
ARB_TYPEDEF(ARBConfigActionRenameEvent)
ARB_TYPEDEF(ARBConfigActionRenameLevel)
ARB_TYPEDEF(ARBConfigActionRenameMultiQ)
ARB_TYPEDEF(ARBConfigActionRenameOtherPoints)
ARB_TYPEDEF(ARBConfigActionRenameTitle)
ARB_TYPEDEF(ARBConfigActionRenameVenue)
ARB_TYPEDEF(Element)
ARB_TYPEDEF_LIST(ARBCalendar)
ARB_TYPEDEF_LIST(ARBConfigAction)
ARB_TYPEDEF_LIST(ARBConfigCalSite)
ARB_TYPEDEF_LIST(ARBConfigDivision)
ARB_TYPEDEF_LIST(ARBConfigEvent)
ARB_TYPEDEF_LIST(ARBConfigFault)
ARB_TYPEDEF_LIST(ARBConfigLevel)
ARB_TYPEDEF_LIST(ARBConfigLifetimePoints)
ARB_TYPEDEF_LIST(ARBConfigMultiQ)
ARB_TYPEDEF_LIST(ARBConfigOtherPoints)
ARB_TYPEDEF_LIST(ARBConfigPlaceInfo)
ARB_TYPEDEF_LIST(ARBConfigScoring)
ARB_TYPEDEF_LIST(ARBConfigSubLevel)
ARB_TYPEDEF_LIST(ARBConfigTitle)
ARB_TYPEDEF_LIST(ARBConfigTitlePoints)
ARB_TYPEDEF_LIST(ARBConfigVenue)
ARB_TYPEDEF_LIST(ARBDog)
ARB_TYPEDEF_LIST(ARBDogClub)
ARB_TYPEDEF_LIST(ARBDogExistingPoints)
ARB_TYPEDEF_LIST(ARBDogReferenceRun)
ARB_TYPEDEF_LIST(ARBDogRegNum)
ARB_TYPEDEF_LIST(ARBDogRun)
ARB_TYPEDEF_LIST(ARBDogRunOtherPoints)
ARB_TYPEDEF_LIST(ARBDogRunPartner)
ARB_TYPEDEF_LIST(ARBDogRunScoring)
ARB_TYPEDEF_LIST(ARBDogTitle)
ARB_TYPEDEF_LIST(ARBDogTrial)
ARB_TYPEDEF_LIST(ARBInfoItem)
ARB_TYPEDEF_LIST(ARBTraining)
ARB_TYPEDEF2(ElementNode)
ARB_TYPEDEF2(ElementText)


/**
 * Make a string safe for HTML viewing.
 * @param inRawData String to sanitize
 * @param bConvertCR Convert \n to html-breaks.
 */
std::wstring SanitizeStringForHTML(
		std::wstring const& inRawData,
		bool bConvertCR = true);

/////////////////////////////////////////////////////////////////////////////
/**
 * Extend some common functionality.
 */
template <typename T>
class ARBVectorNoSave : public std::vector<T>
{
public:
	ARBVectorNoSave()
	{
	}

	/**
	 * Equality test (by value, not pointer).
	 */
	bool operator==(ARBVectorNoSave<T> const& rhs) const
	{
		if (this == &rhs)
			return true;
		if (ARBVectorNoSave<T>::size() != rhs.size())
			return false;
		typename ARBVectorNoSave<T>::const_iterator iter1, iter2;
		for (iter1 = ARBVectorNoSave<T>::begin(), iter2 = rhs.begin();
			iter1 != ARBVectorNoSave<T>::end();
			++iter1, ++iter2)
		{
			if (*(*iter1) != *(*iter2))
				return false;
		}
		return true;
	}
	bool operator!=(ARBVectorNoSave<T> const& rhs) const
	{
		return !operator==(rhs);
	}

	/**
	 * Make a copy of everything.
	 * @param outList Object being copied to.
	 */
	size_t Clone(ARBVectorNoSave<T>& outList) const
	{
		outList.clear();
		outList.reserve(ARBVectorNoSave<T>::size());
		for (typename ARBVectorNoSave<T>::const_iterator iter = ARBVectorNoSave<T>::begin();
			iter != ARBVectorNoSave<T>::end();
			++iter)
		{
			T pItem = *iter;
			if (pItem)
				outList.push_back(pItem->Clone());
		}
		return outList.size();
	}

	/**
	 * Get all the strings to search in this list.
	 * @param ioStrings Accumulated list of strings to be used during a search.
	 * @return Number of strings accumulated in this object.
	 */
	size_t GetSearchStrings(std::set<std::wstring>& ioStrings) const
	{
		size_t nItems = 0;
		for (typename ARBVectorNoSave<T>::const_iterator iter = ARBVectorNoSave<T>::begin();
			iter != ARBVectorNoSave<T>::end();
			++iter)
		{
			nItems += (*iter)->GetSearchStrings(ioStrings);
		}
		return nItems;
	}

	/**
	 * Reorder a list.
	 * @param inItem Object to be moved.
	 * @param inMove Number of positions to move object.
	 * @return Whether or not object was moved.
	 */
	bool Move(
			T inItem,
			int inMove)
	{
		bool bOk = false;
		if (inItem)
		{
			int n = 0;
			for (typename ARBVectorNoSave<T>::iterator iter = ARBVectorNoSave<T>::begin();
				iter != ARBVectorNoSave<T>::end();
				++iter, ++n)
			{
				if (inItem == *iter)
				{
					int offset = n + inMove;
					if (offset < 0)
						offset = 0;
					if (offset >= static_cast<int>(ARBVectorNoSave<T>::size()))
						offset = static_cast<int>(ARBVectorNoSave<T>::size()) - 1;
					if (offset != n)
					{
						bOk = true;
						std::swap(ARBVectorNoSave<T>::at(n), ARBVectorNoSave<T>::at(offset));
						break;
					}
				}
			}
		}
		return bOk;
	}
};


template <typename T>
class ARBVector : public ARBVectorNoSave<T>
{
public:
	ARBVector()
	{
	}

	/**
	 * Save a document.
	 * In general, the vectors themselves are never stored, only the elements.
	 * (It flattens the tree a little.)
	 * @param ioTree Parent element.
	 * @return Success
	 * @post The T element will be created in ioTree.
	 */
	bool Save(ElementNodePtr ioTree) const
	{
		for (typename ARBVector<T>::const_iterator iter = ARBVector<T>::begin();
			iter != ARBVector<T>::end();
			++iter)
		{
			if (!(*iter)->Save(ioTree))
				return false;
		}
		return true;
	}
};


template <typename T>
class ARBVectorSaveConfig : public ARBVectorNoSave<T>
{
public:
	ARBVectorSaveConfig()
	{
	}

	/**
	 * Save a document.
	 * In general, the vectors themselves are never stored, only the elements.
	 * (It flattens the tree a little.)
	 * @param ioTree Parent element.
	 * @param inConfig Configuration.
	 * @return Success
	 * @post The T element will be created in ioTree.
	 */
	bool Save(ElementNodePtr ioTree, ARBConfig const& inConfig) const
	{
		for (typename ARBVectorSaveConfig<T>::const_iterator iter = ARBVectorSaveConfig<T>::begin();
			iter != ARBVectorSaveConfig<T>::end();
			++iter)
		{
			if (!(*iter)->Save(ioTree, inConfig))
				return false;
		}
		return true;
	}
};

/////////////////////////////////////////////////////////////////////////////
/**
 * Error callback class.
 */
class ARBErrorCallback
{
public:
	ARBErrorCallback(std::wostringstream& ioErrMsg)
		: m_ErrMsg(ioErrMsg)
	{
	}
	virtual ~ARBErrorCallback() {}

	/**
	 * Error message that requires a user response.
	 * @param pMsg Message to display to user.
	 * @return True to continue, false to abort.
	 */
	virtual bool OnError(wchar_t const* const pMsg)
	{
		return false;
	}
	virtual bool OnError(std::wstring const& msg)
	{
		return OnError(msg.c_str());
	}

	/**
	 * Log an error message.
	 */
	virtual void LogMessage(std::wstring const& inMsg)
	{
		m_ErrMsg << inMsg;
	}

protected:
	std::wostringstream& m_ErrMsg;
};

/////////////////////////////////////////////////////////////////////////////
/**
 * A version has a major and minor number.
 * Differences between major numbers prevents older versions of the program
 * from reading newer files. Differences in minor numbers allows an older
 * version to read the newer file, but warns that some information may be
 * lost if the file is saved.
 *
 * This class is assuming that a short==16 bits and long==32 bits.
 */
class ARBVersion
{
public:
	ARBVersion()
		: m_Version(0)
	{
	}

	ARBVersion(
			unsigned short major,
			unsigned short minor)
		: m_Version((major<<16) | minor)
	{
	}

	ARBVersion(ARBVersion const& inVer)
		: m_Version(inVer.m_Version)
	{
	}

	ARBVersion& operator=(ARBVersion const& rhs)
	{
		if (this != &rhs)
		{
			m_Version = rhs.m_Version;
		}
		return *this;
	}
	bool operator==(ARBVersion const& rhs) const
	{
		return m_Version == rhs.m_Version;
	}
	bool operator<(ARBVersion const& rhs) const
	{
		return m_Version < rhs.m_Version;
	}
	bool operator<=(ARBVersion const& rhs) const
	{
		return m_Version <= rhs.m_Version;
	}
	bool operator>(ARBVersion const& rhs) const
	{
		return m_Version > rhs.m_Version;
	}
	bool operator>=(ARBVersion const& rhs) const
	{
		return m_Version >= rhs.m_Version;
	}

	unsigned short Major() const
	{
		return static_cast<unsigned short>(m_Version >> 16);
	}
	unsigned short Minor() const
	{
		return static_cast<unsigned short>(m_Version & 0xffff);
	}
	std::wstring str() const;

private:
	unsigned long m_Version;
};

/////////////////////////////////////////////////////////////////////////////
/**
 * Helper functions for quickly/easily converting doubles to strings.
 */
class ARBDouble
{
	ARBDouble();
	~ARBDouble();
public:
	enum LocaleType
	{
		eNone,		///< Use '.' in numbers always.
		eCurrent,	///< Use current locale.
		eDefault	///< Use default (system) locale.
	};
	/**
	 * Trailing zeros are trimmed unless inPrec=2.
	 * Then they are only trimmed if all zero (and inPrec=2).
	 */
	static std::wstring ToString(
			double inValue,
			int inPrec = 2,
			LocaleType eUseDefaultLocale = eDefault,
			bool bAlwaysStripZeros = false);
	/**
	 * Compare two doubles, allowing for 'prec' error.
	 */
	static bool equal(
			double const& inVal1,
			double const& inVal2,
			double inPrec = 1e-9);
};
