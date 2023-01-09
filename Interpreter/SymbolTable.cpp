#include "SymbolTable.h"

std::string SymbolTable::ToStr()
{
    std::string res;
    res += "******************************\n";
    for (auto it = table.begin(); it != table.end(); ++it) {
        res += (" " + (it->first) + "\t, " + (it->second).ToStr() + "\n");
    }
    res += "******************************\n";
    return res;
}
