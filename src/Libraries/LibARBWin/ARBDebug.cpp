/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief Helper functions to retrieve debug data for dumps and ARBHelp
 * @author David Connet
 *
 * Revision History
 * 2019-01-31 Moved from ARB.
 * 2018-12-16 Convert to fmt.
 * 2018-01-28 Created
 */

#include "stdafx.h"
#include "LibARBWin/ARBDebug.h"

#include "ARBCommon/ARBMisc.h"
#include "ARBCommon/ARBUtils.h"
#include "ARBCommon/StringUtil.h"
#include "ARBCommon/VersionNum.h"
#include "LibARBWin/DPI.h"
#include <wx/config.h>
#include <wx/platinfo.h>
#include <wx/stdpaths.h>
#include <wx/string.h>

#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>
#endif

/////////////////////////////////////////////////////////////////////////////

std::wstring ARBDebug::GetOSName()
{
	std::wstring str;

	int majVer;
	int minVer;
	if (!GetOSInfo(majVer, minVer))
		return std::wstring();

#if defined(__WXWINDOWS__)
	wxPlatformInfo info;
	str = fmt::format(L"{} {}.{}", info.GetOperatingSystemIdName().wx_str(), majVer, minVer);

#elif defined(_WIN32)
	switch (majVer)
	{
	default:
		str = fmt::format(L"Windows {}.{}", majVer, minVer);
		break;
	case 6:
		switch (minVer)
		{
		default:
			str = fmt::format(L"Windows {}.{}", majVer, minVer);
			break;
		case 0:
			str = L"Windows Vista";
			break;
		case 1:
			str = L"Windows 7";
			break;
		case 2:
			str = L"Windows 8";
			break;
		case 3:
			str = L"Windows 8.1";
			break;
		}
		break;
	case 5:
		switch (minVer)
		{
		default:
			str = fmt::format(L"Windows {}.{}", majVer, minVer);
			break;
		case 2:
			str = L"Windows XP"; // Not really, 64bitXP, or Server
			break;
		case 1:
			str = L"Windows XP";
			break;
		case 0:
			str = L"Windows 2000";
			break;
		}
		break;
	}

#else
#error Unknown OS
#endif

	return str;
}


std::wstring ARBDebug::GetArchName()
{
#if defined(__WXWINDOWS__)
	wxPlatformInfo info;
	return StringUtil::stringW(info.GetArchName());

#elif defined(_WIN32)
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetNativeSystemInfo(&si);
	switch (si.wProcessorArchitecture)
	{
	default:
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
		return L"??";
	case PROCESSOR_ARCHITECTURE_AMD64:
		return L"x64";
	case PROCESSOR_ARCHITECTURE_ARM:
		return L"ARM";
	case PROCESSOR_ARCHITECTURE_IA64:
		return L"ia64";
	case PROCESSOR_ARCHITECTURE_INTEL:
		return L"x86";
	}

#else
#error Unknown OS
#endif
}


std::wstring ARBDebug::GetEndiannessName()
{
#if defined(__WXWINDOWS__)
	wxPlatformInfo info;
	return StringUtil::stringW(info.GetEndiannessName());

#else
	// Copied from wxWidgets 2.9.5: utilscmn.cpp: wxIsPlatformLittleEndian
	// Are we little or big endian? This method is from Harbison & Steele.
	union
	{
		long l;
		char c[sizeof(long)];
	} u;
	u.l = 1;

	bool isLittleEndian = (u.c[0] == 1);
	if (isLittleEndian)
		return L"Little endian";
	else
		return L"Big endian";

#endif
}

/////////////////////////////////////////////////////////////////////////////

std::wstring ARBDebug::GetSystemInfo(wxWindow const* pWindow, CVersionNum const& ver)
{
	fmt::wmemory_buffer str;

	// OS version
	fmt::format_to(str, L"OS: {}\n", GetOSName());
	fmt::format_to(str, L"Architecture: {}, {}\n", GetArchName(), GetEndiannessName());

	// DPI

	fmt::format_to(str, L"DPI Scaling: {}\n", DPI::GetScale(pWindow));

	// Me.
	{
		fmt::format_to(
			str,
			L"{}{}",
			wxStandardPaths::Get().GetExecutablePath().wx_str(),
#ifdef ARB_64BIT
			L" (64-bit): "
#else
			L" (32-bit): "
#endif
		);
		if (ver.Valid())
			fmt::format_to(str, L"{}\n", ver.GetVersionString());
		else
			fmt::format_to(str, L"{}\n", _("Unable to determine version information").wx_str());
	}

	// wxWidgets
	fmt::format_to(str, L"{}\n", wxVERSION_STRING);

	return fmt::to_string(str);
}


