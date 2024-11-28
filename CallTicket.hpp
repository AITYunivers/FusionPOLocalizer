#pragma once
#include <CallType.hpp>
#include <string>

struct CallTicket
{
	CallType Type;
	int Trigger = 0;
	bool HasTrigger = false;
};
