#include "MiniginPCH.h"
#include "Locator.h"

NFE::Audio* NFE::Locator::service_ = nullptr;
NFE::NullAudio NFE::Locator::nullService_{};