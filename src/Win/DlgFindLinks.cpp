/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 *
 * @brief implementation of the CDlgFindLinks class
 * @author David Connet
 *
 * Revision History
 * @li 2011-08-31 DRC Column headings were wrong.
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2009-08-08 DRC Fixed data index lookup when editing an item.
 * @li 2009-02-10 DRC Ported to wxWidgets.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2004-06-02 DRC Added 'Open' button.
 * @li 2004-03-31 DRC Created.
 */

#include "stdafx.h"
#include "DlgFindLinks.h"

#include "AgilityBook.h"
#include "ARBDog.h"
#include "ARBDogRun.h"
#include "ARBDogTrial.h"
#include "CheckLink.h"
#include "ClipBoard.h"
#include "DlgSelectURL.h"
#include "ListCtrl.h"
#include "ListData.h"

#include "res/CalPlan.xpm"
#include "res/CalTentative.xpm"

#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>
#endif

/////////////////////////////////////////////////////////////////////////////

class CDlgFindLinksData : public CListData
{
public:
	CDlgFindLinksData(
			ARBDogPtr pDog,
			ARBDogTrialPtr pTrial,
			ARBDogRunPtr pRun,
			wxString const& inLink,
			int image)
		: m_pDog(pDog)
		, m_pTrial(pTrial)
		, m_pRun(pRun)
		, m_OldLink(inLink)
		, m_Link(inLink)
		, m_Image(image)
	{
	}
	virtual wxString OnNeedText(long iCol) const;
	virtual void OnNeedListItem(long iCol, wxListItem& info) const;

	ARBDogPtr m_pDog;
	ARBDogTrialPtr m_pTrial;
	ARBDogRunPtr m_pRun;
	wxString m_OldLink;
	wxString m_Link;
	int m_Image;
};

#define COL_LINK	0
#define COL_DOG		1
#define COL_TRIAL	2
#define COL_RUN		3

wxString CDlgFindLinksData::OnNeedText(long iCol) const
{
	switch (iCol)
	{
	default:
		assert(0);
	case COL_LINK:
		return m_Link;
	case COL_TRIAL:
		return m_pTrial->GetGenericName();
	case COL_DOG:
		return m_pDog->GetGenericName();
	case COL_RUN:
		return m_pRun->GetGenericName();
	}
}


void CDlgFindLinksData::OnNeedListItem(long iCol, wxListItem& info) const
{
	info.SetMask(info.GetMask() | wxLIST_MASK_TEXT);
	info.SetText(OnNeedText(iCol));
	if (0 == iCol && 0 <= m_Image)
	{
		info.SetMask(info.GetMask() | wxLIST_MASK_IMAGE);
		info.SetImage(m_Image);
	}

}

/////////////////////////////////////////////////////////////////////////////

static struct
{
	int col;
	wxChar const* info;
} colLinkInfo[] =
{
	{COL_LINK, wxT("IDS_COL_NAME")},
	{COL_DOG, wxT("IDS_COL_DOG")},
	{COL_TRIAL, wxT("IDS_COL_TRIAL")},
	{COL_RUN, wxT("IDS_COL_EVENT")},
};
static int const nColLinkInfo = sizeof(colLinkInfo) / sizeof(colLinkInfo[0]);


static struct
{
	CDlgFindLinks* pThis;
	CColumnOrder* pCols;
} s_SortInfo;


