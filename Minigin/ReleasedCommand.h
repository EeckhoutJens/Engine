#pragma once
#include "Commands.h"
namespace NFE
{
	class ReleasedCommand final : public Command
	{
	public:
		ReleasedCommand();
		~ReleasedCommand() = default;
	};
}


