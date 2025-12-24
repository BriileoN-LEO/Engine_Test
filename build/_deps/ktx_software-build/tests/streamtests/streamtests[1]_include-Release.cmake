if(EXISTS "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/Release/streamtests.exe")
  if(NOT EXISTS "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/streamtests/streamtests[1]_tests-Release.cmake" OR
     NOT "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/streamtests/streamtests[1]_tests-Release.cmake" IS_NEWER_THAN "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/Release/streamtests.exe" OR
     NOT "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/streamtests/streamtests[1]_tests-Release.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-4.1/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/Release/streamtests.exe]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/streamtests]==]
      TEST_EXTRA_ARGS [==[C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-src/tests/testimages/]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[streamtest.]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[streamtests_TESTS]==]
      CTEST_FILE [==[C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/streamtests/streamtests[1]_tests-Release.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[20]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build/tests/streamtests/streamtests[1]_tests-Release.cmake")
else()
  add_test(streamtests_NOT_BUILT streamtests_NOT_BUILT)
endif()
