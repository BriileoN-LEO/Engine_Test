//
// Created by brii on 17/06/26.
//

#include "dataTypes_brii.h"

namespace briT
{
    br_4::br_4(){};
    br_4::br_4(float x, float y, float z, float w)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = w;
    }
    br_4::br_4(float all)
    {
        data[0] = all;
        data[1] = all;
        data[2] = all;
        data[3] = all;
    }


    br_4 br_4::operator=(const br_4& op)
    {
        data[0] = op.data[0];
        data[1] = op.data[1];
        data[2] = op.data[2];
        data[3] = op.data[3];
    }
}