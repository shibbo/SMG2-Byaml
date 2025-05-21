#pragma once

#include "revolution.h"
#include "ByamlHeader.h"

namespace MR {
    bool verifiByamlHeader(const u8 *);
    bool verifiByaml(const u8 *);

    ByamlStringTableIter getHashKeyTable(const u8* data);
    ByamlStringTableIter getStringTable(const u8* data);
};