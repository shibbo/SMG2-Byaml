#include "ByamlContainerHeader.h"

s32 ByamlContainerHeader::getType() const {
    return mType;
}

s32 ByamlContainerHeader::getCount() const {
    return mType & 0xFFFFFF;
}