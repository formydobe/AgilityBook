/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief implementation of the CDlgPageDecode class
 * @author David Connet
 *
 * Revision History
 * 2012-07-10 Fix serialization. Broken in 4/15 wxString checkin.
 * 2009-09-13 Add support for wxWidgets 2.9, deprecate tstring.
 * 2009-08-26 Fixed streaming wxString to otstringstream.
 *            Fixed decoding binary files.
 * 2009-03-01 Ported to wxWidgets.
 * 2007-01-02 Created
 */

#include "stdafx.h"
#include "DlgPageDecode.h"

#include "ARBHelp.h"
#include "DlgARBHelp.h"

#include "ARBCommon/ARBTypes.h"
#include "ARBCommon/BinaryData.h"
#include "ARBCommon/StringUtil.h"
#include "ARBCommon/VersionNum.h"
#include "../Win/Widgets.h"
#include <algorithm>
#include <wx/ffile.h>
#include <wx/filename.h>

#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>
#endif


BEGIN_EVENT_TABLE(CDlgPageDecode, wxDialog)
	EVT_BUTTON(wxID_CANCEL, CDlgPageDecode::OnCancel)
END_EVENT_TABLE()


CDlgPageDecode::CDlgPageDecode()
	: wxDialog(nullptr, wxID_ANY, L"Decode", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX | wxRESIZE_BORDER)
{
	// Controls (these are done first to control tab order)

	wxString str(L"In order to decode the gathered information, first select all the text from '");
	str += STREAM_DATA_BEGIN;
	str += L"' to '";
	str += STREAM_DATA_END;
	str += L"' and copy here. Make sure to include those strings! Then press 'Decode'.";

	wxStaticText* staticText = new wxStaticText(this, wxID_ANY, str,
		wxDefaultPosition, wxDefaultSize, 0);
	staticText->Wrap(600);

	m_ctrlEncoded = new CTextCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	m_ctrlEncoded->SetMaxLength(0);
	wxFont font = m_ctrlEncoded->GetFont();
	m_ctrlEncoded->SetFont(wxFont(font.GetPointSize(), wxFONTFAMILY_MODERN, font.GetStyle(), font.GetWeight()));

	m_ctrlDecoded = new CTextCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(-1, 200),
		wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY);
	m_ctrlDecoded->SetMaxLength(0);
	font = m_ctrlDecoded->GetFont();
	m_ctrlDecoded->SetFont(wxFont(font.GetPointSize(), wxFONTFAMILY_MODERN, font.GetStyle(), font.GetWeight()));

	wxButton* btnDecode = new wxButton(this, wxID_ANY, L"Decode",
		wxDefaultPosition, wxDefaultSize, 0);
	BIND_OR_CONNECT_CTRL(btnDecode, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler, CDlgPageDecode::OnDecode);

	wxButton* btnClose = new wxButton(this, wxID_CANCEL, L"Close",
		wxDefaultPosition, wxDefaultSize, 0);

	// Sizers (sizer creation is in same order as wxFormBuilder)

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);
	bSizer->Add(staticText, 0, wxALL|wxEXPAND, 5);
	bSizer->Add(m_ctrlEncoded, 2, wxALL|wxEXPAND, 5);
	bSizer->Add(m_ctrlDecoded, 3, wxALL|wxEXPAND, 5);

	wxBoxSizer* sizeBtns = new wxBoxSizer(wxHORIZONTAL);
	sizeBtns->Add(btnDecode, 0, wxALL, 5);
	sizeBtns->Add(0, 0, 1, wxEXPAND, 0);
	sizeBtns->Add(btnClose, 0, wxALL, 5);

	bSizer->Add(sizeBtns, 0, wxEXPAND, 0);

	SetSizer(bSizer);
	Layout();
	bSizer->Fit(this);
	SetSizeHints(GetSize(), wxDefaultSize);
	CenterOnScreen();
}


