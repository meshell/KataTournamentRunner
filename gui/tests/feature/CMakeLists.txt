#
# Create a single feature test executable
#

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Test_DEFINITIONS})

set(${PROJECT_NAME}_UI_BDDTEST_NAME 
  gui_features
)

#
# Boost
#
set(CORE_BOOST_LIBS thread system regex date_time chrono)

if(USE_STATIC_BOOST)
  set(Boost_USE_STATIC_LIBS ON)
else(USE_STATIC_BOOST)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK")
endif(USE_STATIC_BOOST)


find_package(Boost COMPONENTS  ${CORE_BOOST_LIBS} REQUIRED)

#
# GoogleMock driver
#
find_package(GTest REQUIRED)

#
# cucumber-cpp
#
find_package(Cuke REQUIRED)

function(register_feature_steps STEP_DEFINITION_FILE)
    get_filename_component(STEP_DEFINITION_FILE_NAME ${STEP_DEFINITION_FILE} NAME)
    message(STATUS "Adding " ${STEP_DEFINITION_FILE})
    # In order to generate step definitions from multiple source files
    # we need to use set CUKE_OBJECT_PREFIX variable for every
    # source file to compile
    # https://github.com/cucumber/cucumber-cpp/issues/36
    string(REPLACE "-" "" OBJ_PREFIX ${STEP_DEFINITION_FILE_NAME})
    set_source_files_properties(${STEP_DEFINITION_FILE}.cpp PROPERTIES COMPILE_FLAGS "-DCUKE_OBJECT_PREFIX=Obj${OBJ_PREFIX}")
    set(STEP_SOURCES ${STEP_SOURCES} ${STEP_DEFINITION_FILE}.cpp PARENT_SCOPE)
endfunction()

if(CUKE_FOUND)
  register_feature_steps(features/step_definitions/gui_steps)
endif(CUKE_FOUND)

# Find feature files

# Find all feature files
FILE(GLOB_RECURSE FEATURE_FILES features *.feature)


if(STEP_SOURCES)

  qt5_wrap_ui(MAIN_WINDOW_GUI_FORM_HEADER ../../ui/mainwindow.ui)

  include_directories(
    ${@PROJECT_NAME@_INCLUDE_DIRS} 
    ${GTEST_INCLUDE_DIRS}
    ${CUKE_INCLUDE_DIR} 
    ${Boost_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDES}
    ${Qt5Test_INCLUDE})
  
  set(${PROJECT_NAME}_UI_FEATURETEST_LIBRARIES 
    ${PROJECT_NAME}_GUI-library
    ${GTEST_LIBRARIES}
    ${CUKE_LIBRARIES} 
    ${Boost_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Test_LIBRARIES})
  
  add_executable(${@PROJECT_NAME@_UI_BDDTEST_NAME} 
    ${STEP_SOURCES}
    ${FEATURE_FILES}
    ${MAIN_WINDOW_GUI_FORM_HEADER}
  )
  target_link_libraries(${@PROJECT_NAME@_UI_BDDTEST_NAME} ${@PROJECT_NAME@_UI_FEATURETEST_LIBRARIES})    

  if(NOT MSVC)
    target_link_libraries(${@PROJECT_NAME@_UI_BDDTEST_NAME} pthread rt)
  endif(NOT MSVC)

  # Coverage
  if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)
    include(CodeCoverage)
  
    set(${PROJECT_NAME}_UI_FEATURETEST_LIBRARIES_COVERAGE
      ${PROJECT_NAME}_GUI-library_coverage
      ${GTEST_LIBRARIES}
      ${CUKE_LIBRARIES} 
      ${Boost_LIBRARIES}
      ${Qt5Widgets_LIBRARIES}
      ${Qt5Test_LIBRARIES})

    add_executable(${@PROJECT_NAME@_UI_BDDTEST_NAME}_coverage  
      EXCLUDE_FROM_ALL 
      ${STEP_SOURCES})
    
    target_link_libraries(${@PROJECT_NAME@_UI_BDDTEST_NAME}_coverage ${@PROJECT_NAME@_UI_FEATURETEST_LIBRARIES_COVERAGE} pthread rt)

    setup_target_under_cucumber_for_coverage_cobertura(
      coverage_gui_features 
      ${@PROJECT_NAME@_UI_BDDTEST_NAME}_coverage
      ${COVERAGE_REPORT_DIR}/gui-features
      ${PROJECT_SOURCE_DIR}/gui/tests/feature/features
      ""
      "-ggcov-${GCC_VERSION}"
    )
  endif(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)
endif(STEP_SOURCES)


