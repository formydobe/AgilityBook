/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief File hashing algorithms
 *
 * Revision History
 * @li 2013-10-29 DRC Added more hash algorithms
 */

extern std::wstring ARBMsgDigestComputeMD5(
		wchar_t const* const inFileName,
		size_t* outSize);
extern std::wstring ARBMsgDigestComputeMD5(
		std::istream& inFile,
		size_t* outSize);

extern std::wstring ARBMsgDigestComputeSHA1(
		wchar_t const* const inFileName,
		size_t* outSize);
extern std::wstring ARBMsgDigestComputeSHA1(
		std::istream& inFile,
		size_t* outSize);

extern std::wstring ARBMsgDigestComputeSHA256(
		wchar_t const* const inFileName,
		size_t* outSize);
extern std::wstring ARBMsgDigestComputeSHA256(
		std::istream& inFile,
		size_t* outSize);
