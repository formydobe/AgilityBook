/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 *
 * @brief Data-driven helper to create menus and toolbar items.
 * @author David Connet
 *
 * Revision History
 * @li 2012-07-13 DRC Created
 */

#include "stdafx.h"
#include "MenuHelper.h"

#include "wx/frame.h"

#if defined(__WXMSW__)
#include <wx/msw/msvcrt.h>
#endif


CMenuHelper::CMenuHelper()
	: m_Frame(NULL)
	, m_MenuBar(NULL)
	, m_doTranslation(false)
	, m_MenuData()
	, m_ToolbarData()
{
}


CMenuHelper::~CMenuHelper()
{
}


void CMenuHelper::DoMenuItem(
		wxMenu* menu,
		int id,
		wxString const& label,
		wxString const& desc,
		const char* const* pBitmap)
{
	DoMenuItem(menu, id, label, desc, wxITEM_NORMAL, NULL, pBitmap);
}


void CMenuHelper::DoMenuItem(
		wxMenu* menu,
		int id,
		wxString const& label,
		wxString const& desc,
		wxItemKind kind,
		wxMenu* subMenu,
		const char* const* pBitmap)
{
	wxMenuItem* item = new wxMenuItem(menu, id, label, desc, kind, subMenu);
	if (pBitmap && kind == wxITEM_NORMAL)
	{
		wxBitmap bmp(pBitmap);
		item->SetBitmap(bmp);
	}
	menu->Append(item);
}


void CMenuHelper::Menu(
		MenuHandle& handle,
		size_t& index,
		size_t level,
		wxMenu* mruMenu,
		bool& mruAdded,
		ItemData const items[],
		size_t numItems)
{
	for (++index; index < numItems && level == items[index].menuLevel; ++index)
	{
		if (MENU_SEP & items[index].flags)
			handle.pMenu->AppendSeparator();
		else if ((MENU_ITEM | MENU_MRU) & items[index].flags)
		{
			assert(items[index].menu);
			if (MENU_MRU & items[index].flags)
			{
				assert(mruMenu);
				assert(!mruAdded);
				mruAdded = true;
				wxString name;
				MenuHandle subhandle(mruMenu, static_cast<int>(handle.pMenu->GetMenuItemCount()));
				if (m_doTranslation)
				{
					name = wxGetTranslation(items[index].menu);
					subhandle.item = items[index].menu;
				}
				else
				{
					name = items[index].menu;
				}
				DoMenuItem(handle.pMenu, wxID_ANY,
					name,
					wxString(),
					wxITEM_NORMAL,
					mruMenu,
					items[index].bitmap);
				handle.subMenus.push_back(subhandle);
			}
			else if (0 == items[index].id)
			{
				size_t idxMenu = index;
				MenuHandle subhandle(static_cast<int>(handle.pMenu->GetMenuItemCount()));
				Menu(subhandle, index, level + 1, mruMenu, mruAdded, items, numItems);
				wxString name;
				if (m_doTranslation)
				{
					name = wxGetTranslation(items[idxMenu].menu);
					subhandle.item = items[index].menu;
				}
				else
				{
					name = items[idxMenu].menu;
				}
				DoMenuItem(handle.pMenu, wxID_ANY,
					name,
					wxString(),
					wxITEM_NORMAL,
					subhandle.pMenu,
					items[idxMenu].bitmap);
				--index;
				handle.subMenus.push_back(subhandle);
			}
			else
			{
				wxString name;
				wxString help;
				if (m_doTranslation)
				{
					name = wxGetTranslation(items[index].menu);
					help = wxGetTranslation(items[index].help);
					handle.items.push_back(TranslationData(items[index].id, items[index].menu, items[index].help));
				}
				else
				{
					name = items[index].menu;
					help = items[index].help;
				}
				DoMenuItem(handle.pMenu, items[index].id,
					name,
					help,
					items[index].kind,
					NULL,
					items[index].bitmap);
			}
		}
	}
}


