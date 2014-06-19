/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief Utility classes
 * @author David Connet
 *
 * Revision History
 * 2014-06-19 Added IsWin7OrBetter.
 * 2013-07-17 Created
 */

#include "stdafx.h"
#include "ARBCommon/ARBMisc.h"

#include "ARBCommon/StringUtil.h"
#include <iostream>
#include <sstream>

// For testing in ARB
//#if defined(__WXWINDOWS__)
//#undef __WXWINDOWS__
//#endif

#if defined(__WXWINDOWS__)
#include <wx/string.h>
#elif defined(_WIN32)
#pragma comment(lib, "version.lib")
#endif

#if defined(__WXMSW__)
#include <wx/msw/msvcrt.h>
#endif

/////////////////////////////////////////////////////////////////////////////

std::wstring SanitizeStringForHTML(
		std::wstring const& inRawData,
		bool bConvertCR)
{
	std::wstring::size_type pos = inRawData.find_first_of(L"&<>");
	if (std::wstring::npos == pos && bConvertCR)
		pos = inRawData.find_first_of(L"\r\n");
	if (std::wstring::npos == pos)
		return inRawData;
	std::wostringstream data;
	for (size_t nChar = 0; nChar < inRawData.length(); ++nChar)
	{
		switch (inRawData[nChar])
		{
		case L'&':
			data << L"&amp;";
			break;
		case L'<':
			data << L"&lt;";
			break;
		case L'>':
			data << L"&gt;";
			break;
		case L'\r':
			if (bConvertCR)
			{
				if (nChar + 1 < inRawData.length() && '\n' == inRawData[nChar+1])
					continue;
				else
					data << L"<br/>";
			}
			else
				data << inRawData[nChar];
			break;
		case '\n':
			if (bConvertCR)
				data << L"<br/>";
			else
				data << inRawData[nChar];
			break;
		default:
			data << inRawData[nChar];
			break;
		}
	}
	return data.str();
}

/////////////////////////////////////////////////////////////////////////////

bool GetOSInfo(int& verMajor, int& verMinor)
{
	verMajor = verMinor = 0;

#if defined(__WXWINDOWS__)
	wxPlatformInfo info;
	verMajor = info.GetOSMajorVersion();
	verMinor = info.GetOSMinorVersion();
#ifdef __WXMAC__
#ifndef __WXOSX_COCOA__
#if !wxCHECK_VERSION(3, 0, 0)
	verMajor = (verMajor == 16 ? 10 : verMajor);
	verMinor = (verMinor >> 4);
#endif
#endif
#endif
	return true;

#elif defined(_WIN32)
	std::wstring kernel(L"kernel32.dll");
	DWORD dwHandle;
	DWORD dwLen = GetFileVersionInfoSize(kernel.c_str(), &dwHandle);
	if (!dwLen) 
		return false;

	void* lpData = malloc(dwLen);
	if (!lpData) 
		return false;

	if (!GetFileVersionInfo(kernel.c_str(), dwHandle, dwLen, lpData))
	{
		free(lpData);
		return false;
	}

	UINT BufLen;
	VS_FIXEDFILEINFO* pFileInfo = nullptr;
	if (VerQueryValue(lpData, L"\\", (void**)&pFileInfo, &BufLen))
	{
		verMajor= HIWORD(pFileInfo->dwFileVersionMS);
		verMinor = LOWORD(pFileInfo->dwFileVersionMS);
		//*BuildNumber = HIWORD(pFileInfo->dwFileVersionLS);
		//*RevisionNumber = LOWORD(pFileInfo->dwFileVersionLS);
		free(lpData);
		return true;
	}
	free(lpData);
	return false;

#else
#error Unknown OS
#endif
}

/////////////////////////////////////////////////////////////////////////////

static std::wstring GetOSName()
{
	std::wostringstream str;

	int majVer;
	int minVer;
	if (!GetOSInfo(majVer, minVer))
		return std::wstring();

#if defined(__WXWINDOWS__)
	wxPlatformInfo info;
	str << StringUtil::stringW(info.GetOperatingSystemIdName())
		<< L" "
		<< majVer
		<< L"."
		<< minVer;

#elif defined(_WIN32)
	switch (majVer)
	{
	default:
		str << L"Windows " << majVer << L"." << minVer;
		break;
	case 6:
		switch (minVer)
		{
		default:
			str << L"Windows " << majVer << L"." << minVer;
			break;
		case 0:
			str << L"Windows Vista";
			break;
		case 1:
			str << L"Windows 7";
			break;
		case 2:
			str << L"Windows 8";
			break;
		case 3:
			str << L"Windows 8.1";
			break;
		}
		break;
	case 5:
		switch (minVer)
		{
		default:
			str << L"Windows " << majVer << L"." << minVer;
			break;
		case 2:
			str << L"Windows XP"; // Not really, 64bitXP, or Server
			break;
		case 1:
			str << L"Windows XP";
			break;
		case 0:
			str << L"Windows 2000";
			break;
		}
		break;
	}

#else
#error Unknown OS
#endif

	return str.str();
}


static std::wstring GetArchName()
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


static std::wstring GetEndiannessName()
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


std::wstring GetOSInfo(bool bVerbose)
{
	std::wostringstream str;

	if (bVerbose)
		str << L"OS: ";

	str << GetOSName();

	if (bVerbose)
	{
		str << L"\n";

		str << L"Architecture: "
			<< GetArchName()
			<< L", "
			<< GetEndiannessName()
			<< L"\n";
	}

	return str.str();
}

/////////////////////////////////////////////////////////////////////////////

#ifdef WIN32

bool CheckOS(DWORD dwMajor, DWORD dwMinor, int op)
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;

	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	osvi.dwMajorVersion = dwMajor;
	osvi.dwMinorVersion = dwMinor;
	osvi.wServicePackMajor = 0;
	osvi.wServicePackMinor = 0;

#pragma warning (push)
// warning C4244: 'argument' : conversion from 'int' to 'BYTE', possible loss of data
#pragma warning (disable : 4244)
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, op);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMAJOR, op);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMINOR, op);
#pragma warning (pop)

	return !!VerifyVersionInfo(&osvi, 
			VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR,
			dwlConditionMask);
}


bool IsWin7OrBetter()
{
	return CheckOS(6, 1, VER_GREATER_EQUAL);
}

#endif
