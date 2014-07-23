###################################################
# Create test executables for every testclass (no coverage measurement) 
###################################################

find_package(GMock REQUIRED)
find_package(GTest REQUIRED)

if(GTEST_FOUND AND GMOCK_FOUND)
  include_directories(${GTEST_INCLUDE_DIRS}
                      ${GMOCK_INCLUDE_DIRS}
                      ./include
                      ${@PROJECT_NAME@_INCLUDE_DIRS})
    
  function(register_test TEST_FILE)
      get_filename_component(TEST_NAME ${TEST_FILE} NAME)
      message(STATUS "Adding " ${TEST_NAME})
      add_executable(${TEST_NAME} ${TEST_FILE}.cpp)
      target_link_libraries(${TEST_NAME} ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} ${@PROJECT_NAME@_LIBRARIES} ${@PROJECT_NAME@_EXT_LIBRARIES} ${ARGN})
      gtest_add_tests(${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME} "" ${TEST_FILE}.cpp)
      set(@PROJECT_NAME@_UNITTEST_SOURCES ${@PROJECT_NAME@_UNITTEST_SOURCES} ${TEST_FILE}.cpp PARENT_SCOPE)
  endfunction()

  register_test(src/tournament_test)
  register_test(src/date_test)
  register_test(src/karateka_test)

  if(USE_STATIC_BOOST)
    set(Boost_USE_STATIC_LIBS ON)
  else(USE_STATIC_BOOST)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK")
  endif(USE_STATIC_BOOST)

  find_package(Boost COMPONENTS filesystem system)

  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    register_test(src/persistency_test ${Boost_LIBRARIES})
  endif(Boost_FOUND)
endif(GTEST_FOUND AND GMOCK_FOUND)  


###################################################
# Create a single test executable including all testclass.                     
# With coverage measurement
###################################################

set(${PROJECT_NAME}_UNITTEST_NAME 
  unittests
)

if(GMOCK_FOUND AND GTEST_FOUND)
  set(${PROJECT_NAME}_UNITTEST_LIBRARIES 
    ${GTEST_BOTH_LIBRARIES} 
    ${GMOCK_BOTH_LIBRARIES} 
    ${@PROJECT_NAME@_LIBRARIES}
    ${@PROJECT_NAME@_EXT_LIBRARIES})
  
  if(UNIX)
    find_package(Threads) # GTest needs this and it's a static library
    set(${PROJECT_NAME}_UNITTEST_LIBRARIES 
      ${@PROJECT_NAME@_UNITTEST_LIBRARIES} 
      ${CMAKE_THREAD_LIBS_INIT})
  endif(UNIX)

  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
  endif(Boost_FOUND)

  include_directories(${GTEST_INCLUDE_DIRS}
                      ${GMOCK_INCLUDE_DIRS}
                      ./include
                      ${@PROJECT_NAME@_INCLUDE_DIRS})
  add_executable(${@PROJECT_NAME@_UNITTEST_NAME} ${@PROJECT_NAME@_UNITTEST_SOURCES})
  target_link_libraries(${@PROJECT_NAME@_UNITTEST_NAME} ${@PROJECT_NAME@_UNITTEST_LIBRARIES} )

  if(Boost_FOUND)
    target_link_libraries(${@PROJECT_NAME@_UNITTEST_NAME} ${Boost_LIBRARIES})
  endif(Boost_FOUND)

  # Coverage
  if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)
    include(CodeCoverage)
    set(${PROJECT_NAME}_UNITTEST_LIBRARIES_COVERAGE 
      ${GTEST_BOTH_LIBRARIES}   
      ${GMOCK_BOTH_LIBRARIES} 
      ${@PROJECT_NAME@_LIBRARIES}_coverage)

    add_executable(${@PROJECT_NAME@_UNITTEST_NAME}_coverage  
      EXCLUDE_FROM_ALL 
      ${@PROJECT_NAME@_UNITTEST_SOURCES})
    target_link_libraries(${@PROJECT_NAME@_UNITTEST_NAME}_coverage ${@PROJECT_NAME@_UNITTEST_LIBRARIES_COVERAGE} )

    if(Boost_FOUND)
      target_link_libraries(${@PROJECT_NAME@_UNITTEST_NAME}_coverage ${Boost_LIBRARIES})
    endif(Boost_FOUND)

    setup_target_for_coverage_cobertura(coverage_tests 
	${@PROJECT_NAME@_UNITTEST_NAME}_coverage 
      	${COVERAGE_REPORT_DIR}/unittests
        "--gtest_output='xml:${TESTS_REPORT_DIR}/unittests.xml'"
	"-ggcov-${GCC_VERSION}"
    )
  endif(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)
endif(GMOCK_FOUND AND GTEST_FOUND)  



    
    
