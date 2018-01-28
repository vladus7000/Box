//Engine StdAfx
#pragma once

#include "System\Assert.hpp"
#include "System\Logger.hpp"
#include "System\Types.hpp"

//TOTO: move
#define SAVE_RELEASE(x) if (x) { x->Release(); x = nullptr; }
