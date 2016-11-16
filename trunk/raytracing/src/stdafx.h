#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX // To avoid min-max-keyword collisions in WinApi-headers
#endif

#include <vectorization/vectorization.h>
using namespace vectorization;

#include <primitives/primitives.h>
using namespace primitives;
