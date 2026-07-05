if(EXISTS "/home/brii/CPP_proyects/Engine_Test/build/Release/unittests")
  if(NOT EXISTS "/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/unittests[1]_tests.cmake" OR
     NOT "/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/unittests[1]_tests.cmake" IS_NEWER_THAN "/home/brii/CPP_proyects/Engine_Test/build/Release/unittests" OR
     NOT "/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/unittests[1]_tests.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("/usr/share/cmake/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[/home/brii/CPP_proyects/Engine_Test/build/Release/unittests]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests]==]
      TEST_EXTRA_ARGS [==[]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[unittest.]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[unittests_TESTS]==]
      CTEST_FILE [==[/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/unittests[1]_tests.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[20]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/unittests[1]_tests.cmake")
else()
  add_test(unittests_NOT_BUILT unittests_NOT_BUILT)
endif()
