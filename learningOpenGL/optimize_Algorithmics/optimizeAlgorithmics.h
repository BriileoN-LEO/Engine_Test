//
// Created by brii on 16/02/26.
//

#ifndef OPTIMIZE_ALGORITHMICS
#define OPTIMIZE_ALGORITHMICS

#include <iostream>

namespace FNV
{
 // extern constexpr uint32_t FNV_offset_basis;
//  extern constexpr uint32_t FNV_prime;

 constexpr uint32_t str_to_hash(std::string_view str);
 constexpr uint32_t operator"" _FNV_hash(const char* str, size_t len);

}



#endif //LEARNING_OPTIMIZEALGORITHMICS_H