void CDlgPageDecode::OnDecode(wxCommandEvent& evt)
{
	wxBusyCursor wait;
	std::wostringstream editData;
	std::wstring data = StringUtil::stringW(m_ctrlEncoded->GetValue());

	// Make sure this is synchronized (order of decoding) with
	// DlgARBHelp (encoder)
	std::wstring::size_type pos = data.find(STREAM_DATA_BEGIN);
	if (std::wstring::npos != pos)
	{
		data = data.substr(pos + wcslen(STREAM_DATA_BEGIN));
		pos = data.find(STREAM_DATA_END);
		if (std::wstring::npos != pos)
			data = data.substr(0, pos);
		data = StringUtil::Trim(data);

		std::wstring dataIn(data);
		data.clear();
		BinaryData::DecodeString(dataIn, data);
		dataIn.clear();
		data = StringUtil::Trim(data);

		editData << L"Any temporary files created will be deleted upon closing this window.\n\n";

		static const struct
		{
			std::wstring begin;
			std::wstring end;
		} sc_sections[] = {
			{STREAM_SYSTEM_BEGIN, STREAM_SYSTEM_END},
			{STREAM_REGISTRY_BEGIN, STREAM_REGISTRY_END},
			{L"", L""}
		};
		for (int idx = 0; !sc_sections[idx].begin.empty(); ++idx)
		{
			pos = data.find(sc_sections[idx].begin);
			if (std::wstring::npos != pos)
			{
				std::wstring::size_type posEnd = data.find(sc_sections[idx].end);
				if (pos < posEnd)
				{
					size_t posData = pos + sc_sections[idx].begin.length();
					// Dump the preceding data.
					editData << data.substr(0, posData) << L"\n";
					// Trim preceding
					data = data.substr(posData);
					data = StringUtil::TrimLeft(data);
					// Get the data to decode
					posEnd = data.find(sc_sections[idx].end); // Recompute - we just changed the string
					dataIn = data.substr(0, posEnd);
					// Strip that from main data.
					data = data.substr(posEnd + sc_sections[idx].end.length());
					data = StringUtil::TrimLeft(data);
					// Now decode
					std::wstring dataOut;
					BinaryData::DecodeString(dataIn, dataOut);
					dataIn.clear();
					editData << dataOut << sc_sections[idx].end << L"\n\n";
					dataOut.clear();
				}
			}
		}

		while ((std::wstring::npos != (pos = data.find(STREAM_FILE_BEGIN))))
		{
			std::wstring::size_type posEnd = data.find(STREAM_FILE_END);
			if (0 < posEnd && posEnd > pos)
			{
				std::wstring::size_type posData = pos + wcslen(STREAM_FILE_BEGIN);
				// Dump the preceding data (but not identifier.
				editData << data.substr(0, pos); // New line included
				// Trim preceding
				data = data.substr(posData);
				data = StringUtil::TrimLeft(data);
				// Get the data to decode
				posEnd = data.find(STREAM_FILE_END); // Recompute - we just changed the string
				dataIn = data.substr(0, posEnd);
				// Strip that from main data.
				data = data.substr(posEnd + wcslen(STREAM_FILE_END));
				data = StringUtil::TrimLeft(data);
				// Now decode
				unsigned char* binData = nullptr;
				size_t nBytes = 0;
				BinaryData::Decode(dataIn, binData, nBytes);
				dataIn.clear();
				// Generate a temp file name
				wxString tempname = wxFileName::CreateTempFileName(L"arb");
				m_TmpFiles.push_back(tempname);
				wxFFile output;
				if (output.Open(tempname, L"wb"))
				{
					output.Write(binData, nBytes);
					output.Close();
					editData << L"File written to: " << tempname.wx_str() << L"\n\n";
				}
				else
				{
					std::string tmp(reinterpret_cast<char*>(binData), nBytes);
					editData << STREAM_FILE_BEGIN << L"\n"
						<< StringUtil::stringWX(tmp)
						<< STREAM_FILE_END << L"\n\n";
				}
				BinaryData::Release(binData);
			}
		}
		editData << data;
	}

	else
	{
		editData << L"Error in data: Unable to find " << STREAM_DATA_BEGIN;
	}

	m_ctrlDecoded->SetValue(editData.str().c_str());
}


static void ClearFile(wxString const& inFile)
{
	wxRemoveFile(inFile);
}


void CDlgPageDecode::OnCancel(wxCommandEvent& evt)
{
	std::for_each(m_TmpFiles.begin(), m_TmpFiles.end(), ClearFile);
	Destroy();
}
