/*
 * Copyright � 2003 David Connet. All Rights Reserved.
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
 * @brief Export Wizard
 * @author David Connet
 *
 * Revision History
 * @li 2003-12-10 DRC Created
 */

#include "stdafx.h"
#include "AgilityBook.h"
#include "WizardExport.h"
#include <fstream>

#include "AgilityBookDoc.h"
#include "AgilityBookOptions.h"
#include "AgilityBookViewCalendarList.h"
#include "AgilityBookViewTraining.h"
#include "ARBConfigScoring.h"
#include "ARBDog.h"
#include "ARBDogRun.h"
#include "ARBDogTrial.h"
#include "DlgAssignColumns.h"
#include "Wizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardExport property page

IMPLEMENT_DYNAMIC(CWizardExport, CPropertyPage)

CWizardExport::CWizardExport(CWizard* pSheet, CAgilityBookDoc* pDoc)
	: CPropertyPage(CWizardExport::IDD)
	, m_pSheet(pSheet)
	, m_pDoc(pDoc)
{
	//{{AFX_DATA_INIT(CWizardExport)
	m_Delim = -1;
	m_Delimiter = _T(":");
	//}}AFX_DATA_INIT
	int delim;
	CAgilityBookOptions::GetImportExportDelimiters(false, delim, m_Delimiter);
	switch (delim)
	{
	default:
	case CAgilityBookOptions::eDelimTab:		m_Delim = 0; break;
	case CAgilityBookOptions::eDelimSpace:		m_Delim = 1; break;
	case CAgilityBookOptions::eDelimColon:		m_Delim = 2; break;
	case CAgilityBookOptions::eDelimSemicolon:	m_Delim = 3; break;
	case CAgilityBookOptions::eDelimComma:		m_Delim = 4; break;
	case CAgilityBookOptions::eDelimOther:		m_Delim = 5; break;
	}
}

CWizardExport::~CWizardExport()
{
}

void CWizardExport::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardExport)
	DDX_Radio(pDX, IDC_WIZARD_DELIM_TAB, m_Delim);
	DDX_Text(pDX, IDC_WIZARD_DELIM, m_Delimiter);
	DDV_MaxChars(pDX, m_Delimiter, 1);
	DDX_Control(pDX, IDC_WIZARD_ASSIGN, m_ctrlAssign);
	DDX_Control(pDX, IDC_WIZARD_PREVIEW, m_ctrlPreview);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWizardExport, CPropertyPage)
	//{{AFX_MSG_MAP(CWizardExport)
	ON_BN_CLICKED(IDC_WIZARD_ASSIGN, OnExportAssign)
	ON_BN_CLICKED(IDC_WIZARD_DELIM_TAB, OnExportDelim)
	ON_BN_CLICKED(IDC_WIZARD_DELIM_SPACE, OnExportDelim)
	ON_BN_CLICKED(IDC_WIZARD_DELIM_COLON, OnExportDelim)
	ON_BN_CLICKED(IDC_WIZARD_DELIM_SEMI, OnExportDelim)
	ON_BN_CLICKED(IDC_WIZARD_DELIM_COMMA, OnExportDelim)
	ON_BN_CLICKED(IDC_WIZARD_DELIM_OTHER, OnExportDelim)
	ON_EN_CHANGE(IDC_WIZARD_DELIM, OnExportDelim)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CString CWizardExport::GetDelim() const
{
	CString delim;
	switch (m_Delim)
	{
	default: break;
	case 0: delim = "\t"; break;
	case 1: delim = " "; break;
	case 2: delim = ":"; break;
	case 3: delim = ";"; break;
	case 4: delim = ","; break;
	case 5: delim = m_Delimiter; break;
	}
	return delim;
}

