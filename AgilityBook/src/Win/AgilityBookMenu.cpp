/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief Wrapper for menus to ease i18n and menu ids
 * @author David Connet
 *
 * Important: For accelerators to work, they must be in the main menu.
 * If they are only in a context menu, the accelerator will not work.
 *
 * Revision History
 * 2012-07-28 Fixed extra separator on Mac, removed Update menu icon.
 * 2012-07-13 Reworked menu creation to use a generic reusable helper.
 * 2012-07-08 Put images into menu.
 * 2012-07-04 Add option to use run time or opening time in gamble OPS.
 * 2009-09-19 Fix IdMenuRecentFiles on language switch.
 * 2009-09-13 Add support for wxWidgets 2.9, deprecate tstring.
 * 2009-09-09 Fix Ctrl+R. See note above.
 * 2008-12-14 Created
 */

#include "stdafx.h"
#include "AgilityBookMenu.h"

#include "ImageHelper.h"
#include "MainFrm.h"
#include <wx/artprov.h>
#include <wx/config.h>

#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>
#endif


static const CMenuHelper::ItemData sc_Items[] =
{
	{MENU_ITEM, 0,                        wxITEM_NORMAL, 0, nullptr, arbT("MenuFile"), nullptr, nullptr},
	{MENU_ITEM, wxID_NEW,                 wxITEM_NORMAL, 1, arbT("MenuFileNew"), arbT("MenuFileNew"), arbT("DescFileNew"), ImageMgrNew},
	{MENU_ITEM, wxID_OPEN,                wxITEM_NORMAL, 1, arbT("MenuFileOpen"), arbT("MenuFileOpen"), arbT("DescFileOpen"), ImageMgrOpen},
	{MENU_ITEM, wxID_SAVE,                wxITEM_NORMAL, 1, arbT("MenuFileOpen"), arbT("MenuFileSave"), arbT("DescFileSave"), ImageMgrSave},
	{MENU_ITEM, wxID_SAVEAS,              wxITEM_NORMAL, 1, nullptr, arbT("MenuFileSaveAs"), arbT("DescFileSaveAs"), wxART_FILE_SAVE_AS},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_FILE_LANGUAGE_CHOOSE,  wxITEM_NORMAL, 1, nullptr, arbT("MenuFileLanguage"), arbT("DescFileLanguage"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_FILE_EXPORT_WIZARD,    wxITEM_NORMAL, 1, nullptr, arbT("MenuFileImportExport"), arbT("DescFileImportExport"), nullptr},
	{MENU_ITEM, ID_FILE_LINKED,           wxITEM_NORMAL, 1, nullptr, arbT("MenuFileLinkedFiles"), arbT("DescFileLinkedFiles"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, wxID_PRINT,               wxITEM_NORMAL, 1, arbT("MenuFilePrint"), arbT("MenuFilePrint"), arbT("DescFilePrint"), ImageMgrPrint},
	{MENU_ITEM, ID_FILE_PRINT_BLANK_RUNS, wxITEM_NORMAL, 1, nullptr, arbT("MenuFilePrintBlank"), arbT("DescFilePrintBlank"), nullptr},
	{MENU_ITEM, wxID_PREVIEW,             wxITEM_NORMAL, 1, arbT("MenuFilePrintPreview"), arbT("MenuFilePrintPreview"), arbT("DescFilePrintPreview"), ImageMgrPreview},
	{MENU_MRU,  0,                        wxITEM_NORMAL, 1, nullptr, arbT("MenuFileRecent"), nullptr, nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_FILE_PROPERTIES,       wxITEM_NORMAL, 1, nullptr, arbT("MenuFileProperties"), arbT("DescFileProperties"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, wxID_EXIT,                wxITEM_NORMAL, 1, nullptr, arbT("MenuFileExit"), arbT("DescFileExit"), nullptr},

	{MENU_ITEM, 0,                        wxITEM_NORMAL, 0, nullptr, arbT("MenuEdit"), nullptr, nullptr},
	{MENU_ITEM, wxID_DUPLICATE,           wxITEM_NORMAL, 1, nullptr, arbT("MenuEditDuplicate"), arbT("DescEditDuplicate"), nullptr},
	{MENU_ITEM, wxID_CUT,                 wxITEM_NORMAL, 1, arbT("MenuEditCut"), arbT("MenuEditCut"), arbT("DescEditCut"), ImageMgrCut},
	{MENU_ITEM, wxID_COPY,                wxITEM_NORMAL, 1, arbT("MenuEditCopy"), arbT("MenuEditCopy"), arbT("DescEditCopy"), ImageMgrCopy},
	{MENU_ITEM, wxID_PASTE,               wxITEM_NORMAL, 1, arbT("MenuEditPaste"), arbT("MenuEditPaste"), arbT("DescEditPaste"), ImageMgrPaste},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, wxID_SELECTALL,           wxITEM_NORMAL, 1, nullptr, arbT("MenuEditSelectAll"), arbT("DescEditSelectAll"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_COPY_TITLES_LIST,      wxITEM_NORMAL, 1, nullptr, arbT("MenuEditCopyTitles"), arbT("DescEditCopyTitles"), nullptr},
	{MENU_ITEM, ID_REORDER,               wxITEM_NORMAL, 1, nullptr, arbT("MenuEditReorder"), arbT("DescEditReorder"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, wxID_FIND,                wxITEM_NORMAL, 1, nullptr, arbT("MenuEditFind"), arbT("DescEditFind"), nullptr},
	{MENU_ITEM, ID_EDIT_FIND_NEXT,        wxITEM_NORMAL, 1, nullptr, arbT("MenuEditFindNext"), arbT("DescEditFindNext"), nullptr},
	{MENU_ITEM, ID_EDIT_FIND_PREVIOUS,    wxITEM_NORMAL, 1, nullptr, arbT("MenuEditFindPrev"), arbT("DescEditFindPrev"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_EDIT_CONFIGURATION,    wxITEM_NORMAL, 1, nullptr, arbT("MenuEditConfig"), arbT("DescEditConfig"), nullptr},

	{MENU_ITEM, 0,                               wxITEM_NORMAL, 0, nullptr, arbT("MenuAgility"), nullptr, nullptr},
	{MENU_ITEM, 0,                               wxITEM_NORMAL, 1, nullptr, arbT("MenuDog"), nullptr, ImageMgrDog},
	{MENU_ITEM, ID_AGILITY_EDIT_DOG,             wxITEM_NORMAL, 2, nullptr, arbT("MenuDogProperties"), arbT("DescDogProperties"), ImageMgrDog},
	{MENU_SEP,  0,                               wxITEM_NORMAL, 2, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_AGILITY_NEW_DOG,              wxITEM_NORMAL, 2, arbT("MenuNew"), arbT("MenuNew"), arbT("DescDogProperties"), ImageMgrDog},
	{MENU_ITEM, ID_AGILITY_DELETE_DOG,           wxITEM_NORMAL, 2, nullptr, arbT("MenuDogDelete"), arbT("DescDogDelete"), nullptr},
	{MENU_ITEM, 0,                               wxITEM_NORMAL, 1, nullptr, arbT("MenuTitle"), nullptr, ImageMgrTitle},
	{MENU_ITEM, ID_AGILITY_EDIT_TITLE,           wxITEM_NORMAL, 2, nullptr, arbT("MenuTitleProperties"), arbT("DescTitleProperties"), ImageMgrTitle},
	{MENU_SEP,  0,                               wxITEM_NORMAL, 2, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_AGILITY_NEW_TITLE,            wxITEM_NORMAL, 2, arbT("MenuNew"), arbT("MenuNew"), arbT("DescTitleNew"), ImageMgrTitle},
	{MENU_ITEM, ID_AGILITY_DELETE_TITLE,         wxITEM_NORMAL, 2, nullptr, arbT("MenuTitleDelete"), arbT("DescTitleDelete"), nullptr},
	{MENU_ITEM, 0,                               wxITEM_NORMAL, 1, nullptr, arbT("MenuTrial"), nullptr, ImageMgrTrial},
	{MENU_ITEM, ID_AGILITY_EDIT_TRIAL,           wxITEM_NORMAL, 2, nullptr, arbT("MenuTrialProperties"), arbT("DescTrialProperties"), ImageMgrTrial},
	{MENU_SEP,  0,                               wxITEM_NORMAL, 2, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_AGILITY_NEW_TRIAL,            wxITEM_NORMAL, 2, arbT("MenuNew"), arbT("MenuNew"), arbT("DescTrialNew"), ImageMgrTrial},
	{MENU_ITEM, ID_AGILITY_DELETE_TRIAL,         wxITEM_NORMAL, 2, nullptr, arbT("MenuTrialDelete"), arbT("DescTrialDelete"), nullptr},
	{MENU_SEP,  0,                               wxITEM_NORMAL, 2, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_AGILITY_PRINT_TRIAL,          wxITEM_NORMAL, 2, nullptr, arbT("MenuTrialPrintRuns"), arbT("DescTrialPrintRuns"), nullptr},
	{MENU_ITEM, 0,                               wxITEM_NORMAL, 1, nullptr, arbT("MenuRun"), nullptr, ImageMgrRuns},
	{MENU_ITEM, ID_AGILITY_EDIT_RUN,             wxITEM_NORMAL, 2, nullptr, arbT("MenuRunProperties"), arbT("DescRunProperties"), ImageMgrRuns},
	{MENU_SEP,  0,                               wxITEM_NORMAL, 2, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_AGILITY_NEW_RUN,              wxITEM_NORMAL, 2, arbT("MenuNewRun"), arbT("MenuNewRun"), arbT("DescRunNew"), ImageMgrRuns},
	{MENU_ITEM, ID_AGILITY_DELETE_RUN,           wxITEM_NORMAL, 2, nullptr, arbT("MenuRunDelete"), arbT("DescRunDelete"), nullptr},
	{MENU_SEP,  0,                               wxITEM_NORMAL, 2, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_AGILITY_PRINT_RUNS,           wxITEM_NORMAL, 2, nullptr, arbT("MenuRunPrintRuns"), arbT("DescRunPrintRuns"), nullptr},
	{MENU_ITEM, 0,                               wxITEM_NORMAL, 1, nullptr, arbT("MenuCalendar"), nullptr, ImageMgrCalendar},
	{MENU_ITEM, ID_AGILITY_EDIT_CALENDAR,        wxITEM_NORMAL, 2, nullptr, arbT("MenuCalendarProperties"), arbT("DescCalendarProperties"), ImageMgrCalendar},
	{MENU_SEP,  0,                               wxITEM_NORMAL, 2, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_AGILITY_NEW_CALENDAR,         wxITEM_NORMAL, 2, arbT("MenuNew"), arbT("MenuNew"), arbT("DescCalendarNew"), ImageMgrCalendar},
	{MENU_ITEM, ID_AGILITY_DELETE_CALENDAR,      wxITEM_NORMAL, 2, nullptr, arbT("MenuCalendarDelete"), arbT("DescCalendarDelete"), nullptr},
	{MENU_SEP,  0,                               wxITEM_NORMAL, 2, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_AGILITY_UPDATE_CALENDAR,      wxITEM_NORMAL, 2, nullptr, arbT("MenuCalendarUpdate"), arbT("DescCalendarUpdate"), nullptr},
	{MENU_ITEM, ID_AGILITY_EXPORT_CALENDAR,      wxITEM_NORMAL, 2, nullptr, arbT("MenuCalendarExport"), arbT("DescCalendarExport"), nullptr},
	{MENU_SEP,  0,                               wxITEM_NORMAL, 2, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_AGILITY_CREATEENTRY_CALENDAR, wxITEM_NORMAL, 2, nullptr, arbT("MenuCalendarTrialEntry"), arbT("DescCalendarTrialEntry"), nullptr},
	{MENU_ITEM, 0,                               wxITEM_NORMAL, 1, nullptr, arbT("MenuTraining"), nullptr, ImageMgrTraining},
	{MENU_ITEM, ID_AGILITY_EDIT_TRAINING,        wxITEM_NORMAL, 2, nullptr, arbT("MenuTrainingProperties"), arbT("DescTrainingProperties"), ImageMgrTraining},
	{MENU_SEP,  0,                               wxITEM_NORMAL, 2, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_AGILITY_NEW_TRAINING,         wxITEM_NORMAL, 2, arbT("MenuNew"), arbT("MenuNew"), arbT("DescTrainingNew"), ImageMgrTraining},
	{MENU_ITEM, ID_AGILITY_DELETE_TRAINING,      wxITEM_NORMAL, 2, nullptr, arbT("MenuTrainingDelete"), arbT("DescTrainingDelete"), nullptr},

	{MENU_ITEM, 0,                        wxITEM_NORMAL, 0, nullptr, arbT("MenuNotes"), nullptr, nullptr},
	{MENU_ITEM, ID_NOTES_CLUBS,           wxITEM_NORMAL, 1, nullptr, arbT("MenuNotesClubs"), arbT("DescNotesClubs"), nullptr},
	{MENU_ITEM, ID_NOTES_JUDGES,          wxITEM_NORMAL, 1, nullptr, arbT("MenuNotesJudges"), arbT("DescNotesJudges"), nullptr},
	{MENU_ITEM, ID_NOTES_LOCATIONS,       wxITEM_NORMAL, 1, nullptr, arbT("MenuNotesLocations"), arbT("DescNotesLocations"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_NOTES_SEARCH,          wxITEM_NORMAL, 1, nullptr, arbT("MenuNotesSearch"), arbT("DescNotesSearch"), nullptr},

	{MENU_ITEM, 0,                        wxITEM_NORMAL, 0, nullptr, arbT("MenuView"), nullptr, nullptr},
	{MENU_ITEM, wxID_PREFERENCES,         wxITEM_NORMAL, 1, nullptr, arbT("MenuViewOptions"), arbT("DescViewOptions"), nullptr},
	{MENU_ITEM, ID_VIEW_CUSTOMIZE,        wxITEM_NORMAL, 1, nullptr, arbT("MenuViewCustomize"), arbT("DescViewCustomize"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_UNSORT,                wxITEM_NORMAL, 1, nullptr, arbT("MenuUnsort"), arbT("DescUnsort"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_VIEW_SORTRUNS,         wxITEM_CHECK,  1, nullptr, arbT("MenuViewNewestTrials"), arbT("DescViewNewestTrials"), nullptr},
	{MENU_ITEM, ID_VIEW_RUNS_BY_TRIAL,    wxITEM_CHECK,  1, nullptr, arbT("MenuViewRunsByTrial"), arbT("DescViewRunsByTrial"), nullptr},
	{MENU_ITEM, ID_VIEW_TABLE_IN_YPS,     wxITEM_CHECK,  1, nullptr, arbT("MenuViewIncludeTable"), arbT("DescViewIncludeTable"), nullptr},
	{MENU_ITEM, ID_VIEW_RUNTIME_IN_OPS,   wxITEM_CHECK,  1, nullptr, arbT("MenuViewIncludeRunTime"), arbT("DescViewIncludeRunTime"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_VIEW_POINTS_VIEW_SORT, wxITEM_NORMAL, 1, nullptr, arbT("MenuViewSortPoints"), arbT("DescViewSortPoints"), nullptr},
	{MENU_ITEM, ID_VIEW_HIDDEN,           wxITEM_CHECK,  1, nullptr, arbT("MenuViewHiddenTitles"), arbT("DescViewHiddenTitles"), nullptr},
	{MENU_ITEM, ID_VIEW_LIFETIME_EVENTS,  wxITEM_CHECK,  1, nullptr, arbT("MenuViewLifePoints"), arbT("DescViewLifePoints"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_EXPAND,                wxITEM_NORMAL, 1, nullptr, arbT("MenuViewExpand"), arbT("DescViewExpand"), nullptr},
	{MENU_ITEM, ID_COLLAPSE,              wxITEM_NORMAL, 1, nullptr, arbT("MenuViewCollapse"), arbT("DescViewCollapse"), nullptr},
	{MENU_ITEM, ID_EXPAND_ALL,            wxITEM_NORMAL, 1, nullptr, arbT("MenuViewExpandAll"), arbT("DescViewExpandAll"), nullptr},
	{MENU_ITEM, ID_COLLAPSE_ALL,          wxITEM_NORMAL, 1, nullptr, arbT("MenuViewCollapseAll"), arbT("DescViewCollapseAll"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, 0,                        wxITEM_NORMAL, 1, nullptr, arbT("MenuViewType"), nullptr, nullptr},
#if wxUSE_NOTEBOOK
	{MENU_ITEM, ID_BOOK_NOTEBOOK,         wxITEM_RADIO,  2, nullptr, arbT("MenuViewNotebook"), arbT("DescViewNotebook"), nullptr},
#endif
#if wxUSE_LISTBOOK
	{MENU_ITEM, ID_BOOK_LISTBOOK,         wxITEM_RADIO,  2, nullptr, arbT("MenuViewListbook"), arbT("DescViewListbook"), nullptr},
#endif
#if wxUSE_CHOICEBOOK
	{MENU_ITEM, ID_BOOK_CHOICEBOOK,       wxITEM_RADIO,  2, nullptr, arbT("MenuViewChoicebook"), arbT("DescViewChoicebook"), nullptr},
#endif
#if wxUSE_TREEBOOK
	{MENU_ITEM, ID_BOOK_TREEBOOK,         wxITEM_RADIO,  2, nullptr, arbT("MenuViewTreebook"), arbT("DescViewTreebook"), nullptr},
#endif
#if wxUSE_TOOLBOOK
	{MENU_ITEM, ID_BOOK_TOOLBOOK,         wxITEM_RADIO,  2, nullptr, arbT("MenuViewToolbook"), arbT("DescViewToolbook"), nullptr},
#endif
	{MENU_ITEM, 0,                        wxITEM_NORMAL, 1, nullptr, arbT("MenuViewOrientation"), nullptr, nullptr},
	{MENU_ITEM, ID_ORIENT_TOP,            wxITEM_RADIO,  2, nullptr, arbT("MenuViewTop"), arbT("DescViewTop"), nullptr},
	{MENU_ITEM, ID_ORIENT_BOTTOM,         wxITEM_RADIO,  2, nullptr, arbT("MenuViewBottom"), arbT("DescViewBottom"), nullptr},
	{MENU_ITEM, ID_ORIENT_LEFT,           wxITEM_RADIO,  2, nullptr, arbT("MenuViewLeft"), arbT("DescViewLeft"), nullptr},
	{MENU_ITEM, ID_ORIENT_RIGHT,          wxITEM_RADIO,  2, nullptr, arbT("MenuViewRight"), arbT("DescViewRight"), nullptr},

	{MENU_ITEM, 0,                        wxITEM_NORMAL, 0, nullptr, arbT("MenuWindow"), nullptr, nullptr},
	{MENU_ITEM, ID_WINDOW_SPLIT,          wxITEM_NORMAL, 1, nullptr, arbT("MenuWindowSplitter"), arbT("DescWindowSplitter"), nullptr},
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
	{MENU_ITEM, ID_NEXT_PANE,             wxITEM_NORMAL, 1, nullptr, arbT("MenuWindowNext"), arbT("DescWindowNext"), nullptr},
	{MENU_ITEM, ID_PREV_PANE,             wxITEM_NORMAL, 1, nullptr, arbT("MenuWindowPrev"), arbT("DescWindowPrev"), nullptr},

	{MENU_ITEM, 0,                        wxITEM_NORMAL, 0, nullptr, arbT("MenuHelp"), nullptr, nullptr},
#ifdef _DEBUG
	{MENU_ITEM, ID_HELP_DEBUG,            wxITEM_NORMAL, 1, nullptr, L"Debug: Test Feature", L"Debug Helper", nullptr},
#endif
	{MENU_ITEM, ID_HELP_UPDATE,           wxITEM_NORMAL, 1, nullptr, arbT("MenuHelpUpdates"), arbT("DescHelpUpdates"), nullptr},
	{MENU_ITEM, ID_HELP_SYSINFO,          wxITEM_NORMAL, 1, nullptr, arbT("MenuHelpSysInfo"), arbT("DescHelpSysInfo"), nullptr},
#ifndef __WXMAC__
	{MENU_SEP,  0,                        wxITEM_NORMAL, 1, nullptr, nullptr, nullptr, nullptr},
#endif
	{MENU_HELP, wxID_ABOUT,               wxITEM_NORMAL, 1, arbT("MenuHelpAbout"), arbT("MenuHelpAbout"), arbT("DescHelpAbout"), ImageMgrAbout},
};
static const size_t sc_ItemsCount = sizeof(sc_Items) / sizeof(sc_Items[0]);

static const int sc_toolbarItems[] =
{
	wxID_NEW,
	wxID_OPEN,
	wxID_SAVE,
	0,
	ID_AGILITY_NEW_DOG,
	ID_AGILITY_NEW_TITLE,
	ID_AGILITY_NEW_TRIAL,
	ID_AGILITY_NEW_RUN,
	ID_AGILITY_NEW_CALENDAR,
	ID_AGILITY_NEW_TRAINING,
	0,
	wxID_CUT,
	wxID_COPY,
	wxID_PASTE,
	0,
	wxID_PREVIEW,
	wxID_PRINT,
	0,
	wxID_ABOUT
};
static const size_t sc_toolbarItemsCount = sizeof(sc_toolbarItems) / sizeof(sc_toolbarItems[0]);


void CreateMainMenu(
		CMenuHelper& menus,
		wxFrame* pFrame,
		wxMenu* pRecent)
{
	menus.CreateMenu(pFrame, sc_Items, sc_ItemsCount, sc_toolbarItems, sc_toolbarItemsCount, true, pRecent);
}


/////////////////////////////////////////////////////////////////////////////
// Popup menus are created each time, so we don't need the effort of above
// to fix the text on language change.

// Create New submenu with dog/title/trial/run
static wxMenu* CreateNewMenu(bool incRun)
{
	wxMenu* menu = new wxMenu;
	CMenuHelper::DoMenuItem(menu, ID_AGILITY_NEW_DOG, _("MenuDogNew"), _("DescDogNew"), ImageMgrDog);
	CMenuHelper::DoMenuItem(menu, ID_AGILITY_NEW_TITLE, _("MenuTitleNew"), _("DescTitleNew"), ImageMgrTitle);
	menu->AppendSeparator();
	CMenuHelper::DoMenuItem(menu, ID_AGILITY_NEW_TRIAL, _("MenuTrialNew"), _("DescTrialNew"), ImageMgrTrial);
	if (incRun)
	{
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_NEW_RUN, _("MenuRunNew"), _("DescRunNew"), ImageMgrRuns);
	}
	return menu;
}


wxMenu* CreatePopup(MenuIdentityPopup idMenu)
{
	wxMenu* menu = new wxMenu;

	switch (idMenu)
	{
	default:
		// all items in 'enum MenuIdentityPopup' should be listed
		assert(0);
		break;

	case IdMenuNone:
		break;

	case IdMenuDog:
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_EDIT_DOG, _("MenuDogProperties"), _("DescDogProperties"), ImageMgrDog);
		CMenuHelper::DoMenuItem(menu, wxID_PREFERENCES, _("MenuViewOptions"), _("DescViewOptions"));
		menu->AppendSeparator();
		menu->Append(wxID_ANY, _("MenuNew"), CreateNewMenu(false));
		CMenuHelper::DoMenuItem(menu, wxID_DUPLICATE, _("MenuEditDuplicate"), _("DescEditDuplicate"));
		CMenuHelper::DoMenuItem(menu, wxID_CUT, _("MenuEditCut"), _("DescEditCut"), ImageMgrCut);
		CMenuHelper::DoMenuItem(menu, wxID_COPY, _("MenuEditCopy"), _("DescEditCopy"), ImageMgrCopy);
		CMenuHelper::DoMenuItem(menu, wxID_PASTE, _("MenuEditPaste"), _("DescEditPaste"), ImageMgrPaste);
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_DELETE_DOG, _("MenuDogDelete"), _("DescDogDelete"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_REORDER, _("MenuEditReorder"), _("DescEditReorder"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_EXPAND, _("MenuViewExpand"), _("DescViewExpand"));
		CMenuHelper::DoMenuItem(menu, ID_COLLAPSE, _("MenuViewCollapse"), _("DescViewCollapse"));
		CMenuHelper::DoMenuItem(menu, ID_EXPAND_ALL, _("MenuViewExpandAll"), _("DescViewExpandAll"));
		CMenuHelper::DoMenuItem(menu, ID_COLLAPSE_ALL, _("MenuViewCollapseAll"), _("DescViewCollapseAll"));
		break;

	case IdMenuTrial:
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_EDIT_TRIAL, _("MenuTrialProperties"), _("DescTrialProperties"), ImageMgrTrial);
		CMenuHelper::DoMenuItem(menu, wxID_PREFERENCES, _("MenuViewOptions"), _("DescViewOptions"));
		menu->AppendSeparator();
		menu->Append(wxID_ANY, _("MenuNew"), CreateNewMenu(true));
		CMenuHelper::DoMenuItem(menu, wxID_DUPLICATE, _("MenuEditDuplicate"), _("DescEditDuplicate"));
		CMenuHelper::DoMenuItem(menu, wxID_CUT, _("MenuEditCut"), _("DescEditCut"), ImageMgrCut);
		CMenuHelper::DoMenuItem(menu, wxID_COPY, _("MenuEditCopy"), _("DescEditCopy"), ImageMgrCopy);
		CMenuHelper::DoMenuItem(menu, wxID_PASTE, _("MenuEditPaste"), _("DescEditPaste"), ImageMgrPaste);
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_DELETE_TRIAL, _("MenuTrialDelete"), _("DescTrialDelete"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_REORDER, _("MenuEditReorder"), _("DescEditReorder"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_EXPAND, _("MenuViewExpand"), _("DescViewExpand"));
		CMenuHelper::DoMenuItem(menu, ID_COLLAPSE, _("MenuViewCollapse"), _("DescViewCollapse"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_PRINT_TRIAL, _("MenuTrialPrintRuns"), _("DescTrialPrintRuns"));
		break;

	case IdMenuRun:
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_EDIT_RUN, _("MenuRunProperties"), _("DescRunProperties"), ImageMgrRuns);
		CMenuHelper::DoMenuItem(menu, wxID_PREFERENCES, _("MenuViewOptions"), _("DescViewOptions"));
		menu->AppendSeparator();
		menu->Append(wxID_ANY, _("MenuNew"), CreateNewMenu(true));
		CMenuHelper::DoMenuItem(menu, wxID_DUPLICATE, _("MenuEditDuplicate"), _("DescEditDuplicate"));
		CMenuHelper::DoMenuItem(menu, wxID_CUT, _("MenuEditCut"), _("DescEditCut"), ImageMgrCut);
		CMenuHelper::DoMenuItem(menu, wxID_COPY, _("MenuEditCopy"), _("DescEditCopy"), ImageMgrCopy);
		CMenuHelper::DoMenuItem(menu, wxID_PASTE, _("MenuEditPaste"), _("DescEditPaste"), ImageMgrPaste);
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_DELETE_RUN, _("MenuRunDelete"), _("DescRunDelete"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_REORDER, _("MenuEditReorder"), _("DescEditReorder"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_PRINT_TRIAL, _("MenuTrialPrintRuns"), _("DescTrialPrintRuns"));
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_PRINT_RUNS, _("MenuRunPrintRuns"), _("DescRunPrintRuns"));
		break;

	case IdMenuPoints:
		CMenuHelper::DoMenuItem(menu, ID_DETAILS, _("MenuDetails"), _("DescDetails"));
		CMenuHelper::DoMenuItem(menu, wxID_PREFERENCES, _("MenuViewOptions"), _("DescViewOptions"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, wxID_COPY, _("MenuEditCopySelection"), _("DescEditCopy"), ImageMgrCopy);
		CMenuHelper::DoMenuItem(menu, ID_COPY_TITLES_LIST, _("MenuEditCopyTitles"), _("DescEditCopyTitles"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, wxID_SELECTALL, _("MenuEditSelectAll"), _("DescEditSelectAll"));
		break;

	case IdMenuCalendar:
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_EDIT_CALENDAR, _("MenuCalendarProperties"), _("DescCalendarProperties"), ImageMgrCalendar);
		CMenuHelper::DoMenuItem(menu, wxID_PREFERENCES, _("MenuViewOptions"), _("DescViewOptions"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_NEW_CALENDAR, _("MenuNew"), _("DescCalendarNew"), ImageMgrCalendar);
		CMenuHelper::DoMenuItem(menu, wxID_DUPLICATE, _("MenuEditDuplicate"), _("DescEditDuplicate"));
		CMenuHelper::DoMenuItem(menu, wxID_CUT, _("MenuEditCut"), _("DescEditCut"), ImageMgrCut);
		CMenuHelper::DoMenuItem(menu, wxID_COPY, _("MenuEditCopy"), _("DescEditCopy"), ImageMgrCopy);
		CMenuHelper::DoMenuItem(menu, wxID_PASTE, _("MenuEditPaste"), _("DescEditPaste"), ImageMgrPaste);
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_DELETE_CALENDAR, _("MenuCalendarDelete"), _("DescCalendarDelete"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_UPDATE_CALENDAR, _("MenuCalendarUpdate"), _("DescCalendarUpdate"));
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_EXPORT_CALENDAR, _("MenuCalendarExport"), _("DescCalendarExport"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_CREATEENTRY_CALENDAR, _("MenuCalendarTrialEntry"), _("DescCalendarTrialEntry"));
		break;

	case IdMenuTraining:
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_EDIT_TRAINING, _("MenuTrainingProperties"), _("DescTrainingProperties"), ImageMgrTraining);
		CMenuHelper::DoMenuItem(menu, wxID_PREFERENCES, _("MenuViewOptions"), _("DescViewOptions"));
		menu->AppendSeparator();
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_NEW_TRAINING, _("MenuNew"), _("DescTrainingNew"), ImageMgrTraining);
		CMenuHelper::DoMenuItem(menu, wxID_DUPLICATE, _("MenuEditDuplicate"), _("DescEditDuplicate"));
		CMenuHelper::DoMenuItem(menu, wxID_CUT, _("MenuEditCut"), _("DescEditCut"), ImageMgrCut);
		CMenuHelper::DoMenuItem(menu, wxID_COPY, _("MenuEditCopy"), _("DescEditCopy"), ImageMgrCopy);
		CMenuHelper::DoMenuItem(menu, wxID_PASTE, _("MenuEditPaste"), _("DescEditPaste"), ImageMgrPaste);
		CMenuHelper::DoMenuItem(menu, ID_AGILITY_DELETE_TRAINING, _("MenuTrainingDelete"), _("DescTrainingDelete"));
		break;
	}

	return menu;
}
