// stdafx.h: Includedatei f�r Standardsystem-Includedateien
// oder h�ufig verwendete projektspezifische Includedateien,
// die nur in unregelm��igen Abst�nden ge�ndert werden.
//

#pragma once

#include "targetver.h"

// Header f�r CppUnitTest
#include <CppUnitTest.h>

#include <vectorization.h>

#define ASSERT_MSG(msg) const wchar_t * const MESSAGE = L#msg;
