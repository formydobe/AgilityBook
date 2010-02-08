/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See Licence.txt
 */

/**
 * @file
 *
 * @brief implementation of the CDlgConfigEventMethod class
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2009-08-12 DRC Fix dates on save.
 * @li 2009-02-11 DRC Ported to wxWidgets.
 * @li 2006-11-04 DRC Created.
 */

#include "stdafx.h"
#include "DlgConfigEventMethod.h"

#include "AgilityBook.h"
#include "ARBAgilityRecordBook.h"
#include "ARBConfigEvent.h"
#include "ARBConfigVenue.h"
#include "DlgConfigPlaceInfo.h"
#include "DlgConfigTitlePoints.h"
#include "DlgConfigure.h"
#include "DlgConfigureData.h"
#include "DlgName.h"
#include "ListCtrl.h"
#include "ListData.h"
#include "Validators.h"
#include <wx/datectrl.h>


/////////////////////////////////////////////////////////////////////////////

class CDlgConfigureMethodDataDivision : public wxClientData
{
public:
	CDlgConfigureMethodDataDivision(ARBConfigDivisionPtr div)
		: m_Div(div)
	{
	}
	ARBConfigDivisionPtr m_Div;
};


class CDlgConfigureDataPlacement : public CListData
{
public:
	CDlgConfigureDataPlacement(short inPlace, double inVal)
		: m_Place(inPlace)
		, m_Value(inVal)
	{
	}
	virtual wxString OnNeedText(long iColumn) const;
	short Place() const					{return m_Place;}
	void Place(short place)				{m_Place = place;}
	double Value() const				{return m_Value;}
	void Value(double val)				{m_Value = val;}
protected:
	short m_Place;
	double m_Value;
};


wxString CDlgConfigureDataPlacement::OnNeedText(long iColumn) const
{
	wxString str;
	switch (iColumn)
	{
	default:
		break;
	case 0:
		str << m_Place;
		break;
	case 1:
		str << m_Value;
		break;
	}
	return str;
}


static struct
{
	CDlgConfigEventMethod* pThis;
} s_SortInfo;


#if wxCHECK_VERSION(2,9,1)
int wxCALLBACK ComparePlacement(long item1, long item2, wxIntPtr sortData)
#else
int wxCALLBACK ComparePlacement(long item1, long item2, long sortData)
#endif
{
	CDlgConfigureDataPlacementPtr place1 = s_SortInfo.pThis->GetPlacementDataByData(item1);
	CDlgConfigureDataPlacementPtr place2 = s_SortInfo.pThis->GetPlacementDataByData(item2);
	return place1->Place() > place2->Place();
}

/////////////////////////////////////////////////////////////////////////////
// CDlgConfigEventMethod dialog

BEGIN_EVENT_TABLE(CDlgConfigEventMethod, wxDialog)
	EVT_BUTTON(wxID_OK, CDlgConfigEventMethod::OnOk)
END_EVENT_TABLE()


