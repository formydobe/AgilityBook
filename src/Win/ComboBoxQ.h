#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief Special comboboxes
 * @author David Connet
 *
 * Revision History
 * 2013-04-15 Separated from ComboBoxes.h
 */

#include "ARB/ARBTypes2.h"
#include "ARB/ARB_Q.h"


/// Combobox for Qs
class CQualifyingComboBox : public wxChoice
{
	DECLARE_CLASS(CQualifyingComboBox)
public:
	CQualifyingComboBox(
		wxWindow* parent,
		ARBDogReferenceRunPtr const& inRefRun,
		wxValidator const& validator = wxDefaultValidator);
	CQualifyingComboBox(wxWindow* parent, ARBDogRunPtr const& inRun, wxValidator const& validator = wxDefaultValidator);
	CQualifyingComboBox(
		wxWindow* parent,
		ARBDogRunPtr const& inRun,
		ARBConfigScoringPtr const& inScoring,
		wxValidator const& validator = wxDefaultValidator);

	void ResetContent(ARBConfigScoringPtr const& inScoring);
	ARB_Q GetQ(int index) const;

private:
	ARBDogReferenceRunPtr m_refRun;
	ARBDogRunPtr m_Run;
};