void CWizardExport::UpdateButtons()
{
	DWORD dwWiz = PSWIZB_BACK;
	// Some test to make sure things are ready
	bool bOk = false;
	if (1 == GetDelim().GetLength())
		bOk = true;
	BOOL bEnable = FALSE;
	CAgilityBookOptions::ColumnOrder order = CAgilityBookOptions::eUnknown;
	switch (m_pSheet->GetImportExportItem())
	{
	default:
		break;
	case WIZ_EXPORT_RUNS:
		order = CAgilityBookOptions::eRunsExport;
		break;
	case WIZ_EXPORT_CALENDAR:
		order = CAgilityBookOptions::eCalExport;
		break;
	case WIZ_EXPORT_LOG:
		order = CAgilityBookOptions::eLogExport;
		break;
	}
	if (CAgilityBookOptions::eUnknown != order)
	{
		bEnable = TRUE;
		if (bOk)
		{
			for (int i = 0; bOk && i < IO_TYPE_MAX; ++i)
			{
				std::vector<int> columns;
				if (CDlgAssignColumns::GetColumnOrder(order, i, columns))
				{
					if (0 == columns.size())
						bOk = false;
				}
			}
		}
	}
	m_ctrlAssign.EnableWindow(bEnable);
	dwWiz |= (bOk ? PSWIZB_FINISH : PSWIZB_DISABLEDFINISH);
	m_pSheet->SetWizardButtons(dwWiz);
}