CDlgConfigEventMethod::CDlgConfigEventMethod(
		ARBConfigVenuePtr pVenue,
		ARBConfigScoringPtr pScoring,
		wxWindow* pParent)
	: wxDialog()
	, m_pVenue(pVenue)
	, m_pScoring(pScoring)
	, m_PlaceInfo()
	, m_ctrlDivision(NULL)
	, m_ctrlLevel(NULL)
	, m_ctrlValidFrom(NULL)
	, m_ctrlDateFrom(NULL)
	, m_ctrlValidTo(NULL)
	, m_ctrlDateTo(NULL)
	, m_ctrlType(NULL)
	, m_ctrlDropFractions(NULL)
	, m_ctrlBonus(NULL)
	, m_ctrlSuperQ(NULL)
	, m_ctrlSpeedPts(NULL)
	, m_ctrlMultiplyText(NULL)
	, m_ctrlMultiply(NULL)
	, m_ctrlPlacementText(NULL)
	, m_ctrlPlacement(NULL)
	, m_ctrlPlacementNew(NULL)
	, m_ctrlPlacementEdit(NULL)
	, m_ctrlPlacementDelete(NULL)
	, m_ctrlTimeFaultsCleanQ(NULL)
	, m_ctrlTimeFaultsUnder(NULL)
	, m_ctrlTimeFaultsOver(NULL)
	, m_ctrlSubtractTimeFaults(NULL)
	, m_ctrlPointsOpeningText(NULL)
	, m_ctrlPointsOpening(NULL)
	, m_ctrlPointsClosingText(NULL)
	, m_ctrlPointsClosing(NULL)
	, m_dateFrom(m_pScoring->GetValidFrom())
	, m_dateTo(m_pScoring->GetValidTo())
	, m_DropFractions(m_pScoring->DropFractions())
	, m_Bonus(m_pScoring->HasBonusPts())
	, m_SuperQ(m_pScoring->HasSuperQ())
	, m_SpeedPts(m_pScoring->HasSpeedPts())
	, m_Multiply(m_pScoring->TimeFaultMultiplier())
	, m_TimeFaultsCleanQ(m_pScoring->QsMustBeClean())
	, m_TimeFaultsUnder(m_pScoring->ComputeTimeFaultsUnder())
	, m_TimeFaultsOver(m_pScoring->ComputeTimeFaultsOver())
	, m_SubtractTimeFaults(m_pScoring->SubtractTimeFaultsFromScore())
	, m_OpeningPts(m_pScoring->GetRequiredOpeningPoints())
	, m_ClosingPts(m_pScoring->GetRequiredClosingPoints())
{
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP);
	if (!pParent)
		pParent = wxGetApp().GetTopWindow();
	Create(pParent, wxID_ANY, _("IDD_CONFIG_EVENT_METHOD"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);

	// Copy the existing scorings.
	assert(m_pVenue);
	assert(m_pScoring);
	m_pScoring->GetPlaceInfo().Clone(m_PlaceInfo);

	// Controls (these are done first to control tab order)

	wxStaticText* textDiv = new wxStaticText(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_DIVISION"),
		wxDefaultPosition, wxDefaultSize, 0);
	textDiv->Wrap(-1);

	m_ctrlDivision = new wxComboBox(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize,
		0, NULL, 0);
	m_ctrlDivision->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(CDlgConfigEventMethod::OnSelchangeDivision), NULL, this);
	m_ctrlDivision->SetHelpText(_("HIDC_CONFIG_EVENT_DIVISION"));
	m_ctrlDivision->SetToolTip(_("HIDC_CONFIG_EVENT_DIVISION"));
	wxString all = _("IDS_ALL");
	int index = m_ctrlDivision->Append(all);
	if (m_pScoring->GetDivision() == WILDCARD_DIVISION)
		m_ctrlDivision->SetSelection(index);
	for (ARBConfigDivisionList::iterator iterDiv = m_pVenue->GetDivisions().begin();
		iterDiv != m_pVenue->GetDivisions().end();
		++iterDiv)
	{
		ARBConfigDivisionPtr pDiv = (*iterDiv);
		index = m_ctrlDivision->Append(pDiv->GetName());
		m_ctrlDivision->SetClientObject(index, new CDlgConfigureMethodDataDivision(pDiv));
		if (m_pScoring->GetDivision() == pDiv->GetName())
			m_ctrlDivision->SetSelection(index);
	}

	wxStaticText* textLevel = new wxStaticText(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_LEVEL"),
		wxDefaultPosition, wxDefaultSize, 0);
	textLevel->Wrap(-1);

	m_ctrlLevel = new wxComboBox(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize,
		0, NULL, 0);
	m_ctrlLevel->SetHelpText(_("HIDC_CONFIG_EVENT_LEVEL"));
	m_ctrlLevel->SetToolTip(_("HIDC_CONFIG_EVENT_LEVEL"));

	m_ctrlValidFrom = new wxCheckBox(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_DATE_VALID_FROM"),
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	m_ctrlValidFrom->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(CDlgConfigEventMethod::OnValidFrom), NULL, this);
	m_ctrlValidFrom->SetHelpText(_("HIDC_CONFIG_EVENT_DATE_VALID_FROM"));
	m_ctrlValidFrom->SetToolTip(_("HIDC_CONFIG_EVENT_DATE_VALID_FROM"));

	wxDateTime date(wxDateTime::Now());
	if (m_dateFrom.IsValid())
	{
		m_ctrlValidFrom->SetValue(true);
		date = m_dateFrom.GetDate();
	}
	else
	{
		m_ctrlValidFrom->SetValue(false);
	}
	m_ctrlDateFrom = new wxDatePickerCtrl(this, wxID_ANY, date,
		wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN|wxDP_SHOWCENTURY,
		CGenericValidator(&m_dateFrom));
	m_ctrlDateFrom->SetHelpText(_("HIDC_CONFIG_EVENT_DATE_START"));
	m_ctrlDateFrom->SetToolTip(_("HIDC_CONFIG_EVENT_DATE_START"));
	if (m_pScoring->GetValidFrom().IsValid())
		m_ctrlDateFrom->Enable(true);
	else
		m_ctrlDateFrom->Enable(false);

	m_ctrlValidTo = new wxCheckBox(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_DATE_VALID_TO"),
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	m_ctrlValidTo->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(CDlgConfigEventMethod::OnValidTo), NULL, this);
	m_ctrlValidTo->SetHelpText(_("HIDC_CONFIG_EVENT_DATE_VALID_TO"));
	m_ctrlValidTo->SetToolTip(_("HIDC_CONFIG_EVENT_DATE_VALID_TO"));

	date = wxDateTime::Now();
	if (m_dateTo.IsValid())
	{
		m_ctrlValidTo->SetValue(true);
		date = m_dateTo.GetDate();
	}
	else
	{
		m_ctrlValidTo->SetValue(false);
	}
	m_ctrlDateTo = new wxDatePickerCtrl(this, wxID_ANY, date,
		wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN|wxDP_SHOWCENTURY,
		CGenericValidator(&m_dateTo));
	m_ctrlDateTo->SetHelpText(_("HIDC_CONFIG_EVENT_DATE_END"));
	m_ctrlDateTo->SetToolTip(_("HIDC_CONFIG_EVENT_DATE_END"));
	if (m_pScoring->GetValidTo().IsValid())
		m_ctrlDateTo->Enable(true);
	else
		m_ctrlDateTo->Enable(false);

	wxStaticText* textStyle = new wxStaticText(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_TYPE"),
		wxDefaultPosition, wxDefaultSize, 0);
	textStyle->Wrap(-1);

	m_ctrlType = new wxComboBox(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize,
		0, NULL, wxCB_DROPDOWN|wxCB_READONLY); 
	m_ctrlType->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(CDlgConfigEventMethod::OnSelchangeType), NULL, this);
	m_ctrlType->SetHelpText(_("HIDC_CONFIG_EVENT_TYPE"));
	m_ctrlType->SetToolTip(_("HIDC_CONFIG_EVENT_TYPE"));
	// If any additional types are added in ARBConfigScoring,
	// they'll have to be manually added here...
	static ARBConfigScoring::ScoringStyle const Styles[] =
	{
		ARBConfigScoring::eFaultsThenTime,
		ARBConfigScoring::eFaults100ThenTime,
		ARBConfigScoring::eFaults200ThenTime,
		ARBConfigScoring::eOCScoreThenTime,
		ARBConfigScoring::eScoreThenTime,
		ARBConfigScoring::eTimePlusFaults
	};
	static int const nStyles = sizeof(Styles) / sizeof(Styles[0]);
	for (index = 0; index < nStyles; ++index)
	{
		wxString str = ARBConfigScoring::GetScoringStyleStr(Styles[index]);
		int idx = m_ctrlType->Append(str);
		m_ctrlType->SetClientData(idx, reinterpret_cast<void*>(Styles[index]));
	}

	m_ctrlDropFractions = new wxCheckBox(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_DROP_FRACTIONS"),
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_DropFractions));
	m_ctrlDropFractions->SetHelpText(_("HIDC_CONFIG_EVENT_DROP_FRACTIONS"));
	m_ctrlDropFractions->SetToolTip(_("HIDC_CONFIG_EVENT_DROP_FRACTIONS"));

	m_ctrlBonus = new wxCheckBox(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_BONUS"),
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_Bonus));
	m_ctrlBonus->SetHelpText(_("HIDC_CONFIG_EVENT_BONUS"));
	m_ctrlBonus->SetToolTip(_("HIDC_CONFIG_EVENT_BONUS"));

	m_ctrlSuperQ = new wxCheckBox(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_SUPERQ"),
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_SuperQ));
	m_ctrlSuperQ->SetHelpText(_("HIDC_CONFIG_EVENT_SUPERQ"));
	m_ctrlSuperQ->SetToolTip(_("HIDC_CONFIG_EVENT_SUPERQ"));

	m_ctrlSpeedPts = new wxCheckBox(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_SPEED"),
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_SpeedPts));
	m_ctrlSpeedPts->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(CDlgConfigEventMethod::OnSpeedPoints), NULL, this);
	m_ctrlSpeedPts->SetHelpText(_("HIDC_CONFIG_EVENT_SPEED"));
	m_ctrlSpeedPts->SetToolTip(_("HIDC_CONFIG_EVENT_SPEED"));

	m_ctrlMultiplyText = new wxStaticText(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_TF_MULTIPLY"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlMultiplyText->Wrap(-1);

	m_ctrlMultiply = new CTextCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(30, -1), 0,
		CGenericValidator(&m_Multiply));
	m_ctrlMultiply->SetHelpText(_("HIDC_CONFIG_EVENT_TF_MULTIPLY"));
	m_ctrlMultiply->SetToolTip(_("HIDC_CONFIG_EVENT_TF_MULTIPLY"));

	m_ctrlPlacementText = new wxStaticText(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_PLACEMENT"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlPlacementText->Wrap(-1);

	m_ctrlPlacement = new CReportListCtrl(this,
		wxDefaultPosition, wxDefaultSize,
		true, CReportListCtrl::eNoSortHeader, true);
	m_ctrlPlacement->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(CDlgConfigEventMethod::OnItemchangedPlacement), NULL, this);
	m_ctrlPlacement->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(CDlgConfigEventMethod::OnItemActivatedPlacement), NULL, this);
	m_ctrlPlacement->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CDlgConfigEventMethod::OnKeydownPlacement), NULL, this);
	m_ctrlPlacement->SetHelpText(_("HIDC_CONFIG_EVENT_PLACEMENT"));
	m_ctrlPlacement->SetToolTip(_("HIDC_CONFIG_EVENT_PLACEMENT"));
	m_ctrlPlacement->InsertColumn(0, _("IDS_COL_PLACE"));
	m_ctrlPlacement->InsertColumn(1, _("IDS_COL_MULTIPLIER"));
	index = 0;
	for (ARBConfigPlaceInfoList::iterator iterPlace = m_PlaceInfo.begin();
		iterPlace != m_PlaceInfo.end();
		++index, ++iterPlace)
	{
		CDlgConfigureDataPlacementPtr pData(new CDlgConfigureDataPlacement(
			(*iterPlace)->GetPlace(), (*iterPlace)->GetValue()));
		m_ctrlPlacement->InsertItem(pData);
	}
	s_SortInfo.pThis = this;
	m_ctrlPlacement->SortItems(ComparePlacement, 0);
	m_ctrlPlacement->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	m_ctrlPlacement->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);

	m_ctrlPlacementNew = new wxButton(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_PLACEMENT_NEW"),
		wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	m_ctrlPlacementNew->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgConfigEventMethod::OnPlacementNew), NULL, this);
	m_ctrlPlacementNew->SetHelpText(_("HIDC_CONFIG_EVENT_PLACEMENT_NEW"));
	m_ctrlPlacementNew->SetToolTip(_("HIDC_CONFIG_EVENT_PLACEMENT_NEW"));

	m_ctrlPlacementEdit = new wxButton(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_PLACEMENT_EDIT"),
		wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	m_ctrlPlacementEdit->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgConfigEventMethod::OnPlacementEdit), NULL, this);
	m_ctrlPlacementEdit->SetHelpText(_("HIDC_CONFIG_EVENT_PLACEMENT_EDIT"));
	m_ctrlPlacementEdit->SetToolTip(_("HIDC_CONFIG_EVENT_PLACEMENT_EDIT"));

	m_ctrlPlacementDelete = new wxButton(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_PLACEMENT_DELETE"),
		wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	m_ctrlPlacementDelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgConfigEventMethod::OnPlacementDelete), NULL, this);
	m_ctrlPlacementDelete->SetHelpText(_("HIDC_CONFIG_EVENT_PLACEMENT_DELETE"));
	m_ctrlPlacementDelete->SetToolTip(_("HIDC_CONFIG_EVENT_PLACEMENT_DELETE"));

	m_ctrlTimeFaultsCleanQ = new wxCheckBox(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_TIME_FAULTS_CLEANQ"),
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_TimeFaultsCleanQ));
	m_ctrlTimeFaultsCleanQ->SetHelpText(_("HIDC_CONFIG_EVENT_TIME_FAULTS_CLEANQ"));
	m_ctrlTimeFaultsCleanQ->SetToolTip(_("HIDC_CONFIG_EVENT_TIME_FAULTS_CLEANQ"));

	m_ctrlTimeFaultsUnder = new wxCheckBox(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_TIME_FAULTS_UNDER"),
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_TimeFaultsUnder));
	m_ctrlTimeFaultsUnder->SetHelpText(_("HIDC_CONFIG_EVENT_TIME_FAULTS_UNDER"));
	m_ctrlTimeFaultsUnder->SetToolTip(_("HIDC_CONFIG_EVENT_TIME_FAULTS_UNDER"));

	m_ctrlTimeFaultsOver = new wxCheckBox(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_TIME_FAULTS_OVER"),
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_TimeFaultsOver));
	m_ctrlTimeFaultsOver->SetHelpText(_("HIDC_CONFIG_EVENT_TIME_FAULTS_OVER"));
	m_ctrlTimeFaultsOver->SetToolTip(_("HIDC_CONFIG_EVENT_TIME_FAULTS_OVER"));

	m_ctrlSubtractTimeFaults = new wxCheckBox(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_SUBTRACT_TIME_FAULTS"),
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_SubtractTimeFaults));
	m_ctrlSubtractTimeFaults->SetHelpText(_("HIDC_CONFIG_EVENT_SUBTRACT_TIME_FAULTS"));
	m_ctrlSubtractTimeFaults->SetToolTip(_("HIDC_CONFIG_EVENT_SUBTRACT_TIME_FAULTS"));

	m_ctrlPointsOpeningText = new wxStaticText(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_OPENING_PTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlPointsOpeningText->Wrap(-1);

	m_ctrlPointsOpening = new CTextCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(30, -1), 0,
		CGenericValidator(&m_OpeningPts));
	m_ctrlPointsOpening->SetHelpText(_("HIDC_CONFIG_EVENT_OPENING_PTS"));
	m_ctrlPointsOpening->SetToolTip(_("HIDC_CONFIG_EVENT_OPENING_PTS"));

	m_ctrlPointsClosingText = new wxStaticText(this, wxID_ANY,
		_("IDC_CONFIG_EVENT_CLOSING_PTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlPointsClosingText->Wrap(-1);

	m_ctrlPointsClosing = new CTextCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(30, -1), 0,
		CGenericValidator(&m_ClosingPts));
	m_ctrlPointsClosing->SetHelpText(_("HIDC_CONFIG_EVENT_CLOSING_PTS"));
	m_ctrlPointsClosing->SetToolTip(_("HIDC_CONFIG_EVENT_CLOSING_PTS"));

	// Sizers (sizer creation is in same order as wxFormBuilder)

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* sizerDivDates = new wxBoxSizer(wxHORIZONTAL);

	wxFlexGridSizer* sizerDiv = new wxFlexGridSizer(2, 2, 0, 0);
	sizerDiv->SetFlexibleDirection(wxBOTH);
	sizerDiv->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sizerDiv->Add(textDiv, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5);
	sizerDiv->Add(m_ctrlDivision, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerDiv->Add(textLevel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5);
	sizerDiv->Add(m_ctrlLevel, 1, wxALL, 5);

	sizerDivDates->Add(sizerDiv, 0, wxEXPAND, 5);

	wxFlexGridSizer* sizerDates = new wxFlexGridSizer(2, 2, 0, 0);
	sizerDates->SetFlexibleDirection(wxBOTH);
	sizerDates->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sizerDates->Add(m_ctrlValidFrom, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5);
	sizerDates->Add(m_ctrlDateFrom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerDates->Add(m_ctrlValidTo, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5);
	sizerDates->Add(m_ctrlDateTo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	sizerDivDates->Add(sizerDates, 1, wxEXPAND, 5);

	bSizer->Add(sizerDivDates, 0, wxEXPAND, 5);

	wxBoxSizer* sizerStyle = new wxBoxSizer(wxHORIZONTAL);
	sizerStyle->Add(textStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerStyle->Add(m_ctrlType, 0, wxALL, 5);

	bSizer->Add(sizerStyle, 0, wxEXPAND, 5);

	wxBoxSizer* sizerOptions = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* sizerCol1 = new wxBoxSizer(wxVERTICAL);
	sizerCol1->Add(m_ctrlDropFractions, 0, wxALL, 5);
	sizerCol1->Add(m_ctrlBonus, 0, wxALL, 5);
	sizerCol1->Add(m_ctrlSuperQ, 0, wxALL, 5);
	sizerCol1->Add(m_ctrlSpeedPts, 0, wxALL, 5);

	wxBoxSizer* sizerMultiplier = new wxBoxSizer(wxHORIZONTAL);
	sizerMultiplier->Add(m_ctrlMultiplyText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerMultiplier->Add(m_ctrlMultiply, 0, wxALL, 5);

	sizerCol1->Add(sizerMultiplier, 0, 0, 5);
	sizerCol1->Add(m_ctrlPlacementText, 0, wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* sizerPlacements = new wxBoxSizer(wxHORIZONTAL);

	sizerPlacements->Add(m_ctrlPlacement, 1, wxALL|wxEXPAND, 5);

	wxBoxSizer* sizerBtns = new wxBoxSizer(wxVERTICAL);
	sizerBtns->Add(m_ctrlPlacementNew, 0, wxALIGN_CENTER|wxALL, 5);
	sizerBtns->Add(m_ctrlPlacementEdit, 0, wxALIGN_CENTER|wxBOTTOM|wxLEFT|wxRIGHT, 5);
	sizerBtns->Add(m_ctrlPlacementDelete, 0, wxALIGN_CENTER|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerPlacements->Add(sizerBtns, 0, wxEXPAND, 5);

	sizerCol1->Add(sizerPlacements, 0, wxEXPAND, 5);

	sizerOptions->Add(sizerCol1, 1, wxEXPAND, 5);

	wxBoxSizer* sizerCol2 = new wxBoxSizer(wxVERTICAL);
	sizerCol2->Add(m_ctrlTimeFaultsCleanQ, 0, wxALL, 5);
	sizerCol2->Add(m_ctrlTimeFaultsUnder, 0, wxALL, 5);
	sizerCol2->Add(m_ctrlTimeFaultsOver, 0, wxALL, 5);
	sizerCol2->Add(m_ctrlSubtractTimeFaults, 0, wxALL, 5);

	wxFlexGridSizer* sizerPoints = new wxFlexGridSizer(2, 2, 0, 0);
	sizerPoints->SetFlexibleDirection(wxBOTH);
	sizerPoints->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sizerPoints->Add(m_ctrlPointsOpeningText, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5);
	sizerPoints->Add(m_ctrlPointsOpening, 0, wxALL, 5);
	sizerPoints->Add(m_ctrlPointsClosingText, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5);
	sizerPoints->Add(m_ctrlPointsClosing, 0, wxALL, 5);

	sizerCol2->Add(sizerPoints, 0, wxEXPAND, 5);

	sizerOptions->Add(sizerCol2, 1, wxEXPAND, 5);

	bSizer->Add(sizerOptions, 1, wxEXPAND, 5);

	wxSizer* sdbSizer = CreateSeparatedButtonSizer(wxOK|wxCANCEL);
	bSizer->Add(sdbSizer, 0, wxALL|wxEXPAND, 5);

	FillLevelList();
	m_ctrlType->SetSelection(-1);
	for (unsigned int idxType = 0; idxType < m_ctrlType->GetCount(); ++idxType)
	{
		ARBConfigScoring::ScoringStyle style = GetType(idxType);
		if (m_pScoring->GetScoringStyle() == style)
		{
			m_ctrlType->SetSelection(idxType);
			break;
		}
	}

	UpdateControls();
	UpdateButtons();

	SetSizer(bSizer);
	Layout();
	GetSizer()->Fit(this);
	SetSizeHints(GetSize(), wxDefaultSize);
	CenterOnParent();
}


ARBConfigScoring::ScoringStyle CDlgConfigEventMethod::GetType(int index) const
{
	return static_cast<ARBConfigScoring::ScoringStyle>(reinterpret_cast<int>(m_ctrlType->GetClientData(index)));
}


CDlgConfigureDataPlacementPtr CDlgConfigEventMethod::GetPlacementData(int index) const
{
	return std::tr1::dynamic_pointer_cast<CDlgConfigureDataPlacement, CListData>(m_ctrlPlacement->GetData(index));
}


CDlgConfigureDataPlacementPtr CDlgConfigEventMethod::GetPlacementDataByData(int index) const
{
	return std::tr1::dynamic_pointer_cast<CDlgConfigureDataPlacement, CListData>(m_ctrlPlacement->GetDataByData(index));
}


void CDlgConfigEventMethod::UpdateButtons()
{
	bool bEdit = false;
	bool bDelete = false;
	if (m_SpeedPts && 0 <= m_ctrlPlacement->GetFirstSelected())
		bEdit = bDelete = true;
	m_ctrlPlacementEdit->Enable(bEdit);
	m_ctrlPlacementDelete->Enable(bDelete);
}


void CDlgConfigEventMethod::UpdateControls()
{
	int idxType = m_ctrlType->GetSelection();
	if (wxNOT_FOUND == idxType)
	{
		m_ctrlPointsOpeningText->Show(false);
		m_ctrlPointsOpening->Show(false);
		m_ctrlPointsClosingText->Show(false);
		m_ctrlPointsClosing->Show(false);
		m_ctrlDropFractions->Show(false);
		m_ctrlTimeFaultsCleanQ->Show(false);
		m_ctrlSubtractTimeFaults->Show(false);
		m_ctrlTimeFaultsUnder->Show(false);
		m_ctrlTimeFaultsOver->Show(false);
		m_ctrlMultiplyText->Show(false);
		m_ctrlMultiply->Show(false);
	}
	else
	{
		switch (GetType(idxType))
		{
		default:
			m_ctrlPointsOpeningText->Show(false);
			m_ctrlPointsOpening->Show(false);
			m_ctrlPointsClosingText->Show(false);
			m_ctrlPointsClosing->Show(false);
			m_ctrlDropFractions->Show(false);
			m_ctrlTimeFaultsCleanQ->Show(false);
			m_ctrlSubtractTimeFaults->Show(false);
			m_ctrlTimeFaultsUnder->Show(false);
			m_ctrlTimeFaultsOver->Show(false);
			m_ctrlMultiplyText->Show(false);
			m_ctrlMultiply->Show(false);
			break;
		case ARBConfigScoring::eFaultsThenTime:
		case ARBConfigScoring::eFaults100ThenTime:
		case ARBConfigScoring::eFaults200ThenTime:
			m_ctrlPointsOpeningText->Show(false);
			m_ctrlPointsOpening->Show(false);
			m_ctrlPointsClosingText->Show(false);
			m_ctrlPointsClosing->Show(false);
			m_ctrlDropFractions->Show(true);
			m_ctrlTimeFaultsCleanQ->Show(false);
			m_ctrlSubtractTimeFaults->Show(false);
			m_ctrlTimeFaultsUnder->Show(false);
			m_ctrlTimeFaultsOver->Show(false);
			m_ctrlMultiplyText->Show(true);
			m_ctrlMultiply->Show(true);
			break;
		case ARBConfigScoring::eOCScoreThenTime:
			m_ctrlPointsOpeningText->Show(true);
			m_ctrlPointsOpening->Show(true);
			m_ctrlPointsClosingText->Show(true);
			m_ctrlPointsClosing->Show(true);
			m_ctrlDropFractions->Show(true);
			m_ctrlTimeFaultsCleanQ->Show(false);
			m_ctrlSubtractTimeFaults->Show(true);
			m_ctrlTimeFaultsUnder->Show(true);
			m_ctrlTimeFaultsOver->Show(true);
			m_ctrlMultiplyText->Show(false);
			m_ctrlMultiply->Show(false);
			m_ctrlPointsOpeningText->SetLabel(_("IDC_CONFIG_EVENT_OPENING_PTS"));
			break;
		case ARBConfigScoring::eScoreThenTime:
			m_ctrlPointsOpeningText->Show(true);
			m_ctrlPointsOpening->Show(true);
			m_ctrlPointsClosingText->Show(false);
			m_ctrlPointsClosing->Show(false);
			m_ctrlDropFractions->Show(true);
			m_ctrlTimeFaultsCleanQ->Show(false);
			m_ctrlSubtractTimeFaults->Show(true);
			m_ctrlTimeFaultsUnder->Show(true);
			m_ctrlTimeFaultsOver->Show(true);
			m_ctrlMultiplyText->Show(false);
			m_ctrlMultiply->Show(false);
			m_ctrlPointsOpeningText->SetLabel(_("IDS_SCORING_REQUIRED_POINTS"));
			break;
		case ARBConfigScoring::eTimePlusFaults:
			m_ctrlPointsOpeningText->Show(false);
			m_ctrlPointsOpening->Show(false);
			m_ctrlPointsClosingText->Show(false);
			m_ctrlPointsClosing->Show(false);
			m_ctrlDropFractions->Show(true);
			m_ctrlTimeFaultsCleanQ->Show(true);
			m_ctrlSubtractTimeFaults->Show(false);
			m_ctrlTimeFaultsUnder->Show(true);
			m_ctrlTimeFaultsOver->Show(true);
			m_ctrlMultiplyText->Show(true);
			m_ctrlMultiply->Show(true);
			break;
		}
	}
}


void CDlgConfigEventMethod::FillLevelList()
{
	m_ctrlLevel->Clear();
	wxString all = _("IDS_ALL");
	int index = m_ctrlLevel->Append(all);
	if (m_pScoring->GetLevel() == WILDCARD_LEVEL)
		m_ctrlLevel->SetSelection(index);
	int idxDiv = m_ctrlDivision->GetSelection();
	if (0 == idxDiv) // All
	{
		for (ARBConfigDivisionList::iterator iter = m_pVenue->GetDivisions().begin();
			iter != m_pVenue->GetDivisions().end();
			++iter)
		{
			ARBConfigDivisionPtr pDiv = (*iter);
			for (ARBConfigLevelList::iterator iterLevel = pDiv->GetLevels().begin();
				iterLevel != pDiv->GetLevels().end();
				++iterLevel)
			{
				if (wxNOT_FOUND == m_ctrlLevel->FindString((*iterLevel)->GetName(), true))
				{
					index = m_ctrlLevel->Append((*iterLevel)->GetName());
					if (m_pScoring->GetLevel() == (*iterLevel)->GetName())
						m_ctrlLevel->SetSelection(index);
				}
			}
		}
	}
	else if (0 < idxDiv)
	{
		CDlgConfigureMethodDataDivision* pDiv = dynamic_cast<CDlgConfigureMethodDataDivision*>(m_ctrlDivision->GetClientObject(idxDiv));
		for (ARBConfigLevelList::iterator iterLevel = pDiv->m_Div->GetLevels().begin();
			iterLevel != pDiv->m_Div->GetLevels().end();
			++iterLevel)
		{
			index = m_ctrlLevel->Append((*iterLevel)->GetName());
			if (m_pScoring->GetLevel() == (*iterLevel)->GetName())
				m_ctrlLevel->SetSelection(index);
		}
	}
}


void CDlgConfigEventMethod::DoPlacementEdit()
{
	int idx = m_ctrlPlacement->GetFirstSelected();
	if (0 > idx)
		return;
	std::vector<short> placements;
	int count = m_ctrlPlacement->GetItemCount();
	for (int index = 0; index < count; ++index)
	{
		CDlgConfigureDataPlacementPtr pData = GetPlacementData(index);
		placements.push_back(pData->Place());
	}
	CDlgConfigureDataPlacementPtr pData = GetPlacementData(idx);
	CDlgConfigPlaceInfo dlg(placements, pData->Place(), pData->Value(), this);
	if (wxID_OK == dlg.ShowModal())
	{
		pData->Place(dlg.GetPlace());
		pData->Value(dlg.GetValue());
		s_SortInfo.pThis = this;
		m_ctrlPlacement->SortItems(ComparePlacement, 0);
		m_ctrlPlacement->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
		m_ctrlPlacement->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
		UpdateButtons();
	}
}


void CDlgConfigEventMethod::OnSelchangeDivision(wxCommandEvent& evt)
{
	FillLevelList();
}


void CDlgConfigEventMethod::OnValidFrom(wxCommandEvent& evt)
{
	m_ctrlDateFrom->Enable(m_ctrlValidFrom->GetValue());
}


void CDlgConfigEventMethod::OnValidTo(wxCommandEvent& evt)
{
	m_ctrlDateTo->Enable(m_ctrlValidTo->GetValue());
}


void CDlgConfigEventMethod::OnSelchangeType(wxCommandEvent& evt)
{
	UpdateControls();
	Layout();
	GetSizer()->Fit(this);
}


void CDlgConfigEventMethod::OnSpeedPoints(wxCommandEvent& evt)
{
	TransferDataFromWindow();
	m_ctrlPlacementText->Show(m_SpeedPts);
	m_ctrlPlacement->Show(m_SpeedPts);
	m_ctrlPlacementNew->Show(m_SpeedPts);
	m_ctrlPlacementEdit->Show(m_SpeedPts);
	m_ctrlPlacementDelete->Show(m_SpeedPts);
	Layout();
	GetSizer()->Fit(this);
}


void CDlgConfigEventMethod::OnItemchangedPlacement(wxListEvent& evt)
{
	UpdateButtons();
}


void CDlgConfigEventMethod::OnItemActivatedPlacement(wxListEvent& evt)
{
	DoPlacementEdit();
}


void CDlgConfigEventMethod::OnKeydownPlacement(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode())
	{
	default:
		evt.Skip();
		break;
	case WXK_SPACE:
	case WXK_NUMPAD_SPACE:
		DoPlacementEdit();
		break;
	}
}


void CDlgConfigEventMethod::OnPlacementNew(wxCommandEvent& evt)
{
	std::vector<short> placements;
	int count = m_ctrlPlacement->GetItemCount();
	for (int index = 0; index < count; ++index)
	{
		CDlgConfigureDataPlacementPtr pData = GetPlacementData(index);
		placements.push_back(pData->Place());
	}
	CDlgConfigPlaceInfo dlg(placements, -1, 0.0, this);
	if (wxID_OK == dlg.ShowModal())
	{
		CDlgConfigureDataPlacementPtr pData(new CDlgConfigureDataPlacement(
			dlg.GetPlace(), dlg.GetValue()));
		m_ctrlPlacement->InsertItem(pData);
		s_SortInfo.pThis = this;
		m_ctrlPlacement->SortItems(ComparePlacement, 0);
		m_ctrlPlacement->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
		m_ctrlPlacement->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
		UpdateButtons();
	}
}


void CDlgConfigEventMethod::OnPlacementEdit(wxCommandEvent& evt)
{
	DoPlacementEdit();
}


void CDlgConfigEventMethod::OnPlacementDelete(wxCommandEvent& evt)
{
	int idx = m_ctrlPlacement->GetFirstSelected();
	if (0 > idx)
		return;
	m_ctrlPlacement->DeleteItem(idx);
	UpdateButtons();
}


void CDlgConfigEventMethod::OnOk(wxCommandEvent& evt)
{
	if (!Validate() || !TransferDataFromWindow())
		return;

	// Verify we have selections
	int idxDiv = m_ctrlDivision->GetSelection();
	if (wxNOT_FOUND == idxDiv)
	{
		wxMessageBox(_("IDS_SELECT_DIVISION"), wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_WARNING);
		m_ctrlDivision->SetFocus();
		return;
	}
	int idxLevel = m_ctrlLevel->GetSelection();
	if (wxNOT_FOUND == idxLevel)
	{
		wxMessageBox(_("IDS_SELECT_LEVEL"), wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_WARNING);
		m_ctrlLevel->SetFocus();
		return;
	}
	int idxType = m_ctrlType->GetSelection();
	if (wxNOT_FOUND == idxType)
	{
		wxMessageBox(_("IDS_SELECT_STYLE"), wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_WARNING);
		m_ctrlType->SetFocus();
		return;
	}

	if (!m_ctrlValidFrom->GetValue())
		m_dateFrom.clear();
	if (!m_ctrlValidTo->GetValue())
		m_dateTo.clear();

	// Validate that from-to dates are okay.
	if (m_dateFrom.IsValid() && m_dateTo.IsValid()
	&& m_dateFrom > m_dateTo)
	{
		wxMessageBox(_("IDS_CONFIGEVENT_DATEORDER"), wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_WARNING);
		m_ctrlDateTo->SetFocus();
		return;
	}

	// Validate placements.
	if (m_SpeedPts)
	{
		if (1 == m_ctrlPlacement->GetItemCount())
		{
			CDlgConfigureDataPlacementPtr pData = GetPlacementData(0);
			if (0 == pData->Place())
			{
				wxMessageBox(_("IDS_CONFIGEVENT_PLACEMENT0"), wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_WARNING);
				m_ctrlPlacementNew->SetFocus();
				return;
			}
		}
	}

	// Save it.
	wxString str;
	m_pScoring->SetValidFrom(m_dateFrom);
	m_pScoring->SetValidTo(m_dateTo);
	if (0 == idxDiv)
		str = WILDCARD_DIVISION;
	else
		str = m_ctrlDivision->GetString(idxDiv);
	m_pScoring->SetDivision(str);
	if (0 == idxLevel)
		str = WILDCARD_LEVEL;
	else
		str = m_ctrlLevel->GetString(idxLevel);
	m_pScoring->SetLevel(str);

	m_pScoring->SetDropFractions(m_DropFractions);
	m_pScoring->SetHasBonusPts(m_Bonus);
	m_pScoring->SetHasSuperQ(m_SuperQ);
	m_pScoring->SetHasSpeedPts(m_SpeedPts);
	if (m_SpeedPts)
	{
		m_PlaceInfo.clear();
		int count = m_ctrlPlacement->GetItemCount();
		for (int index = 0; index < count; ++index)
		{
			CDlgConfigureDataPlacementPtr pData = GetPlacementData(index);
			m_PlaceInfo.AddPlaceInfo(pData->Place(), pData->Value(), true);
		}
		m_pScoring->GetPlaceInfo() = m_PlaceInfo;
	}

	ARBConfigScoring::ScoringStyle style = GetType(idxType);
	m_pScoring->SetScoringStyle(style);
	switch (style)
	{
	default:
		break;
	case ARBConfigScoring::eFaultsThenTime:
	case ARBConfigScoring::eFaults100ThenTime:
	case ARBConfigScoring::eFaults200ThenTime:
		m_pScoring->SetTimeFaultMultiplier(m_Multiply);
		break;
	case ARBConfigScoring::eOCScoreThenTime:
		m_pScoring->SetRequiredOpeningPoints(m_OpeningPts);
		m_pScoring->SetRequiredClosingPoints(m_ClosingPts);
		m_pScoring->SetSubtractTimeFaultsFromScore(m_SubtractTimeFaults);
		m_pScoring->SetComputeTimeFaultsUnder(m_TimeFaultsUnder);
		m_pScoring->SetComputeTimeFaultsOver(m_TimeFaultsOver);
		break;
	case ARBConfigScoring::eScoreThenTime:
		m_pScoring->SetRequiredOpeningPoints(m_OpeningPts);
		m_pScoring->SetSubtractTimeFaultsFromScore(m_SubtractTimeFaults);
		m_pScoring->SetComputeTimeFaultsUnder(m_TimeFaultsUnder);
		m_pScoring->SetComputeTimeFaultsOver(m_TimeFaultsOver);
		break;
	case ARBConfigScoring::eTimePlusFaults:
		m_pScoring->SetQsMustBeClean(m_TimeFaultsCleanQ);
		m_pScoring->SetComputeTimeFaultsUnder(m_TimeFaultsUnder);
		m_pScoring->SetComputeTimeFaultsOver(m_TimeFaultsOver);
		m_pScoring->SetTimeFaultMultiplier(m_Multiply);
		break;
	}

	EndDialog(wxID_OK);
}