#if wxCHECK_VERSION(2, 9, 3)
int wxCALLBACK CompareLinks(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
#else
int wxCALLBACK CompareLinks(long item1, long item2, long sortData)
#endif
{
	int rc = 0;
	CDlgFindLinksDataPtr pData1 = s_SortInfo.pThis->GetItemLinkDataByData(item1);
	CDlgFindLinksDataPtr pData2 = s_SortInfo.pThis->GetItemLinkDataByData(item2);

	for (int i = 0; i < s_SortInfo.pCols->GetSize(); ++i)
	{
		int col = s_SortInfo.pCols->GetColumnAt(i);
		wxString str1 = pData1->OnNeedText(col);
		wxString str2 = pData2->OnNeedText(col);
		if (str1 < str2)
			rc = -1;
		else if (str1 > str2)
			rc = 1;
		if (rc)
		{
			if (s_SortInfo.pCols->IsDescending(col))
				rc *= -1;
			break;
		}
	}
	return rc;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgFindLinks dialog

BEGIN_EVENT_TABLE(CDlgFindLinks, wxDialog)
	EVT_BUTTON(wxID_OK, CDlgFindLinks::OnOk)
END_EVENT_TABLE()


CDlgFindLinks::CDlgFindLinks(
		ARBDogList& inDogs,
		wxWindow* pParent)
	: wxDialog()
	, m_ctrlLinks(NULL)
	, m_ctrlEdit(NULL)
	, m_ctrlOpen(NULL)
	, m_sortLinks(wxT("Links"))
	, m_imgEmpty(-1)
	, m_imgOk(-1)
	, m_imgMissing(-1)
{
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP | GetExtraStyle());
	if (!pParent)
		pParent = wxGetApp().GetTopWindow();
	Create(pParent, wxID_ANY, _("IDD_FIND_LINKS"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

	wxBusyCursor wait;
	m_sortLinks.Initialize(nColLinkInfo);

	// Controls (these are done first to control tab order)

	m_ctrlLinks = new CReportListCtrl(this,
		wxDefaultPosition, wxSize(400,150),
		true, CReportListCtrl::eSortHeader, true);
	BIND_OR_CONNECT_CTRL(m_ctrlLinks, wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler, CDlgFindLinks::OnColumnClick);
	BIND_OR_CONNECT_CTRL(m_ctrlLinks, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler, CDlgFindLinks::OnItemSelected);
	BIND_OR_CONNECT_CTRL(m_ctrlLinks, wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler, CDlgFindLinks::OnItemActivated);
	BIND_OR_CONNECT_CTRL(m_ctrlLinks, wxEVT_KEY_DOWN, wxKeyEventHandler, CDlgFindLinks::OnKeyDown);
	m_imgEmpty = m_ctrlLinks->ImageEmpty();
	m_imgOk = m_ctrlLinks->AddIcon(wxIcon(CalPlan_xpm));
	m_imgMissing = m_ctrlLinks->AddIcon(wxIcon(CalTentative_xpm));
	m_ctrlLinks->SetHelpText(_("HIDC_FINDLINKS_LIST"));
	m_ctrlLinks->SetToolTip(_("HIDC_FINDLINKS_LIST"));

	wxButton* btnOk = new wxButton(this, wxID_OK);

	wxButton* btnCancel = new wxButton(this, wxID_CANCEL);

	wxButton* btnCopy = new wxButton(this, wxID_ANY,
		_("IDC_FINDLINKS_COPY"),
		wxDefaultPosition, wxDefaultSize, 0);
	BIND_OR_CONNECT_CTRL(btnCopy, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler, CDlgFindLinks::OnCopy);
	btnCopy->SetHelpText(_("HIDC_FINDLINKS_COPY"));
	btnCopy->SetToolTip(_("HIDC_FINDLINKS_COPY"));

	m_ctrlEdit = new wxButton(this, wxID_ANY,
		_("IDC_FINDLINKS_EDIT"),
		wxDefaultPosition, wxDefaultSize, 0);
	BIND_OR_CONNECT_CTRL(m_ctrlEdit, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler, CDlgFindLinks::OnEdit);
	m_ctrlEdit->SetHelpText(_("HIDC_FINDLINKS_EDIT"));
	m_ctrlEdit->SetToolTip(_("HIDC_FINDLINKS_EDIT"));

	m_ctrlOpen = new wxButton(this, wxID_ANY,
		_("IDC_FINDLINKS_OPEN"),
		wxDefaultPosition, wxDefaultSize, 0);
	BIND_OR_CONNECT_CTRL(m_ctrlOpen, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler, CDlgFindLinks::OnOpen);
	m_ctrlOpen->SetHelpText(_("HIDC_FINDLINKS_OPEN"));
	m_ctrlOpen->SetToolTip(_("HIDC_FINDLINKS_OPEN"));

	// Sizers (sizer creation is in same order as wxFormBuilder)

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizerList = new wxBoxSizer(wxHORIZONTAL);
	bSizerList->Add(m_ctrlLinks, 1, wxALL|wxEXPAND, 5);

	wxBoxSizer* bSizerSide = new wxBoxSizer(wxVERTICAL);
	bSizerSide->Add(btnOk, 0, wxALL, 5);
	bSizerSide->Add(btnCancel, 0, wxALL, 5);
	bSizerSide->Add(0, 0, 1, wxEXPAND, 5);
	bSizerSide->Add(btnCopy, 0, wxALL, 5);
	bSizerSide->Add(m_ctrlEdit, 0, wxALL, 5);
	bSizerSide->Add(m_ctrlOpen, 0, wxALL, 5);

	bSizerList->Add(bSizerSide, 0, wxEXPAND, 5);

	bSizer->Add(bSizerList, 1, wxEXPAND, 5);

	SetSizer(bSizer);
	Layout();
	GetSizer()->Fit(this);
	SetSizeHints(GetSize(), wxDefaultSize);
	CenterOnParent();

	m_ctrlLinks->SetFocus();

	for (int i = 0; i < nColLinkInfo; ++i)
	{
		assert(i == colLinkInfo[i].col);
		m_ctrlLinks->InsertColumn(i, wxGetTranslation(colLinkInfo[i].info));
	}

	for (ARBDogList::iterator iterDog = inDogs.begin();
		iterDog != inDogs.end();
		++iterDog)
	{
		ARBDogPtr pDog = *iterDog;
		for (ARBDogTrialList::iterator iterTrial = pDog->GetTrials().begin();
			iterTrial != pDog->GetTrials().end();
			++iterTrial)
		{
			ARBDogTrialPtr pTrial = *iterTrial;
			for (ARBDogRunList::iterator iterRun = pTrial->GetRuns().begin();
				iterRun != pTrial->GetRuns().end();
				++iterRun)
			{
				ARBDogRunPtr pRun = *iterRun;
				std::set<wxString> links;
				pRun->GetLinks(links);
				for (std::set<wxString>::iterator iter = links.begin();
					iter != links.end();
					++iter)
				{
					int image = GetImageIndex(*iter);
					m_Data.push_back(CDlgFindLinksDataPtr(new CDlgFindLinksData(pDog, pTrial, pRun, *iter, image)));
				}
			}
		}
	}

	for (size_t iData = 0; iData < m_Data.size(); ++iData)
	{
		m_ctrlLinks->InsertItem(m_Data[iData]);
	}
	for (int iCol = 0; iCol < nColLinkInfo; ++iCol)
		m_ctrlLinks->SetColumnWidth(iCol, wxLIST_AUTOSIZE_USEHEADER);
	s_SortInfo.pThis = this;
	s_SortInfo.pCols = &m_sortLinks;
	m_ctrlLinks->SortItems(CompareLinks, 0);
	SetColumnHeaders();

	UpdateButtons();
}


CDlgFindLinksDataPtr CDlgFindLinks::GetItemLinkData(long item)
{
	return std::tr1::dynamic_pointer_cast<CDlgFindLinksData, CListData>(m_ctrlLinks->GetData(item));
}


CDlgFindLinksDataPtr CDlgFindLinks::GetItemLinkDataByData(long data)
{
	return std::tr1::dynamic_pointer_cast<CDlgFindLinksData, CListData>(m_ctrlLinks->GetDataByData(data));
}


int CDlgFindLinks::GetImageIndex(wxString const& inLink)
{
	wxBusyCursor wait;
	int img = m_imgEmpty;
	if (0 < inLink.length())
	{
		if (CheckLink(inLink, this))
			img = m_imgOk;
		else
			img = m_imgMissing;
	}
	return img;
}


void CDlgFindLinks::SetColumnHeaders()
{
	for (int i = 0; i < nColLinkInfo; ++i)
	{
		wxString str;
		str << wxGetTranslation(colLinkInfo[i].info)
			<< wxT(" (") << m_sortLinks.FindColumnOrder(i) + 1 << wxT(")");
		wxListItem item;
		item.SetMask(wxLIST_MASK_TEXT);
		item.SetText(str);
		m_ctrlLinks->SetColumn(i, item);
		m_ctrlLinks->SetColumnSort(i, m_sortLinks.IsDescending(i) ? -1 : 1);
	}
}


void CDlgFindLinks::UpdateButtons()
{
	bool bEnable = false;
	if (0 <= m_ctrlLinks->GetFirstSelected())
		bEnable = true;
	m_ctrlEdit->Enable(bEnable);
}


void CDlgFindLinks::Edit()
{
	int nItem = m_ctrlLinks->GetFirstSelected();
	if (0 <= nItem)
	{
		CDlgFindLinksDataPtr data = GetItemLinkData(nItem);
		CDlgSelectURL dlg(data->m_Link, this);
		if (wxID_OK == dlg.ShowModal())
		{
			wxString newName = dlg.GetName();
			if (data->m_Link != newName)
			{
				data->m_Link = newName;
				data->m_Image = GetImageIndex(newName);
				m_ctrlLinks->RefreshItem(nItem);
			}
		}
	}
}


void CDlgFindLinks::OnColumnClick(wxListEvent& evt)
{
	s_SortInfo.pThis = this;
	m_sortLinks.SetColumnOrder(evt.GetColumn());
	SetColumnHeaders();
	s_SortInfo.pCols = &m_sortLinks;
	m_ctrlLinks->SortItems(CompareLinks, 0);
	m_sortLinks.Save();
}


void CDlgFindLinks::OnItemSelected(wxListEvent& evt)
{
	UpdateButtons();
}


void CDlgFindLinks::OnItemActivated(wxListEvent& evt)
{
	Edit();
}


void CDlgFindLinks::OnKeyDown(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode())
	{
	default:
		evt.Skip();
		break;
	case WXK_SPACE:
	case WXK_NUMPAD_SPACE:
		Edit();
		break;
	}
}


void CDlgFindLinks::OnCopy(wxCommandEvent& evt)
{
	if (0 < m_Data.size())
	{
		CClipboardDataWriter clpData;
		if (!clpData.isOkay())
			return;

		wxString data;
		for (size_t i = 0; i < m_Data.size(); ++i)
		{
			data += m_Data[i]->m_OldLink;
			data += wxT("\r\n");
		}

		clpData.AddData(data);
		clpData.CommitData();
	}
}


void CDlgFindLinks::OnEdit(wxCommandEvent& evt)
{
	Edit();
}


void CDlgFindLinks::OnOpen(wxCommandEvent& evt)
{
	int nItem = m_ctrlLinks->GetFirstSelected();
	if (0 <= nItem)
	{
		CDlgFindLinksDataPtr data = GetItemLinkData(nItem);
		if (data)
			wxLaunchDefaultBrowser(data->m_Link);
	}
}


void CDlgFindLinks::OnOk(wxCommandEvent& evt)
{
	if (!Validate() || !TransferDataFromWindow())
		return;
	for (size_t idx = 0; idx < m_Data.size(); ++idx)
	{
		if (m_Data[idx]->m_Link != m_Data[idx]->m_OldLink)
		{
			m_Data[idx]->m_pRun->RemoveLink(m_Data[idx]->m_OldLink);
			if (0 < m_Data[idx]->m_Link.length())
				m_Data[idx]->m_pRun->AddLink(m_Data[idx]->m_Link);
		}
	}
	EndDialog(wxID_OK);
}
