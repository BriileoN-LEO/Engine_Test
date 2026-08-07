#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include "dataManager/dataTypes_brii.h" 
#include <condition_variable>
#include <thread>
#include <vector>

namespace thread_core
{

template <funcPtr::ptr_func_only T_funcPtr>  ///DETERMINATE IF THE TYPE IS FUNCTION POINTER FOR THE THREAD POOL | IF IS NOT THROWS COMPILATION ERROR
class threadPool_System
{
 private:
 std::atomic_bool done;
 
 std::vector<T_funcPtr> threads;

 public:


};

}
#endif //THREAD_MANAGER_H