void CWizardExport::UpdatePreview()
{
	m_ctrlPreview.ResetContent();
	CString delim = GetDelim();
	if (delim.IsEmpty())
		m_ctrlPreview.AddString("No delimiter specified. Unable to preview data.");
	else switch (m_pSheet->GetImportExportItem())
	{
	default:
		break;

	case WIZ_EXPORT_RUNS:
		{
			int i;
			std::vector<int> columns[IO_TYPE_MAX];
			for (i = 0; i < IO_TYPE_MAX; ++i)
				CDlgAssignColumns::GetColumnOrder(CAgilityBookOptions::eRunsExport, i, columns[i]);
			for (i = 0; i < IO_TYPE_MAX; ++i)
			{
				if (0 == columns[i].size())
					continue;
				CString data;
				for (size_t idx = 0; idx < columns[i].size(); ++idx)
				{
					if (0 < idx)
						data += delim;
					data += CDlgAssignColumns::GetNameFromColumnID(columns[i][idx]);
				}
				m_ctrlPreview.AddString(data);
			}
			for (ARBDogList::const_iterator iterDog = m_pDoc->GetDogs().begin(); iterDog != m_pDoc->GetDogs().end(); ++iterDog)
			{
				const ARBDog* pDog = *iterDog;
				for (ARBDogTrialList::const_iterator iterTrial = pDog->GetTrials().begin(); iterTrial != pDog->GetTrials().end(); ++iterTrial)
				{
					const ARBDogTrial* pTrial = *iterTrial;
					for (ARBDogRunList::const_iterator iterRun = pTrial->GetRuns().begin(); iterRun != pTrial->GetRuns().end(); ++iterRun)
					{
						const ARBDogRun* pRun = *iterRun;
						const ARBConfigScoring* pScoring = m_pDoc->GetConfig().GetVenues().FindEvent(
							pTrial->GetClubs().GetPrimaryClub()->GetVenue(),
							pRun->GetEvent(),
							pRun->GetDivision(),
							pRun->GetLevel(),
							pRun->GetDate());
						ASSERT(pScoring);
						if (pScoring)
						{
							int idxType = -1;
							switch (pScoring->GetScoringStyle())
							{
							case ARBConfigScoring::eFaultsThenTime:
							case ARBConfigScoring::eFaults100ThenTime:
							case ARBConfigScoring::eFaults200ThenTime:
								idxType = IO_TYPE_RUNS_FAULTS_TIME;
								break;
							case ARBConfigScoring::eOCScoreThenTime:
								idxType = IO_TYPE_RUNS_OPEN_CLOSE;
								break;
							case ARBConfigScoring::eScoreThenTime:
								idxType = IO_TYPE_RUNS_POINTS;
								break;
							case ARBConfigScoring::eTimePlusFaults:
								idxType = IO_TYPE_RUNS_TIME_FAULTS;
								break;
							}
							ASSERT(-1 != idxType);
							if (0 <= idxType)
							{
								CString data;
								for (size_t idx = 0; idx < columns[idxType].size(); ++idx)
								{
									if (0 < idx)
										data += delim;
									switch (columns[idxType][idx])
									{
									case IO_RUNS_REG_NAME:
										data += pDog->GetRegisteredName().c_str();
										break;
									case IO_RUNS_CALL_NAME:
										data += pDog->GetCallName().c_str();
										break;
									case IO_RUNS_DATE:
										data += pRun->GetDate().GetString(false, true).c_str();
										break;
									case IO_RUNS_VENUE:
										{
											int i = 0;
											for (ARBDogClubList::const_iterator iter = pTrial->GetClubs().begin();
												iter != pTrial->GetClubs().end();
												++iter, ++i)
											{
												if (0 < i)
													data += "/";
												data += (*iter)->GetVenue().c_str();
											}
										}
										break;
									case IO_RUNS_CLUB:
										{
											int i = 0;
											for (ARBDogClubList::const_iterator iter = pTrial->GetClubs().begin();
												iter != pTrial->GetClubs().end();
												++iter, ++i)
											{
												if (0 < i)
													data += "/";
												data += (*iter)->GetName().c_str();
											}
										}
										break;
									case IO_RUNS_LOCATION:
										data += pTrial->GetLocation().c_str();
										break;
									case IO_RUNS_TRIAL_NOTES:
										data += pTrial->GetNote().c_str();
										break;
									case IO_RUNS_DIVISION:
										data += pRun->GetDivision().c_str();
										break;
									case IO_RUNS_LEVEL:
										data += pRun->GetLevel().c_str();
										break;
									case IO_RUNS_EVENT:
										data += pRun->GetEvent().c_str();
										break;
									case IO_RUNS_HEIGHT:
										data += pRun->GetHeight().c_str();
										break;
									case IO_RUNS_JUDGE:
										data += pRun->GetJudge().c_str();
										break;
									case IO_RUNS_HANDLER:
										data += pRun->GetHandler().c_str();
										break;
									case IO_RUNS_CONDITIONS:
										data += pRun->GetConditions().c_str();
										break;
									case IO_RUNS_COURSE_FAULTS:
										{
											CString str;
											str.Format("%hd", pRun->GetScoring().GetCourseFaults());
											data += str;
										}
										break;
									case IO_RUNS_TIME:
										data += pRun->GetScoring().GetTime().str().c_str();
										break;
									case IO_RUNS_YARDS:
										{
											CString str;
											str.Format("%.3f", pRun->GetScoring().GetYards());
											data += str;
										}
										break;
									case IO_RUNS_YPS:
										{
											if (ARBDogRunScoring::eTypeByTime == pRun->GetScoring().GetType()
											&& 0 < pRun->GetScoring().GetYards() && 0.0 < pRun->GetScoring().GetTime())
											{
												CString str;
												str.Format("%.3f", pRun->GetScoring().GetYards() / pRun->GetScoring().GetTime());
												data += str;
											}
										}
										break;
									case IO_RUNS_SCT:
										data += pRun->GetScoring().GetSCT().str().c_str();
										break;
									case IO_RUNS_TOTAL_FAULTS:
										{
											if (ARBDogRunScoring::eTypeByTime == pRun->GetScoring().GetType())
											{
												CString str;
												double faults = pRun->GetScoring().GetCourseFaults() + pRun->GetScoring().GetTimeFaults();
												str.Format("%.3f", faults);
												data += str;
											}
										}
										break;
									case IO_RUNS_REQ_OPENING:
										{
											CString str;
											str.Format("%hd", pRun->GetScoring().GetNeedOpenPts());
											data += str;
										}
										break;
									case IO_RUNS_REQ_CLOSING:
										{
											CString str;
											str.Format("%hd", pRun->GetScoring().GetNeedClosePts());
											data += str;
										}
										break;
									case IO_RUNS_OPENING:
										{
											CString str;
											str.Format("%hd", pRun->GetScoring().GetOpenPts());
											data += str;
										}
										break;
									case IO_RUNS_CLOSING:
										{
											CString str;
											str.Format("%hd", pRun->GetScoring().GetClosePts());
											data += str;
										}
										break;
									case IO_RUNS_REQ_POINTS:
										{
											CString str;
											str.Format("%hd", pRun->GetScoring().GetNeedOpenPts());
											data += str;
										}
										break;
									case IO_RUNS_POINTS:
										{
											CString str;
											str.Format("%hd", pRun->GetScoring().GetOpenPts());
											data += str;
										}
										break;
									case IO_RUNS_PLACE:
										{
											CString str;
											short place = pRun->GetPlace();
											if (0 > place)
												str = "?";
											else if (0 == place)
												str = "-";
											else
												str.Format("%hd", place);
											data += str;
										}
										break;
									case IO_RUNS_IN_CLASS:
										{
											CString str;
											short inClass = pRun->GetInClass();
											if (0 >= inClass)
												str = "?";
											else
												str.Format("%hd", inClass);
											data += str;
										}
										break;
									case IO_RUNS_DOGSQD:
										{
											CString str;
											short qd = pRun->GetDogsQd();
											if (0 > qd)
												str = "?";
											else
												str.Format("%hd", qd);
											data += str;
										}
										break;
									case IO_RUNS_Q:
										{
											CString str = pRun->GetQ().str().c_str();
											if (pRun->GetQ().Qualified())
											{
												if (pTrial->HasQQ(
													pRun->GetDate(),
													m_pDoc->GetConfig(),
													pRun->GetDivision(),
													pRun->GetLevel()))
												{
													str.LoadString(IDS_QQ);
												}
												if (ARB_Q::eQ_SuperQ == pRun->GetQ())
													str.LoadString(IDS_SQ);
											}
											data += str;
										}
										break;
									case IO_RUNS_SCORE:
										if (pRun->GetQ().Qualified()
										|| ARB_Q::eQ_NQ == pRun->GetQ())
										{
											data += pRun->GetScore(pScoring).str().c_str();
										}
										break;
									case IO_RUNS_TITLE_POINTS:
										{
											CString str;
											short pts = 0;
											if (pRun->GetQ().Qualified())
												pts = pRun->GetTitlePoints(pScoring);
											str.Format("%hd", pts);
											data += str;
										}
										break;
									case IO_RUNS_COMMENTS:
										data += pRun->GetNote().c_str();
										break;
									case IO_RUNS_FAULTS:
										{
											int i = 0;
											for (ARBDogFaultList::const_iterator iter = pRun->GetFaults().begin();
												iter != pRun->GetFaults().end();
												++iter)
											{
												if (0 < i)
													data += "/";
												data += (*iter).c_str();
											}
										}
										break;
									}
								}
								data.Replace("\n", " ");
								m_ctrlPreview.AddString(data);
							}
						}
					}
				}
			}
		}
		break;

	case WIZ_EXPORT_CALENDAR:
		{
			std::vector<int> columns;
			CDlgAssignColumns::GetColumnOrder(CAgilityBookOptions::eCalExport, IO_TYPE_CALENDAR, columns);
			CString data;
			for (size_t idx = 0; idx < columns.size(); ++idx)
			{
				if (0 < idx)
					data += delim;
				data += CDlgAssignColumns::GetNameFromColumnID(columns[idx]);
			}
			m_ctrlPreview.AddString(data);
			for (ARBCalendarList::const_iterator iterCal = m_pDoc->GetCalendar().begin(); iterCal != m_pDoc->GetCalendar().end(); ++iterCal)
			{
				CString data;
				const ARBCalendar* pCal = *iterCal;
				for (size_t idx = 0; idx < columns.size(); ++idx)
				{
					ARBDate date;
					if (0 < idx)
						data += delim;
					switch (columns[idx])
					{
					case IO_CAL_START_DATE:
						data += pCal->GetStartDate().GetString(false, true).c_str();
						break;
					case IO_CAL_END_DATE:
						data += pCal->GetEndDate().GetString(false, true).c_str();
						break;
					case IO_CAL_TENTATIVE:
						if (pCal->IsTentative())
							data += "?";
						break;
					case IO_CAL_ENTERED:
						switch (pCal->GetEntered())
						{
						default:
						case ARBCalendar::eNot:
							break;
						case ARBCalendar::eEntered:
							data += "Entered";
							break;
						case ARBCalendar::ePlanning:
							data += "Planning";
							break;
						}
						break;
					case IO_CAL_LOCATION:
						data += pCal->GetLocation().c_str();
						break;
					case IO_CAL_CLUB:
						data += pCal->GetClub().c_str();
						break;
					case IO_CAL_VENUE:
						data += pCal->GetVenue().c_str();
						break;
					case IO_CAL_OPENS:
						date = pCal->GetOpeningDate();
						if (date.IsValid())
							data += date.GetString(false, true).c_str();
						break;
					case IO_CAL_CLOSES:
						date = pCal->GetClosingDate();
						if (date.IsValid())
							data += date.GetString(false, true).c_str();
						break;
					case IO_CAL_NOTES:
						data += pCal->GetNote().c_str();
						break;
					}
				}
				data.Replace("\n", " ");
				m_ctrlPreview.AddString(data);
			}
		}
		break;

	case WIZ_EXPORT_LOG:
		{
			std::vector<int> columns;
			CDlgAssignColumns::GetColumnOrder(CAgilityBookOptions::eLogExport, IO_TYPE_TRAINING, columns);
			CString data;
			for (size_t idx = 0; idx < columns.size(); ++idx)
			{
				if (0 < idx)
					data += delim;
				data += CDlgAssignColumns::GetNameFromColumnID(columns[idx]);
			}
			m_ctrlPreview.AddString(data);
			for (ARBTrainingList::const_iterator iterLog = m_pDoc->GetTraining().begin(); iterLog != m_pDoc->GetTraining().end(); ++iterLog)
			{
				const ARBTraining* pLog = *iterLog;
				for (size_t idx = 0; idx < columns.size(); ++idx)
				{
					if (0 < idx)
						data += delim;
					switch (columns[idx])
					{
					case IO_LOG_DATE:
						data += pLog->GetDate().GetString(false, true).c_str();
						break;
					case IO_LOG_NAME:
						data += pLog->GetName().c_str();
						break;
					case IO_LOG_NOTES:
						data += pLog->GetNote().c_str();
						break;
					}
				}
				data.Replace("\n", " ");
				m_ctrlPreview.AddString(data);
			}
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWizardExport message handlers

BOOL CWizardExport::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CWizardExport::OnSetActive() 
{
	UpdateButtons();
	UpdatePreview();
	return CPropertyPage::OnSetActive();
}

LRESULT CWizardExport::OnWizardBack()
{
	return IDD_WIZARD_START;
}

BOOL CWizardExport::OnWizardFinish() 
{
	UpdateData(TRUE);
	UpdatePreview();
	int delim;
	switch (m_Delim)
	{
	default:
	case 0: delim = CAgilityBookOptions::eDelimTab; break;
	case 1: delim = CAgilityBookOptions::eDelimSpace; break;
	case 2: delim = CAgilityBookOptions::eDelimColon; break;
	case 3: delim = CAgilityBookOptions::eDelimSemicolon; break;
	case 4: delim = CAgilityBookOptions::eDelimComma; break;
	case 5: delim = CAgilityBookOptions::eDelimOther; break;
	}
	CAgilityBookOptions::SetImportExportDelimiters(false, delim, m_Delimiter);

	CString filter;
	filter.LoadString(IDS_FILEEXT_FILTER_TXT);
	CFileDialog file(FALSE, "", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST, filter, this);
	if (IDOK == file.DoModal())
	{
		std::ofstream output(file.GetFileName(), std::ios::out);
		output.exceptions(std::ios_base::badbit);
		if (output.is_open())
		{
			for (int i = 0; i < m_ctrlPreview.GetCount(); ++i)
			{
				CString line;
				m_ctrlPreview.GetText(i, line);
				output << (LPCTSTR)line << std::endl;
			}
			output.close();
		}
		return CPropertyPage::OnWizardFinish();
	}
	else
		return FALSE;
}

void CWizardExport::OnExportDelim() 
{
	UpdateData(TRUE);
	UpdateButtons();
	UpdatePreview();
}

void CWizardExport::OnExportAssign() 
{
	UpdateData(TRUE);
	CAgilityBookOptions::ColumnOrder order = CAgilityBookOptions::eUnknown;
	switch (m_pSheet->GetImportExportItem())
	{
	default: break;
	case WIZ_EXPORT_RUNS: order = CAgilityBookOptions::eRunsExport; break;
	case WIZ_EXPORT_CALENDAR: order = CAgilityBookOptions::eCalExport; break;
	case WIZ_EXPORT_LOG: order = CAgilityBookOptions::eLogExport; break;
	}
	if (CAgilityBookOptions::eUnknown != order)
	{
		CDlgAssignColumns dlg(order, this);
		if (IDOK == dlg.DoModal())
		{
			UpdateButtons();
			UpdatePreview();
		}
	}
}
