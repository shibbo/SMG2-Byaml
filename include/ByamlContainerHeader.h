#pragma once

#include "revolution.h"

class ByamlContainerHeader {
public:
    s32 getType() const;
    s32 getCount() const;

    u32 mType;
};