void CMenuHelper::CreateMenu(
		wxFrame* pFrame,
		ItemData const items[],
		size_t numItems,
		bool doTranslation,
		wxMenu* mruMenu)
{
	assert(!m_MenuBar);
	m_Frame = pFrame;
	m_doTranslation = doTranslation;

	if (0 < numItems)
	{
		m_MenuBar = new wxMenuBar();
		bool mruAdded = false;

		for (size_t index = 0; index < numItems; )
		{
			if (!((MENU_ITEM | MENU_HELP) & items[index].flags))
				continue;
			assert(items[index].menu);
			size_t idxMenu = index;
			MenuHandle handle(static_cast<int>(m_MenuBar->GetMenuCount()));
			Menu(handle, index, 1, mruMenu, mruAdded, items, numItems);
			wxString name;
			if (m_doTranslation)
			{
				name = wxGetTranslation(items[idxMenu].menu);
				handle.item = items[idxMenu].menu;
			}
			else
			{
				name = items[idxMenu].menu;
			}
#ifdef __WXMAC__
			if (MENU_HELP & items[index].flags)
			{
				wxApp::s_macHelpMenuTitleName = name;
			}
#endif
			m_MenuBar->Append(handle.pMenu, name);
			m_MenuData.push_back(handle);
		}

		if (mruMenu)
		{
			assert(mruAdded);
		}

		m_Frame->SetMenuBar(m_MenuBar);
	}
}


static size_t FindItem(int id, CMenuHelper::ItemData const menuItems[], size_t numMenuItems)
{
	for (size_t i = 0; i < numMenuItems; ++i)
	{
		if (menuItems[i].id == id)
			return i;
	}
	return numMenuItems;
}


void CMenuHelper::CreateMenu(
		wxFrame* pFrame,
		ItemData const menuItems[],
		size_t numMenuItems,
		int const toolbarItems[],
		size_t numToolbarItems,
		bool doTranslation,
		wxMenu* mruMenu)
{
	CreateMenu(pFrame, menuItems, numMenuItems, doTranslation, mruMenu);

	if (0 < numMenuItems && 0 < numToolbarItems)
	{
		wxToolBar* toolbar = m_Frame->CreateToolBar(wxTB_FLAT);

		for (size_t index = 0; index < numToolbarItems; ++index)
		{
			if (0 == toolbarItems[index])
			{
				toolbar->AddSeparator();
			}
			else
			{
				size_t idxItem = FindItem(toolbarItems[index], menuItems, numMenuItems);
				if (idxItem >= numMenuItems)
					continue;
				assert(menuItems[idxItem].toolbar);
				wxString name;
				wxString desc;
				if (doTranslation)
				{
					name = wxGetTranslation(menuItems[idxItem].toolbar);
					desc = wxGetTranslation(menuItems[idxItem].help);
					m_ToolbarData.push_back(TranslationData(menuItems[idxItem].id, menuItems[idxItem].toolbar, menuItems[idxItem].help));
				}
				else
				{
					name = menuItems[idxItem].toolbar;
					desc = menuItems[idxItem].help;
				}
				toolbar->AddTool(menuItems[idxItem].id,
					name,
					menuItems[idxItem].bitmap,
					desc,
					menuItems[idxItem].kind);
			}
		}
		toolbar->Realize();
	}
}


void CMenuHelper::DoSubMenu(wxMenu* parent, MenuHandle const& handle)
{
	if (parent)
	{
#if wxCHECK_VERSION(2, 9, 4)
		parent->FindItemByPosition(handle.idx)->SetItemLabel(wxGetTranslation(handle.item.c_str()));
#else
		parent->FindItemByPosition(handle.idx)->SetText(wxGetTranslation(handle.item.c_str()));
#endif
	}
	for (std::vector<MenuHandle>::const_iterator data = handle.subMenus.begin(); data != handle.subMenus.end(); ++data)
	{
		DoSubMenu(handle.pMenu, *data);
	}
	for (std::vector<TranslationData>::const_iterator data = handle.items.begin(); data != handle.items.end(); ++data)
	{
		handle.pMenu->SetLabel(data->id, wxGetTranslation(data->name.c_str()));
		handle.pMenu->SetHelpString(data->id, wxGetTranslation(data->desc.c_str()));
	}
}


void CMenuHelper::UpdateMenu()
{
	if (m_doTranslation)
	{
		for (std::vector<MenuHandle>::iterator i = m_MenuData.begin(); i != m_MenuData.end(); ++i)
		{
			m_MenuBar->SetMenuLabel(i->idx, wxGetTranslation(i->item.c_str()));
			DoSubMenu(NULL, *i);
		}

		wxToolBar* toolbar = m_Frame->GetToolBar();
		if (toolbar)
		{
			for (std::vector<TranslationData>::iterator i = m_ToolbarData.begin(); i != m_ToolbarData.end(); ++i)
			{
				if (0 == i->id)
					continue;
				wxToolBarToolBase* btn = toolbar->FindById(i->id);
				if (btn)
				{
					btn->SetLabel(wxGetTranslation(i->name.c_str()));
					btn->SetShortHelp(wxGetTranslation(i->desc.c_str()));
				}
			}
		}
	}
}