#
# Build library
#

set(${PROJECT_NAME}_SOURCES
  tournament.cpp
  karateka.cpp
  date.cpp
  persistency.cpp
  kataround_scores.cpp
)

if(WIN32)
  set(${PROJECT_NAME}_SOURCES
    ${@PROJECT_NAME@_SOURCES}
    os_helper_win.cpp
  )
else(WIN32)
  set(${PROJECT_NAME}_SOURCES
    ${@PROJECT_NAME@_SOURCES}
    os_helper.cpp
  )
endif(WIN32)

#
# Boost
#
set(CORE_BOOST_LIBS date_time serialization system filesystem)

if(USE_STATIC_BOOST)
  set(Boost_USE_STATIC_LIBS ON)
else(USE_STATIC_BOOST)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK")
endif(USE_STATIC_BOOST)

find_package(Boost COMPONENTS ${CORE_BOOST_LIBS} REQUIRED)

set(${PROJECT_NAME}_EXT_LIBRARIES ${Boost_LIBRARIES})

if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wextra -pedantic")
endif(CMAKE_COMPILER_IS_GNUCXX)


# To many errors with boost -> disable
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-disabled-macro-expansion")
#endif()

add_library(${@PROJECT_NAME@_LIBRARY}
	STATIC  
	${@PROJECT_NAME@_SOURCES})

  include_directories(
    ${@PROJECT_NAME@_INCLUDE_DIRS} 
    ${Boost_INCLUDE_DIRS})   

target_link_libraries(${@PROJECT_NAME@_LIBRARY} ${@PROJECT_NAME@_EXT_LIBRARIES})


#
# Build library with coverage support
#

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)
  include(CodeCoverage)
  
  add_library(${@PROJECT_NAME@_LIBRARY}_coverage
	STATIC
        ${@PROJECT_NAME@_SOURCES}
  )
  target_link_libraries(${@PROJECT_NAME@_LIBRARY}_coverage ${@PROJECT_NAME@_EXT_LIBRARIES})
 
  set_target_properties(${@PROJECT_NAME@_LIBRARY}_coverage PROPERTIES
        COMPILE_FLAGS "-fprofile-arcs -ftest-coverage"
  )
endif(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)
