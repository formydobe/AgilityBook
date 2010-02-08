/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See Licence.txt
 */

/**
 * @file
 *
 * @brief implementation of the CDlgPageDecode class
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2009-08-26 DRC Fixed streaming wxString to otstringstream.
 *                    Fixed decoding binary files.
 * @li 2009-03-01 DRC Ported to wxWidgets.
 * @li 2007-01-02 DRC Created
 */

#include "stdafx.h"
#include "DlgPageDecode.h"

#include "../ARB/VersionNum.h"
#include "../Win/Widgets.h"
#include "ARBHelp.h"
#include "ARBTypes.h"
#include "BinaryData.h"
#include "DlgARBHelp.h"
#include <algorithm>
#include <wx/ffile.h>
#include <wx/filename.h>


BEGIN_EVENT_TABLE(CDlgPageDecode, wxDialog)
	EVT_BUTTON(wxID_CANCEL, CDlgPageDecode::OnCancel)
END_EVENT_TABLE()


CDlgPageDecode::CDlgPageDecode()
	: wxDialog(NULL, wxID_ANY, wxT("Decode"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX | wxRESIZE_BORDER)
{
	// Controls (these are done first to control tab order)

	wxString str(wxT("In order to decode the gathered information, first select all the text from '"));
	str += STREAM_DATA_BEGIN;
	str += wxT("' to '");
	str += STREAM_DATA_END;
	str += wxT("' and copy here. Make sure to include those strings! Then press 'Decode'.");

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

	wxButton* btnDecode = new wxButton(this, wxID_ANY, wxT("Decode"),
		wxDefaultPosition, wxDefaultSize, 0);
	btnDecode->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgPageDecode::OnDecode), NULL, this);

	wxButton* btnClose = new wxButton(this, wxID_CANCEL, wxT("Close"),
		wxDefaultPosition, wxDefaultSize, 0);

	// Sizers (sizer creation is in same order as wxFormBuilder)

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);
	bSizer->Add(staticText, 0, wxALL|wxEXPAND, 5);
	bSizer->Add(m_ctrlEncoded, 2, wxALL|wxEXPAND, 5);
	bSizer->Add(m_ctrlDecoded, 3, wxALL|wxEXPAND, 5);

	wxBoxSizer* sizeBtns = new wxBoxSizer(wxHORIZONTAL);
	sizeBtns->Add(btnDecode, 0, wxALL, 5);
	sizeBtns->Add(0, 0, 1, wxEXPAND, 5);
	sizeBtns->Add(btnClose, 0, wxALL, 5);

	bSizer->Add(sizeBtns, 0, wxEXPAND, 5);

	SetSizer(bSizer);
	Layout();
	bSizer->Fit(this);
	SetSizeHints(GetSize(), wxDefaultSize);
	CenterOnScreen();
}


void CDlgPageDecode::OnDecode(wxCommandEvent& evt)
{
	wxBusyCursor wait;
	wxString editData;
	wxString data = m_ctrlEncoded->GetValue();

	// Make sure this is synchronized (order of decoding) with
	// DlgARBHelp (encoder)
	int pos = data.Find(STREAM_DATA_BEGIN);
	if (0 <= pos)
	{
		data = data.Mid(pos + wxString(STREAM_DATA_BEGIN).length());
		pos = data.Find(STREAM_DATA_END);
		if (0 <= pos)
			data = data.Left(pos);
		data.Trim(true);
		data.Trim(false);

		wxString dataIn(data);
		data.Empty();
		BinaryData::DecodeString(dataIn, data);
		dataIn.clear();
		data.Trim(true);
		data.Trim(false);

		editData << wxT("Any temporary files created will be deleted upon closing this window.\n\n");

		static const struct
		{
			wxString begin;
			wxString end;
		} sc_sections[] = {
			{STREAM_SYSTEM_BEGIN, STREAM_SYSTEM_END},
			{STREAM_REGISTRY_BEGIN, STREAM_REGISTRY_END},
			{wxT(""), wxT("")}
		};
		for (int idx = 0; !sc_sections[idx].begin.empty(); ++idx)
		{
			pos = data.Find(sc_sections[idx].begin);
			if (0 <= pos)
			{
				int posEnd = data.Find(sc_sections[idx].end);
				if (pos < posEnd)
				{
					size_t posData = pos + static_cast<int>(sc_sections[idx].begin.Length());
					// Dump the preceding data.
					editData << data.Left(posData) << wxT("\n");
					// Trim preceding
					data = data.Mid(posData);
					data.Trim(false);
					// Get the data to decode
					posEnd = data.Find(sc_sections[idx].end); // Recompute - we just changed the string
					dataIn = data.Left(posEnd);
					// Strip that from main data.
					data = data.Mid(posEnd + sc_sections[idx].end.Length());
					data.Trim(false);
					// Now decode
					wxString dataOut;
					BinaryData::DecodeString(dataIn, dataOut);
					dataIn.clear();
					editData << dataOut << sc_sections[idx].end << wxT("\n\n");
					dataOut.clear();
				}
			}
		}

		while ((0 <= (pos = data.Find(STREAM_FILE_BEGIN))))
		{
			int posEnd = data.Find(STREAM_FILE_END);
			if (0 < posEnd && posEnd > pos)
			{
				int posData = pos + static_cast<int>(wxString(STREAM_FILE_BEGIN).length());
				// Dump the preceding data (but not identifier.
				editData << data.Left(pos); // New line included
				// Trim preceding
				data = data.Mid(posData);
				data.Trim(false);
				// Get the data to decode
				posEnd = data.Find(STREAM_FILE_END); // Recompute - we just changed the string
				dataIn = data.Left(posEnd);
				// Strip that from main data.
				data = data.Mid(posEnd + wxString(STREAM_FILE_END).length());
				data.Trim(false);
				// Now decode
				unsigned char* binData = NULL;
				size_t nBytes = 0;
				BinaryData::Decode(dataIn, binData, nBytes);
				dataIn.clear();
				// Generate a temp file name
				wxString tempname = wxFileName::CreateTempFileName(wxT("arb"));
				m_TmpFiles.push_back(tempname);
				wxFFile output;
				if (output.Open(tempname, wxT("wb")))
				{
					output.Write(binData, nBytes);
					output.Close();
					editData << wxT("File written to: ") << tempname << wxT("\n\n");
				}
				else
				{
					std::string tmp(reinterpret_cast<char*>(binData), nBytes);
					editData << STREAM_FILE_BEGIN << wxT("\n")
						<< tstringUtil::TString(tmp)
						<< STREAM_FILE_END << wxT("\n\n");
				}
				BinaryData::Release(binData);
			}
		}
		editData << data;
	}

	else
	{
		editData << wxT("Error in data: Unable to find ") << STREAM_DATA_BEGIN;
	}

	m_ctrlDecoded->SetValue(editData);
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