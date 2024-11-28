#pragma once
#include "DarkEdif.hpp"
#include <deque>
#include <CallTicket.hpp>
#include <ErrorCode.hpp>

class Extension
{
public:

	RunHeader* rhPtr;
	RunObjectMultiPlatPtr rdPtr; // you should not need to access this
#ifdef __ANDROID__
	global<jobject> javaExtPtr;
#elif defined(__APPLE__)
	void* const objCExtPtr;
#endif

	Edif::Runtime Runtime;

	static const int MinimumBuild = 254;
	static const int Version = 1;

	// If you change OEFLAGS, make sure you modify RUNDATA so the data is available, or you'll get crashes!
	// For example, OEFLAGS::VALUES makes use of the AltVals rv struct.
	static const OEFLAGS OEFLAGS = OEFLAGS::NONE;
	static const OEPREFS OEPREFS = OEPREFS::NONE;

	static const int WindowProcPriority = 100;

#ifdef _WIN32
	Extension(RunObject* const rdPtr, const EDITDATA* const edPtr, const CreateObjectInfo* const cobPtr);
#elif defined(__ANDROID__)
	Extension(const EDITDATA* const edPtr, const jobject javaExtPtr);
#else
	Extension(const EDITDATA* const edPtr, void* const objCExtPtr);
#endif
	~Extension();

	std::tstring DefaultLangCode;
	std::deque<std::unique_ptr<std::thread>> Threads;

	// Spirit-PO stuff

		spirit_po::default_catalog* spiritCat = NULL;

	/// Actions

		void LoadTranslation(const TCHAR* filePath);
		void LoadTranslationAsync(const TCHAR* filePath);
		void UnloadTranslation();

	/// Conditions

		bool ImmediateCondition();

		bool TranslationLoaded();

		bool HasKey(const TCHAR* key);
		bool HasPluralKey(const TCHAR* key);
		bool HasKeyWithContext(const TCHAR* key, const TCHAR* context);
		bool HasPluralKeyWithContext(const TCHAR* key, const TCHAR* context);

#define Cnd_TranslationUpdated	0
#define Cnd_OnError				1

	/// Expressions

		const TCHAR* GetKeyValue(const TCHAR* key);
		const TCHAR* GetKeyValueWithContext(const TCHAR* key, const TCHAR* context);
		const TCHAR* GetPluralKeyValue(const TCHAR* key, const TCHAR* pluralKey, int pluralCheck);
		const TCHAR* GetPluralKeyValueWithContext(const TCHAR* key, const TCHAR* pluralKey, int pluralCheck, const TCHAR* context);

		const TCHAR* GetLangCode();
		const TCHAR* GetMetadata(const TCHAR* key);

		int GetErrorCode();
		const TCHAR* GetErrorReason();



	/* These are called if there's no function linked to an ID */

	void UnlinkedAction(int ID);
	long UnlinkedCondition(int ID);
	long UnlinkedExpression(int ID);

	// Ticket System

	Edif::recursive_mutex TriggerLock;
	std::deque<std::unique_ptr<CallTicket>> TriggerBuffer;
	void CheckoutTicket(CallType type, int trigger);

	// Error System

	ErrorCode LatestErrorCode;
	std::tstring LatestErrorReason;
	void ThrowError(ErrorCode code, std::tstring reason);


	/*  These replace the functions like HandleRunObject that used to be
		implemented in Runtime.cpp. They work exactly the same, but they're
		inside the extension class.
	*/

	REFLAG Handle();
	REFLAG Display();

	short FusionRuntimePaused();
	short FusionRuntimeContinued();
};
