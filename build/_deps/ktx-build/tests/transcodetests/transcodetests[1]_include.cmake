if(EXISTS "/home/brii/CPP_proyects/Engine_Test/build/Release/transcodetests")
  if(NOT EXISTS "/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/transcodetests/transcodetests[1]_tests.cmake" OR
     NOT "/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/transcodetests/transcodetests[1]_tests.cmake" IS_NEWER_THAN "/home/brii/CPP_proyects/Engine_Test/build/Release/transcodetests" OR
     NOT "/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/transcodetests/transcodetests[1]_tests.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("/usr/share/cmake/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[/home/brii/CPP_proyects/Engine_Test/build/Release/transcodetests]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/transcodetests]==]
      TEST_EXTRA_ARGS [==[/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-src/tests/resources/]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[transcodetest.]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[transcodetests_TESTS]==]
      CTEST_FILE [==[/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/transcodetests/transcodetests[1]_tests.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[15]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/transcodetests/transcodetests[1]_tests.cmake")
else()
  add_test(transcodetests_NOT_BUILT transcodetests_NOT_BUILT)
endif()
