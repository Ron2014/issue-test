#pragma once
#include "macro.h"

struct ABC {
public:
    enum TYPE {
        Walkable,
        Walkable_Wall,
        Walkable_Grass,
        TypeCount,
    };

    static const int type_chtype[ABC::TypeCount];

    static int GetChtype(int type) {
        if (type < 0 || type >= ABC::TypeCount)
            return 0;
        return type_chtype[type];
    }
};