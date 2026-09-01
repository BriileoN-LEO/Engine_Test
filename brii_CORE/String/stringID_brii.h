#pragma once

#include "dataManager/dataTypes_brii.h"


struct Brii_StringID
{
  BT_StringID_64 stringID{}; 
};


namespace Brii_StringFNV
{
 constexpr uint32_t FNV_offset_basis {2166136261u};
 constexpr uint32_t FNV_prime{16777619u};

 constexpr uint64_t FNV_1a_prime {0x100000001b3ULL};
 constexpr uint64_t FNV_1a_offset_basis{0xcbf29ce484222325ULL};

 constexpr BT_StringView trim_str(BT_StringView str);

 inline constexpr BT_StringID_64 hash_1a(BT_StringView str);

 inline constexpr BT_StringID_32 str_to_hash(BT_StringView str);

 constexpr BT_StringID_32 operator""_FNV_hash(const char* str, size_t len);


}


