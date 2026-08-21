//
// Created by brii on 17/06/26.
//

#ifndef LIBS_DATATYPES_BRII_H
#define LIBS_DATATYPES_BRII_H
#include <iostream>
#include <type_traits>
#include <concepts>

//namespace flags_T
//{
 template<typename T>
 constexpr bool is_bitmask_flag = false;

//WAY OF USE----------------------------------------------------------------- 
//template<> constexpr bool is_bitmask_flag<[name_enumClass]> = true | false;
//---------------------------------------------------------------------------

 template<typename T, typename = std::enable_if_t<is_bitmask_flag<T>>>
 constexpr T operator|(T f1, T f2)
 {
  using underlying_T = std::underlying_type_t<T>; ////std::underlying_type_t -- specified for C++14, -- used to determinied the type o an enum, it quits the enum type and returns the real value determinied for the enum class, for exameple if a enum class is uint8_t, it returns this real type of valuoe
  return static_cast<T>(static_cast<underlying_T>(f1) | static_cast<underlying_T>(f2)); // real numeric operation thanks to (underlaying_T)
 }

 template<typename T, typename = std::enable_if_t<is_bitmask_flag<T>>>
 constexpr T operator&(T f1, T f2)
 {
  using underlying_T = std::underlying_type_t<T>;
  return static_cast<T>(static_cast<underlying_T>(f1) & static_cast<underlying_T>(f2));
 }
//}

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

namespace funcPtr
{
   namespace empty_func
  {
   void str_constStrAmp(std::string f1, const std::string& f2);
  }

  ///NEEDS TO SATISFY THAT THE TYPE IS FUNCTION POINTER
  template <typename T>
  constexpr bool is_func_ptr = std::is_pointer<T>::value && std::is_function<typename std::remove_pointer<T>::type>::value;
  
  //USED TO DETERMINADE IF THE TEMPLATE OF FUNCTION OR CLASS IS DETERMINATED VALUE TYPE, IN THIS CASE NEEDS TO BE A FUNCTION POINTER
  template <typename T>
  concept ptr_func_only = is_func_ptr<T>;  


  using funcDouble_uint64_t = void(*)(uint64_t&, uint64_t&);
  using func_Double_str_constUint = void(*)(std::string&, const std::string&, const unsigned int&);

  using func_str_constStrAmp = void(*) (std::string, const std::string&);
  using func_strDoubleAmp_strAmp = void(*)(std::string&&, const std::string&);
   // using func_lambdaFunc_void = void(*)(void(*)(void*), void*); /// (function, data) --> void    ///////////USE IN THE FUTURE
}

#endif //THIRDPARTY_LIBS_DATATYPES_BRII_H
