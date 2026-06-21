//
// Created by brii on 17/06/26.
//

#ifndef LIBS_DATATYPES_BRII_H
#define LIBS_DATATYPES_BRII_H

namespace briT
{
 struct br_4
 {
   union
   {
      float data[4]{};

      struct
      {
        float x;
        float y;
        float z;
        float w;
      };

      struct
      {
         float r;
         float g;
         float b;
         float a;
      };
   };

    br_4();
    br_4(float x, float y, float z, float w);
    br_4(float all);

    br_4 operator=(const br_4& op);
 };
}


#endif //THIRDPARTY_LIBS_DATATYPES_BRII_H