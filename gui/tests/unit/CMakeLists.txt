###################################################
# Create test executables for every testclass (no coverage measurement) 
###################################################


# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Test_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)


find_package(GMock REQUIRED)
find_package(GTest REQUIRED)

if(GTEST_FOUND AND GMOCK_FOUND)
  include_directories(SYSTEM
                      ${GTEST_INCLUDE_DIRS}
                      ${GMOCK_INCLUDE_DIRS} 
                      ${Qt5Widgets_INCLUDES} 
                      ${Qt5Test_INCLUDE}
                      ./include
                      )
                      
  function(register_test TEST_FILE)
      get_filename_component(TEST_NAME ${TEST_FILE} NAME)
      message(STATUS "Adding " ${TEST_NAME})
      add_executable(${TEST_NAME} ${TEST_FILE}.cpp src/qttest_main.cpp)
      target_link_libraries(${TEST_NAME} ${GTEST_LIBRARY}
                                         ${GMOCK_LIBRARY}
                                         ${PROJECT_NAME}_GUI-library
                                         ${Qt5Widgets_LIBRARIES} 
                                         ${Qt5Test_LIBRARIES}
                                         ${ARGN})
  
      gtest_add_tests(${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME} "" ${TEST_FILE}.cpp)
      set(@PROJECT_NAME@_UI_UNITTEST_SOURCES ${@PROJECT_NAME@_UI_UNITTEST_SOURCES} ${TEST_FILE}.cpp PARENT_SCOPE)
  endfunction()

  register_test(src/mainwindow_test)
  register_test(src/participant_model_test)
  register_test(src/kata_round_participant_model_test)
  register_test(src/participant_kata_round_WizardPage_test)
  register_test(src/kataround_overview_frame_test)
endif(GTEST_FOUND AND GMOCK_FOUND)


###################################################
# Create a single test executable including all testclass.                     
# With coverage measurement
###################################################

set(${PROJECT_NAME}_UI_UNITTEST_NAME 
  gui_unittests
)

set(@PROJECT_NAME@_UI_UNITTEST_SOURCES ${@PROJECT_NAME@_UI_UNITTEST_SOURCES} src/qttest_main.cpp)

if(GMOCK_FOUND AND GTEST_FOUND)
 
  if(UNIX)
    find_package(Threads) # GTest needs this and it's a static library
    set(${PROJECT_NAME}_UI_UNITTEST_LIBRARIES 
      ${@PROJECT_NAME@_UI_UNITTEST_LIBRARIES} 
      ${CMAKE_THREAD_LIBS_INIT})
  endif(UNIX)  
  
  add_executable(${@PROJECT_NAME@_UI_UNITTEST_NAME} ${@PROJECT_NAME@_UI_UNITTEST_SOURCES})
  target_link_libraries(${@PROJECT_NAME@_UI_UNITTEST_NAME} 
                        ${GTEST_LIBRARY}
                        ${GMOCK_LIBRARY}
                        ${PROJECT_NAME}_GUI-library
                        ${Qt5Widgets_LIBRARIES}
                        ${Qt5Test_LIBRARIES})
  
  # Coverage
  if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)
    include(CodeCoverage)
    
    add_executable(${@PROJECT_NAME@_UI_UNITTEST_NAME}_coverage  
                   EXCLUDE_FROM_ALL 
                   ${@PROJECT_NAME@_UI_UNITTEST_SOURCES})
      
    target_link_libraries(${@PROJECT_NAME@_UI_UNITTEST_NAME}_coverage 
                          ${GTEST_BOTH_LIBRARIES} 
                          ${GMOCK_BOTH_LIBRARIES} 
                          ${PROJECT_NAME}_GUI-library_coverage
                          ${Qt5Widgets_LIBRARIES}
                          ${Qt5Test_LIBRARIES})
                          
    setup_target_for_coverage_cobertura(coverage_gui_unittests
	                                    ${@PROJECT_NAME@_UI_UNITTEST_NAME}_coverage 
      	                                ${COVERAGE_REPORT_DIR}/ui_unittests
                                        "--gtest_output='xml:${TESTS_REPORT_DIR}/ui_unittests.xml'"
	                                    "-ggcov-${GCC_VERSION}")
  endif(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)
endif(GMOCK_FOUND AND GTEST_FOUND)  



    
    
