if(EXISTS "/home/brii/CPP_proyects/Engine_Test/build/Release/texturetests")
  if(NOT EXISTS "/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/texturetests[1]_tests.cmake" OR
     NOT "/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/texturetests[1]_tests.cmake" IS_NEWER_THAN "/home/brii/CPP_proyects/Engine_Test/build/Release/texturetests" OR
     NOT "/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/texturetests[1]_tests.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("/usr/share/cmake/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[/home/brii/CPP_proyects/Engine_Test/build/Release/texturetests]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests]==]
      TEST_EXTRA_ARGS [==[/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-src/tests/resources/;/home/brii/CPP_proyects/Engine_Test/build/Release/ktxdiff]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[texturetest.]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[texturetests_TESTS]==]
      CTEST_FILE [==[/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/texturetests[1]_tests.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[20]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("/home/brii/CPP_proyects/Engine_Test/build/_deps/ktx-build/tests/texturetests[1]_tests.cmake")
else()
  add_test(texturetests_NOT_BUILT texturetests_NOT_BUILT)
endif()
