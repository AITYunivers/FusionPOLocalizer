#include "Common.hpp"

bool Extension::ImmediateCondition()
{
	return true;
}

bool Extension::HasKey(const TCHAR* key)
{
	if (!TranslationLoaded())
		return false;

	std::tstring keyStr(key);
	return spiritCat->gettext_line_no(DarkEdif::TStringToUTF8(keyStr).c_str()) > 0;
}

bool Extension::HasPluralKey(const TCHAR* key)
{
	if (!TranslationLoaded())
		return false;

	std::tstring keyStr(key);
	return spiritCat->ngettext_line_no(DarkEdif::TStringToUTF8(keyStr).c_str()) > 0;
}

bool Extension::HasKeyWithContext(const TCHAR* key, const TCHAR* context)
{
	if (!TranslationLoaded())
		return false;

	std::tstring keyStr(key);
	std::tstring contextStr(context);
	return spiritCat->pgettext_line_no(DarkEdif::TStringToUTF8(contextStr).c_str(), DarkEdif::TStringToUTF8(keyStr).c_str()) > 0;
}

bool Extension::HasPluralKeyWithContext(const TCHAR* key, const TCHAR* context)
{
	if (!TranslationLoaded())
		return false;

	std::tstring keyStr(key);
	std::tstring contextStr(context);
	return spiritCat->npgettext_line_no(DarkEdif::TStringToUTF8(contextStr).c_str(), DarkEdif::TStringToUTF8(keyStr).c_str()) > 0;
}

bool Extension::TranslationLoaded()
{
	return spiritCat && spiritCat->size() > 0;
}
