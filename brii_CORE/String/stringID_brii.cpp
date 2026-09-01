
#include "stringID_brii.h"

namespace Brii_StringFNV
{
 constexpr BT_StringView trim_str(BT_StringView str)
 {
  const char* characters_ws {" \t\n\r\f\v\0"};
  size_t start {str.find_first_not_of(characters_ws)};

  if (start == std::string_view::npos)
  {
   return "";
  }

  size_t end{str.find_last_not_of(characters_ws)};

  return str.substr(start, end - start + 1);
 }

 inline constexpr BT_StringID_64 hash_1a(BT_StringView str)
 {
   uint64_t hash {FNV_1a_offset_basis};

   const char* str_s { str.data() };

   while (*str_s)
   {
     hash ^= static_cast<uint64_t>(*str_s++);
     hash *= FNV_1a_prime;
   }

   return hash;
 }

 inline constexpr BT_StringID_32 str_to_hash(BT_StringView str)
 {
  std::string_view str_clean {trim_str(str)}; //////////CONTINUE HERE, SEE WHY THROW ME DIFERENT ID WITH THE SAME STRING

  uint32_t hash {FNV_offset_basis};

  for (auto& c : str_clean)
  {
   hash ^= static_cast<uint32_t>(c);
   hash *= FNV_prime;
  }

  return hash;
 }

 constexpr BT_StringID_32 operator""_FNV_hash(const char* str, size_t len)
 {
  [[maybe_unused]] size_t lee {len};
  return str_to_hash(std::string(str));
 }

 
////CONTINUE HERE 31/08/2026
///Create the string manager to create a hash table saved in the RAM.
//Investigate the CSV files and how to inyect them into the the string manager.
//Write about this files in the documentation
//Continue with the function of the creation of windows inside the SDL3_window.cpp

//tasks from 01/08/2026
///Write the fileSystem to encapsulate the I/O for each Operating System



}
