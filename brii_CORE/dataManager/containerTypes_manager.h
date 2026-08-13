#ifndef CONTAINER_TYPES_MANAGER_H
#define CONTAINER_TYPES_MANAGER_H
#include <iostream>
#include <vector>

namespace std_vectorManager
{
 template <typename T_arg>
  void insert_sorted_order(std::vector<T_arg>& container, T_arg value)
  {
     auto find_p {std::lower_bound(container.begin(), container.end(), value)};
     container.emplace(find_p, value); 
  }

 
 


}


#endif

