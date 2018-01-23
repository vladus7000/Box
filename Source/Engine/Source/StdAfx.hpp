//Engine StdAfx
#pragma once

#include "System\Assert.hpp"
#include "System\Logger.hpp"
#include "System\Types.hpp"

//TOTO: move
#define RELEASE(x) if (x) { x->Release(); x = nullptr; }
