// DarkEdifPreBuildTool: This file was generated for a Extension.cpp/h that was modified at 2024-11-28T02:56:01.8410019Z.
// This file is used by the DarkEdifPreBuildTool as an input template for the calltable generator.
// Do not modify or delete this file.
#if DARKEDIF_ACE_CALL_TABLE_INDEX==0
			case 0:
				return ext->ImmediateCondition();
			case 1:
				return ext->ImmediateCondition();

#elif DARKEDIF_ACE_CALL_TABLE_INDEX==1
			case 0:
				return ext->LoadTranslation(*(const TCHAR* *)&Parameters[0]), 0;
			case 1:
				return ext->UnloadTranslation(), 0;

#elif DARKEDIF_ACE_CALL_TABLE_INDEX==2
		case 0:
			*((const TCHAR* *)&Result) = ext->GetKeyValue(*(const TCHAR* *)&Parameters[0]);
			break;
		case 1:
			*((int *)&Result) = ext->GetErrorCode();
			break;
		case 2:
			*((const TCHAR* *)&Result) = ext->GetErrorReason();
			break;
		case 3:
			*((const TCHAR* *)&Result) = ext->GetLangCode();
			break;

#else
	#error Incorrect DarkEdif ACE Call Table index
#endif
