/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See Licence.txt
 */

/**
 * @file
 * @brief Test ARBVersion class
 * @author David Connet
 *
 * Revision History
 * @li 2008-01-12 DRC Created
 */

#include "stdafx.h"
#include "TestARB.h"

#include "ARBTypes.h"


SUITE(TestVersion)
{
	TEST(ctor)
	{
		unsigned short maj = 0, minor = 0;
		ARBVersion v;
		CHECK(maj == v.Major());
		CHECK(minor == v.Minor());

		maj = 1;
		minor = 2;
		ARBVersion v2(maj, minor);
		CHECK(maj == v2.Major());
		CHECK(minor == v2.Minor());
	}


	TEST(str)
	{
		ARBVersion v(1, 2);
		CHECK(wxT("1.2") == v.str());
	}


	TEST(Comparison)
	{
		ARBVersion v1(1, 2);
		ARBVersion v2(2, 1);
		CHECK(v1 < v2);
	}
}