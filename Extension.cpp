#include "Common.hpp"

///
/// EXTENSION CONSTRUCTOR/DESTRUCTOR
///

#ifdef _WIN32
Extension::Extension(RunObject* const _rdPtr, const EDITDATA* const edPtr, const CreateObjectInfo* const cobPtr) :
	rdPtr(_rdPtr), rhPtr(_rdPtr->get_rHo()->get_AdRunHeader()), Runtime(this)
#elif defined(__ANDROID__)
Extension::Extension(const EDITDATA* const edPtr, const jobject javaExtPtr) :
	javaExtPtr(javaExtPtr, "Extension::javaExtPtr from Extension ctor"),
	Runtime(this, this->javaExtPtr)
#else
Extension::Extension(const EDITDATA* const edPtr, void* const objCExtPtr) :
	objCExtPtr(objCExtPtr), Runtime(this, objCExtPtr)
#endif
{
	LinkAction(0, LoadTranslation);
	LinkAction(1, UnloadTranslation);

	LinkCondition(0, ImmediateCondition /* Cnd_TranslationLoaded	*/);
	LinkCondition(1, ImmediateCondition /* Cnd_OnError				*/);
	LinkCondition(2, HasKey);
	LinkCondition(3, TranslationLoaded);
	LinkCondition(4, HasKeyWithContext);
	LinkCondition(5, HasPluralKey);
	LinkCondition(6, HasPluralKeyWithContext);

	LinkExpression(0, GetKeyValue);
	LinkExpression(1, GetErrorCode);
	LinkExpression(2, GetErrorReason);
	LinkExpression(3, GetLangCode);
	LinkExpression(4, GetKeyValueWithContext);
	LinkExpression(5, GetPluralKeyValue);
	LinkExpression(6, GetMetadata);
	LinkExpression(7, GetPluralKeyValueWithContext);
}

Extension::~Extension()
{
	for (auto& t : Threads)
		t->join();
}

// This should only be used for asynchronous tickets
// Otherwise, please just use Runtime.GenerateEvent so they
// run immediately
void Extension::CheckoutTicket(CallType type, int trigger)
{
	TriggerLock.edif_lock();
	std::unique_ptr<CallTicket> ticket = std::make_unique<CallTicket>();
	ticket->Type = type;
	if (trigger >= 0)
	{
		ticket->HasTrigger = true;
		ticket->Trigger = trigger;
	}
	TriggerBuffer.push_back(std::move(ticket));
	TriggerLock.edif_unlock();
}

REFLAG Extension::Handle()
{
	if (TriggerLock.edif_try_lock())
	{
		while (!TriggerBuffer.empty())
		{
			if (TriggerBuffer[0]->HasTrigger)
				Runtime.GenerateEvent(TriggerBuffer[0]->Trigger);
			TriggerBuffer.pop_front();
		}
		TriggerLock.edif_unlock();
	}
	return REFLAG::NONE;
}

void Extension::ThrowError(ErrorCode code, std::tstring reason)
{
	LatestErrorCode = code;
	LatestErrorReason = reason;
	Runtime.GenerateEvent(Cnd_OnError);
}

REFLAG Extension::Display()
{
	return REFLAG::DISPLAY;
}

short Extension::FusionRuntimePaused()
{
	return 0;
}

short Extension::FusionRuntimeContinued()
{
	return 0;
}

void Extension::UnlinkedAction(int ID)
{
	DarkEdif::MsgBox::Error(_T("Extension::UnlinkedAction() called"), _T("Running a fallback for action ID %d. Make sure you ran LinkAction()."), ID);
}

long Extension::UnlinkedCondition(int ID)
{
	DarkEdif::MsgBox::Error(_T("Extension::UnlinkedCondition() called"), _T("Running a fallback for condition ID %d. Make sure you ran LinkCondition()."), ID);
	return 0;
}

long Extension::UnlinkedExpression(int ID)
{
	DarkEdif::MsgBox::Error(_T("Extension::UnlinkedExpression() called"), _T("Running a fallback for expression ID %d. Make sure you ran LinkExpression()."), ID);
	// Unlinked A/C/E is fatal error, but try not to return null string and definitely crash it
	if ((size_t)ID < Edif::SDK->ExpressionInfos.size() && Edif::SDK->ExpressionInfos[ID]->Flags.ef == ExpReturnType::String)
		return (long)Runtime.CopyString(_T(""));
	return 0;
}
