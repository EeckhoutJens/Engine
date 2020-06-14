#pragma once
#include "Audio.h"
namespace NFE
{
	class Locator
	{
	public:
		static void Initialize() { service_ = &nullService_; }
		static Audio* GetAudio() { return service_; };
		static void Provide(Audio* Service)
		{
			if (Service == nullptr)
			{
				service_ = &nullService_;
			}
			else
			{
				service_ = Service;
			}
		}
	private:
		static Audio* service_;
		static NullAudio nullService_;
	};
}

