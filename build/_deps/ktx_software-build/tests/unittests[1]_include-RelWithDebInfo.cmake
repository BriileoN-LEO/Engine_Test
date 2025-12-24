if(EXISTS "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/RelWithDebInfo/unittests.exe")
  if(NOT EXISTS "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/unittests[1]_tests-RelWithDebInfo.cmake" OR
     NOT "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/unittests[1]_tests-RelWithDebInfo.cmake" IS_NEWER_THAN "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/RelWithDebInfo/unittests.exe" OR
     NOT "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/unittests[1]_tests-RelWithDebInfo.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-4.1/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/RelWithDebInfo/unittests.exe]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests]==]
      TEST_EXTRA_ARGS [==[]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[unittest.]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[unittests_TESTS]==]
      CTEST_FILE [==[C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/unittests[1]_tests-RelWithDebInfo.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[20]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/unittests[1]_tests-RelWithDebInfo.cmake")
else()
  add_test(unittests_NOT_BUILT unittests_NOT_BUILT)
endif()
