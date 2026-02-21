//
// Created by brii on 16/02/26.
//

#include "optimizeAlgorithmics.h"

namespace FNV {
    constexpr uint32_t FNV_offset_basis {2166136261u};
    constexpr uint32_t FNV_prime{16777619u};

    constexpr uint32_t str_to_hash(std::string_view str)
    {
        uint32_t hash {FNV_offset_basis};

        for (auto& c : str)
        {
            hash ^= static_cast<uint32_t>(c);
            hash *= FNV_prime;
        }

        return hash;
    }

    constexpr uint32_t operator"" _FNV_hash(const char* str, size_t len)
    {
      return str_to_hash(std::string_view(str, len));
    }

}