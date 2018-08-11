#pragma once
#include <switch.h>
#include <cstdio>

#include "ldr_nso.hpp"

class PatchUtils {  
    public:
        static void ApplyPatches(const char *target, const NsoUtils::NsoHeader *header, u8 *mapped_nso, size_t size);
};