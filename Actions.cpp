#include "Common.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

void Extension::LoadTranslation(const TCHAR* filePath)
{
	std::tstring newPath = DarkEdif::MakePathUnembeddedIfNeeded(this, filePath);

	if (!std::filesystem::exists(newPath))
	{
		ThrowError(ErrorCode::TranslationFailedToLoad, _T("Could not find filepath '") + std::tstring(filePath) + _T("'"));
		return;
	}

	std::unique_ptr<std::thread> t = std::make_unique<std::thread>([=]() { this->LoadTranslationAsync(newPath.c_str()); });
	Threads.push_back(std::move(t));
}

void Extension::LoadTranslationAsync(const TCHAR* filePath)
{
	std::ifstream ifs(filePath);
	std::string po_file{ std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>() };

	spiritCat = new spirit_po::default_catalog(spirit_po::default_catalog::from_range(po_file));
	CheckoutTicket(CallType::TranslationUpdated, Cnd_TranslationUpdated);
}

void Extension::UnloadTranslation()
{
	if (TranslationLoaded())
	{
		spiritCat = NULL;
		CheckoutTicket(CallType::TranslationUpdated, Cnd_TranslationUpdated);
	}
}