std::wstring ARBDebug::GetRegistryInfo()
{
	fmt::wmemory_buffer data;
	DumpRegistryGroup(wxEmptyString, &data, nullptr);
	return fmt::to_string(data);
}

/////////////////////////////////////////////////////////////////////////////
// Dump registry information

// .reg format (http://support.microsoft.com/kb/310516)
//
// RegistryEditorVersion
// Blank line
// [RegistryPath1]
// "DataItemName1"=DataType1:DataValue1
// DataItemName2=DataType2:DataValue2
// Blank line
// [RegistryPath2]
// "DataItemName3"="DataType3:DataValue3"
// "DataItemName4"="DataValue4"
// Blank line
//
// Difference between "REGEDIT4" and "Windows Registry Editor Version 5.00"
// - v5 supports unicode
//
// DataType (REG_SZ assumed unless DataType specified):
//  REG_BINARY		hex
//  REG_DWORD		dword
//  REG_EXPAND_SZ	hex(2)
//  REG_MULTI_SZ	hex(7)
//
// examples:
// "SetupType"=dword:00000000
// "CmdLine"="setup -newsetup"
// "SystemPrefix"=hex:c5,0b,00,00,00,40,36,02
// ; "String1\0String2" [in v5 (unicode) form, don't know how they look in v4]
// "Test"=hex(7):53,00,74,00,72,00,69,00,6e,00,67,00,20,00,31,00,00,00,53,00,74,\
//   00,72,00,69,00,6e,00,67,00,20,00,32,00,00,00,00,00
// ; %TMP%
// "Expand"=hex(2):25,00,54,00,4d,00,50,00,25,00,00,00
//
// To delete an item (hyphen in front of key):
// [-RegistryPath2]
// To delete a value (hyphen after '=' ):
// "DataItemName4"=-

size_t ARBDebug::DumpRegistryGroup(
	wxString const& inGroup,
	fmt::wmemory_buffer* outData,
	std::vector<std::wstring>* outItems)
{
	size_t added = 0; // Added to outData

	CConfigPathHelper config(inGroup);

	wxString str;
	long dummy;
	if (wxConfig::Get()->GetFirstEntry(str, dummy))
	{
		do
		{
			switch (wxConfig::Get()->GetEntryType(str))
			{
			default:
				if (outData)
				{
					++added;
					fmt::format_to(*outData, L"{}/{} unknown\n", wxConfig::Get()->GetPath().wx_str(), str.wx_str());
				}
				break;
			case wxConfigBase::Type_String:
				if (outData)
				{
					++added;
					fmt::format_to(*outData, L"{}/{} string\n", wxConfig::Get()->GetPath().wx_str(), str.wx_str());
					fmt::format_to(*outData, L"{}\n", wxConfig::Get()->Read(str, wxEmptyString).wx_str());
				}
				if (outItems)
					outItems->push_back(StringUtil::stringW(wxConfig::Get()->Read(str, wxEmptyString)));
				break;
			case wxConfigBase::Type_Boolean:
				if (outData)
				{
					++added;
					fmt::format_to(*outData, L"{}/{} bool\n", wxConfig::Get()->GetPath().wx_str(), str.wx_str());
					bool b;
					wxConfig::Get()->Read(str, &b);
					fmt::format_to(*outData, L"{}\n", b);
				}
				break;
			case wxConfigBase::Type_Integer:
				if (outData)
				{
					++added;
					fmt::format_to(*outData, L"{}/{} int\n", wxConfig::Get()->GetPath().wx_str(), str.wx_str());
					fmt::format_to(*outData, L"{}\n", wxConfig::Get()->Read(str, 0L));
				}
				break;
			case wxConfigBase::Type_Float:
				if (outData)
				{
					++added;
					fmt::format_to(*outData, L"{}/{} float\n", wxConfig::Get()->GetPath().wx_str(), str.wx_str());
					double d;
					wxConfig::Get()->Read(str, &d);
					fmt::format_to(*outData, L"{}\n", d);
				}
				break;
			}
		} while (wxConfig::Get()->GetNextEntry(str, dummy));
	}

	if (wxConfig::Get()->GetFirstGroup(str, dummy))
	{
		do
		{
			added += DumpRegistryGroup(str, outData, outItems);
		} while (wxConfig::Get()->GetNextGroup(str, dummy));
	}

	return added;
}
