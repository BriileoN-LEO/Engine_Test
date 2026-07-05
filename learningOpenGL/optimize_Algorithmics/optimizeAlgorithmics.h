#ifndef OPTIMIZE_ALGORITHMICS
#define OPTIMIZE_ALGORITHMICS

#include <iostream>

namespace FNV
{
 constexpr uint32_t FNV_offset_basis {2166136261u};
 constexpr uint32_t FNV_prime{16777619u};

 constexpr uint64_t FNV_1a_prime {0x100000001b3ULL};
 constexpr uint64_t FNV_1a_offset_basis{0xcbf29ce484222325ULL};

 constexpr std::string_view trim_str(std::string_view str)
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

 inline constexpr uint64_t hash_1a(std::string_view str)
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

 inline constexpr uint32_t str_to_hash(std::string_view str)
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
 constexpr uint32_t operator""_FNV_hash(const char* str, size_t len)
 {
  return str_to_hash(std::string(str));
 }

}



#endif //OPTIMIZE_ALGORITHMICS
