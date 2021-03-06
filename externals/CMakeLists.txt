# CMake file to build the external dependencies

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

include(ExternalProject)

# create output directories
file(MAKE_DIRECTORY
  ${PROJECT_SOURCE_DIR}/externals/lib
  ${PROJECT_SOURCE_DIR}/externals/include
)

if(MSVC)
  set(LIB_DIR "Debug")
  set(SCRIPT_SUFFIX .bat)
else(MSVC)
  set(LIB_DIR "")
  set(SCRIPT_SUFFIX .sh)
endif(MSVC)


check_cxx_compiler_flag("-Wunused-local-typedefs" COMPILER_KNOWS_UNUSED_LOCAL_TYPEDEF)
if(COMPILER_KNOWS_UNUSED_LOCAL_TYPEDEF)
  set(CXX_FLAGS "-Wno-unused-local-typedefs")
endif(COMPILER_KNOWS_UNUSED_LOCAL_TYPEDEF)

if(CMAKE_COMPILER_IS_CLANGCXX)
  set(CXX_FLAGS "${CXX_FLAGS} -std=c++11 -stdlib=libc++")
endif(CMAKE_COMPILER_IS_CLANGCXX)

####################
# Google Mock
####################
ExternalProject_Add(
  googlemock
  INSTALL_DIR ${CMAKE_SOURCE_DIR}/externals
  URL http://googlemock.googlecode.com/files/gmock-1.7.0.zip
  URL_MD5 073b984d8798ea1594f5e44d85b20d66
  UPDATE_COMMAND ""
  CMAKE_ARGS
    -DCMAKE_PREFIX_PATH=${CMAKE_SOURCE_DIR}/externals
    -DCMAKE_CXX_FLAGS=${CXX_FLAGS}
    -DCMAKE_MODULE_PATH=${PROJECT_SOURCE_DIR}/cmake/
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -Dgtest_force_shared_crt=${MSVC}
    -DBUILD_SHARED_LIBS=OFF
  INSTALL_COMMAND
    cp -rf <SOURCE_DIR>/gtest/include/gtest <INSTALL_DIR>/include &&
    cp -rf <SOURCE_DIR>/include/gmock <INSTALL_DIR>/include &&
    cp -f <BINARY_DIR>/${LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX} <INSTALL_DIR>/lib/ &&
    cp -f <BINARY_DIR>/gtest/${LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX} <INSTALL_DIR>/lib/ &&
    cp -f <BINARY_DIR>/${LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gmock_main${CMAKE_STATIC_LIBRARY_SUFFIX} <INSTALL_DIR>/lib/  &&
    cp -f <BINARY_DIR>/gtest/${LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX} <INSTALL_DIR>/lib/
  )

# override GMock_LIBRARY_DIRS such that only the ones from external are used
ExternalProject_Get_Property(googlemock INSTALL_DIR)
set(GMOCK_LIBRARY_DIRS ${INSTALL_DIR})

####################
# Boost
####################

# Set boost minimal version in order to build cucumber-cpp with the same library versions
set(BOOST_MIN_VERSION 1.55)
set(Boost_Version ${BOOST_MIN_VERSION}.0)
set(Boost_MD5 d6eef4b4cacb2183f2bf265a5a03a354)

# cucumber-cpp needs filesystem, date_time, chrono, thread, system, regex and program_options
# library need date_time and serialization
set(Boost_Components filesystem date_time chrono thread system regex program_options serialization)
string( REPLACE "." "_" Boost_Version_Underscore ${Boost_Version} )


foreach(library ${Boost_Components})
  set(BOOST_LIBRARIES ${BOOST_LIBRARIES} --with-${library})
endforeach(library)

string(TOLOWER ${CMAKE_BUILD_TYPE} BOOST_VARIANT)
set(BOOST_THREADING multi)
set(BOOST_LAYOUT tagged)

if(MSVC)
  set(BOOST_LAYOUT versioned)
endif(MSVC)

set(BOOST_TOOLSET)
set(BOOST_TOOLSET_BUILD)

# Use the same compiler for building boost as for your own project
if(CMAKE_COMPILER_IS_GNUCXX)
  set(BOOST_TOOLSET "gcc")
  set(BOOST_TOOLSET_BUILD "toolset=gcc-${GCC_VERSION}")
  set(BOOST_CXX_FLAGS "-std=c++11")
  set(BOOST_LINK_FLAGS "")
elseif(CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(BOOST_TOOLSET "clang")
  set(BOOST_TOOLSET_BUILD "toolset=clang")
  set(BOOST_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wno-c99-extensions")
  set(LINK_FLAGS "-stdlib=libc++")
  set(BOOST_LINK_FLAGS "linkflags=${LINK_FLAGS}")
elseif(MSVC)
  if (NOT BOOST_TOOLSET)
    set(BOOST_TOOLSET "msvc")
  endif(NOT BOOST_TOOLSET)
endif(CMAKE_COMPILER_IS_GNUCXX)

ExternalProject_Add(
  boost
  URL http://downloads.sourceforge.net/project/boost/boost/${Boost_Version}/boost_${Boost_Version_Underscore}.tar.bz2
  URL_MD5 ${Boost_MD5}
  INSTALL_DIR ${CMAKE_SOURCE_DIR}/externals/
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ./bootstrap${SCRIPT_SUFFIX} --with-toolset=${BOOST_TOOLSET} --prefix=<INSTALL_DIR>
  BUILD_COMMAND ./b2 install --prefix=<INSTALL_DIR>
                    ${BOOST_LIBRARIES}
                    -j10
                    --build-type=complete
                    variant=${BOOST_VARIANT}
                    link=shared,static
                    threading=${BOOST_THREADING}
                    --layout=${BOOST_LAYOUT}
                    cxxflags=${BOOST_CXX_FLAGS}
                    ${BOOST_LINK_FLAGS}
                    ${BOOST_TOOLSET_BUILD}
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND ""
)

if(MSVC)
  string( REPLACE "_0" "" Boost_Version_Shorten ${Boost_Version_Underscore} )
  ExternalProject_Add_Step(
    boost MoveHeaders
    COMMAND mv ${CMAKE_SOURCE_DIR}/externals/include/boost-${Boost_Version_Shorten}/boost ${CMAKE_SOURCE_DIR}/externals/include/boost
    DEPENDEES install
  )
    
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
endif(MSVC)

find_package(Threads)

# set boost cmake variables such that only the ones from external are used
ExternalProject_Get_Property(boost INSTALL_DIR)
set(BOOST_ROOT ${INSTALL_DIR})
set(BOOST_ROOT ${INSTALL_DIR} PARENT_SCOPE)
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_NO_SYSTEM_PATHS ON PARENT_SCOPE)

find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS ${Boost_Components})

if(Boost_FOUND)
  set(CUKE_EXTERNAL_PROJ_DEPENDS)
  set(CUKE_DEPENDENCIES_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
  set(CUKE_DEPENDENCIES_LIBS_DIRS ${Boost_LIBRARY_DIRS})
  set(CUKE_DEPENDENCIES ${Boost_LIBRARIES} )
else(Boost_FOUND)
  set(CUKE_EXTERNAL_PROJ_DEPENDS boost)
endif(Boost_FOUND)

if(UNIX) 
  ####################
  # Cucumber-cpp
  ####################
  ExternalProject_Add(
    cucumber-cpp
    INSTALL_DIR ${CMAKE_SOURCE_DIR}/externals/
    GIT_REPOSITORY https://github.com/meshell/cucumber-cpp
    GIT_TAG gcc48
    DEPENDS ${CUKE_EXTERNAL_PROJ_DEPENDS}
    CMAKE_ARGS
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
      -DCMAKE_MODULE_PATH=${PROJECT_SOURCE_DIR}/cmake/
      -DCMAKE_PREFIX_PATH=${CMAKE_SOURCE_DIR}/externals
      -DCMAKE_CXX_FLAGS=${CXX_FLAGS}
      -DCUKE_USE_STATIC_BOOST=ON
      -DCUKE_DISABLE_BOOST_TEST=ON
      -DCUKE_DISABLE_GTEST=OFF
      -DCUKE_DISABLE_CPPSPEC=ON
      -DCUKE_ENABLE_EXAMPLES=OFF
      -DBUILD_SHARED_LIBS=ON
      -DCMAKE_MODULE_PATH=${PROJECT_SOURCE_DIR}/cmake/
    BUILD_COMMAND cmake --build <BINARY_DIR> --target cucumber-cpp
    UPDATE_COMMAND ""
    INSTALL_COMMAND
      cp -rf <SOURCE_DIR>/include/cucumber-cpp <INSTALL_DIR>/include &&
      cp -rf <SOURCE_DIR>/include/json_spirit <INSTALL_DIR>/include &&
      cp -f <BINARY_DIR>/src/${LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}cucumber-cpp${CMAKE_STATIC_LIBRARY_SUFFIX} <INSTALL_DIR>/lib/
  )
else(UNIX)  
  set(CUKE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
endif(UNIX)	    



