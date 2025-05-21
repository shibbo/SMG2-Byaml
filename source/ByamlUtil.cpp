#include "ByamlUtil.h"
#include "ByamlHeader.h"
#include "revolution.h"

namespace MR {
    bool verifiByamlHeader(const u8 *pData) {
        const ByamlHeader* header = reinterpret_cast<const ByamlHeader*>(pData);
        return header->getTag() == 0x4259 && (u32)(header->getVersion() - 1) < 3;
    }

    ByamlStringTableIter getHashKeyTable(const u8* data) {
        const ByamlHeader* header = reinterpret_cast<const ByamlHeader*>(data);
        s32 off = header->getHashKeyTableOffset();
        if (off == 0)
            return ByamlStringTableIter();
        return ByamlStringTableIter(&data[off]);
    }

    ByamlStringTableIter getStringTable(const u8* data) {
        const ByamlHeader* header = reinterpret_cast<const ByamlHeader*>(data);
        s32 off = header->getStringTableOffset();
        if (off == 0)
            return ByamlStringTableIter();
        return ByamlStringTableIter(&data[off]);
    }
};