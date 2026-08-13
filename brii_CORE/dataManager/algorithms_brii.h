//
// Created by brii on 28/06/26.
//

#ifndef ALGORITHMS_BRII_H
#define ALGORITHMS_BRII_H
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

namespace search_algorithms
{
  template <typename T_arg>
  int binary_search_branchless(const T_arg* container, size_t size_c, T_arg& target)
  {
   if(size_c > 0)
   {
    int base{};
    int n {static_cast<int>(size_c)};
 
      while (n > 1)
      {
          int half {n / 2};
          base = (container[base + half - 1] < target) ? base + half : base;
          n -= half;
      }

      return (container[base] == target) ? base : -1;
    }

   return -1;
  }

  template <typename T_arg>
  int binary_search_std(const T_arg* container, size_t size_c, T_arg& target)
  {
     int low = 0;
     int high = static_cast<int>(size_c) - 1;

     while (low <= high)
     {
       int mid = low + ((high - low) / 2);

       if (container[mid] == target)
       {
        return mid;
       }

       else if (container[mid] < target)
       {
        low = mid + 1;
       }

       else
       {
        high = mid - 1;
       }
     }

    return -1;
  }

  template <typename T_arg>
  using binSearch_func = int(*)(const T_arg*, size_t, T_arg&);

  template <typename T_arg>
  auto binary_search_OP_int(std::vector<T_arg>& container, T_arg& target)
  {
    std::vector<binSearch_func<T_arg>> funcBin{};
    funcBin.resize(2);
    funcBin[0] = &binary_search_branchless<T_arg>;
    funcBin[1] = &binary_search_std<T_arg>;

   //size_t size_E {container.size() * sizeof(T_arg)};
   size_t size_E {container.size()};

   int func_p {(size_E <= 24000) ? 0 : 1};

   return funcBin[func_p](container.data(), container.size(), target);
  }

  template <typename T_arg>
  uint32_t binary_search_contains(std::vector<T_arg>& container, T_arg& target)
  {
   return (binary_search_OP_int(container, target)) != -1 ? 1 : 0;
  }

  template <typename T_arg>
  bool contains_vecBinarySearch(std::vector<T_arg>& container, T_arg& target)
  {
   return (binary_search_OP_int(container, target) != -1) ? true : false;
  }

}



#endif //THIRDPARTY_LIBS_ALGORITHMS_BRII_H
