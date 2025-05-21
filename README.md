# SMG2-Byaml
A port of the BYAML file format to Super Mario Galaxy 2.

# Why?
I was bored.

# Usage
Usage is simple. a ByamlIter is created when you want to iterate through an array or dictionary of BYAML nodes. Not all types are supported yet.
<pre>
ByamlIter iter(dataPtr);
ByamlIter objList;
bool success = iter.tryGetIterByKey(&objList, "ObjectList");

for (int i = 0; i < objList.getSize(); i++) {
    ByamlIter obj = objList.getIterByIndex(i);
    const char* objID = "";
    obj.tryGetStringByKey(&objID, "Id");

    if (obj.isExistKey("IsBgmMute")) {
        bool isBgmMute = false;
        obj.tryGetBoolByKey(&isBgmMute, "IsBgmMute");
    }

    if (obj.isExistKey("IsLinkDest")) {
        bool isLinkDest = false;
        obj.tryGetBoolByKey(&isLinkDest, "IsLinkDest");
    }

    const char* unitConfig = "";
    obj.tryGetStringByKey(&unitConfig, "UnitConfigName");
}
</pre>