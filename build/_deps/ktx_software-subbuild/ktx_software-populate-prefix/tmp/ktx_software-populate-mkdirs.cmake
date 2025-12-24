# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-src")
  file(MAKE_DIRECTORY "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-build"
  "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-subbuild/ktx_software-populate-prefix"
  "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-subbuild/ktx_software-populate-prefix/tmp"
  "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-subbuild/ktx_software-populate-prefix/src/ktx_software-populate-stamp"
  "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-subbuild/ktx_software-populate-prefix/src"
  "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-subbuild/ktx_software-populate-prefix/src/ktx_software-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-subbuild/ktx_software-populate-prefix/src/ktx_software-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/DiegoYael/source/repos/BriileoN-LEO/Engine_Test/build/_deps/ktx_software-subbuild/ktx_software-populate-prefix/src/ktx_software-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
