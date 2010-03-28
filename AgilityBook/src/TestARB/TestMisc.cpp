/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief Test misc functions
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2008-01-12 DRC Created
 */

#include "stdafx.h"
#include "TestARB.h"

#include "ARBTypes.h"


SUITE(TestMisc)
{
	TEST(Html_Sanitize)
	{
		if (!g_bMicroTest)
		{
			wxString s(wxT("<&amp>"));
			wxString s2 = SanitizeStringForHTML(s);
			CHECK(wxT("&lt;&amp;amp&gt;") == s2);
			s = wxT("1\r\n2\n3");
			s2 = SanitizeStringForHTML(s, true);
			CHECK(wxT("1<br/>2<br/>3") == s2);
			s2 = SanitizeStringForHTML(s, false);
			CHECK(wxT("1\r\n2\n3") == s2);
		}
	}


	TEST(Localization)
	{
		if (!g_bMicroTest)
		{
			wxString s1(wxT("IDS_ARB_UNKNOWN_VERSION"));
			wxString s2(_("IDS_ARB_UNKNOWN_VERSION"));
			CHECK(s1 != s2);
		}
	}


#ifdef WIN32
	TEST(VerifyColor)
	{
		if (!g_bMicroTest)
		{
			CHECK(wxSYS_COLOUR_SCROLLBAR == COLOR_SCROLLBAR);
			CHECK(wxSYS_COLOUR_DESKTOP == COLOR_BACKGROUND);
			CHECK(wxSYS_COLOUR_ACTIVECAPTION == COLOR_ACTIVECAPTION);
			CHECK(wxSYS_COLOUR_INACTIVECAPTION == COLOR_INACTIVECAPTION);
			CHECK(wxSYS_COLOUR_MENU == COLOR_MENU);
			CHECK(wxSYS_COLOUR_WINDOW == COLOR_WINDOW);
			CHECK(wxSYS_COLOUR_WINDOWFRAME == COLOR_WINDOWFRAME);
			CHECK(wxSYS_COLOUR_MENUTEXT == COLOR_MENUTEXT);
			CHECK(wxSYS_COLOUR_WINDOWTEXT == COLOR_WINDOWTEXT);
			CHECK(wxSYS_COLOUR_CAPTIONTEXT == COLOR_CAPTIONTEXT);
			CHECK(wxSYS_COLOUR_ACTIVEBORDER == COLOR_ACTIVEBORDER);
			CHECK(wxSYS_COLOUR_INACTIVEBORDER == COLOR_INACTIVEBORDER);
			CHECK(wxSYS_COLOUR_APPWORKSPACE == COLOR_APPWORKSPACE);
			CHECK(wxSYS_COLOUR_HIGHLIGHT == COLOR_HIGHLIGHT);
			CHECK(wxSYS_COLOUR_HIGHLIGHTTEXT == COLOR_HIGHLIGHTTEXT);
			CHECK(wxSYS_COLOUR_BTNFACE == COLOR_BTNFACE);
			CHECK(wxSYS_COLOUR_BTNSHADOW == COLOR_BTNSHADOW);
			CHECK(wxSYS_COLOUR_GRAYTEXT == COLOR_GRAYTEXT);
			CHECK(wxSYS_COLOUR_BTNTEXT == COLOR_BTNTEXT);
			CHECK(wxSYS_COLOUR_INACTIVECAPTIONTEXT == COLOR_INACTIVECAPTIONTEXT);
			CHECK(wxSYS_COLOUR_BTNHIGHLIGHT == COLOR_BTNHIGHLIGHT);
			CHECK(wxSYS_COLOUR_3DDKSHADOW == COLOR_3DDKSHADOW);
			CHECK(wxSYS_COLOUR_3DLIGHT == COLOR_3DLIGHT);
			CHECK(wxSYS_COLOUR_INFOTEXT == COLOR_INFOTEXT);
			CHECK(wxSYS_COLOUR_INFOBK == COLOR_INFOBK);
			//CHECK(wxSYS_COLOUR_LISTBOX == );
			CHECK(wxSYS_COLOUR_HOTLIGHT == COLOR_HOTLIGHT);
			CHECK(wxSYS_COLOUR_GRADIENTACTIVECAPTION == COLOR_GRADIENTACTIVECAPTION);
			CHECK(wxSYS_COLOUR_GRADIENTINACTIVECAPTION == COLOR_GRADIENTINACTIVECAPTION);
			//CHECK(wxSYS_COLOUR_MENUHILIGHT == );
			//CHECK(wxSYS_COLOUR_MENUBAR == );
			//CHECK(wxSYS_COLOUR_LISTBOXTEXT == );
		}
	}
#endif
}
