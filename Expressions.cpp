#include "Common.hpp"

const TCHAR * Extension::GetKeyValue(const TCHAR* key)
{
	if (!TranslationLoaded() || !HasKey(key))
		return Runtime.CopyString(key);

	std::tstring keyStr(key);
	return Runtime.CopyString(DarkEdif::UTF8ToTString(spiritCat->gettext(DarkEdif::TStringToUTF8(keyStr).c_str())).c_str());
}

const TCHAR* Extension::GetKeyValueWithContext(const TCHAR* key, const TCHAR* context)
{
	if (!TranslationLoaded() || !HasKeyWithContext(key, context))
		return Runtime.CopyString(key);

	std::tstring keyStr(key);
	std::tstring contextStr(context);
	return Runtime.CopyString(DarkEdif::UTF8ToTString(spiritCat->pgettext(DarkEdif::TStringToUTF8(contextStr).c_str(), DarkEdif::TStringToUTF8(keyStr).c_str())).c_str());
}

const TCHAR* Extension::GetPluralKeyValue(const TCHAR* key, const TCHAR* pluralKey, int pluralCheck)
{
	if (!TranslationLoaded() || !HasPluralKey(key))
		return Runtime.CopyString(key);

	std::tstring keyStr(key);
	std::tstring pluralKeyStr(pluralKey);
	return Runtime.CopyString(DarkEdif::UTF8ToTString(spiritCat->ngettext(DarkEdif::TStringToUTF8(keyStr).c_str(), DarkEdif::TStringToUTF8(pluralKeyStr).c_str(), pluralCheck)).c_str());
}

const TCHAR* Extension::GetPluralKeyValueWithContext(const TCHAR* key, const TCHAR* pluralKey, int pluralCheck, const TCHAR* context)
{
	if (!TranslationLoaded() || !HasPluralKeyWithContext(key, context))
		return Runtime.CopyString(key);

	std::tstring keyStr(key);
	std::tstring pluralKeyStr(pluralKey);
	std::tstring contextStr(context);
	return Runtime.CopyString(DarkEdif::UTF8ToTString(spiritCat->npgettext(DarkEdif::TStringToUTF8(contextStr).c_str(), DarkEdif::TStringToUTF8(keyStr).c_str(), DarkEdif::TStringToUTF8(pluralKeyStr).c_str(), pluralCheck)).c_str());
}

const TCHAR* Extension::GetLangCode()
{
	if (!TranslationLoaded())
		return Runtime.CopyString(_T(""));

	return Runtime.CopyString(DarkEdif::UTF8ToTString(spiritCat->get_metadata().language).c_str());
}

const TCHAR* Extension::GetMetadata(const TCHAR* key)
{
	if (!TranslationLoaded())
		return Runtime.CopyString(_T(""));

	std::tstring keyStr(key);
	spirit_po::catalog_metadata metadata = spiritCat->get_metadata();
	return Runtime.CopyString(DarkEdif::UTF8ToTString(metadata.find_header_line(metadata.header, DarkEdif::TStringToUTF8(keyStr))).c_str());
}

int Extension::GetErrorCode()
{
	return LatestErrorCode;
}

const TCHAR * Extension::GetErrorReason()
{
	TCHAR* errReason = Runtime.CopyString(LatestErrorReason.c_str());
	LatestErrorReason = _T("");
	return errReason;
}
