# cmake project file for the Karate Tournament Manager gui (Qt) project.

#
# Boost
#
set(CORE_BOOST_LIBS date_time serialization filesystem)

if(USE_STATIC_BOOST)
  set(Boost_USE_STATIC_LIBS ON)
else(USE_STATIC_BOOST)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK")
endif(USE_STATIC_BOOST)

find_package(Boost COMPONENTS ${CORE_BOOST_LIBS} REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(SYSTEM 
                    ${Qt5Widgets_INCLUDE_DIRS} 
                    ${Boost_INCLUDE_DIRS}
                    )
                    
include_directories(./include)

add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
  # Disable warnings due to same member names and in generated moc files
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shadow -Wno-undefined-reinterpret-cast")
endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)

set(${PROJECT_NAME}_GUI_SOURCES
  src/about_dialog.cpp
  src/mainwindow.cpp
  src/new_tournament_dialog.cpp
  src/edit_tournament_dialog.cpp
  src/mainframe.cpp
  src/finalframe.cpp
  src/kataround_overview_frame.cpp
  src/participant_model.cpp
  src/participant_ranking_model.cpp
  src/kata_round_participant_model.cpp
  src/add_participant_dialog.cpp
  src/participant_kata_round_WizardPage.cpp
)

set(${PROJECT_NAME}_GUI_HEADERS
  include/tournament_runner_gui/about_dialog.h
  include/tournament_runner_gui/mainwindow.h
  include/tournament_runner_gui/new_tournament_dialog.h
  include/tournament_runner_gui/edit_tournament_dialog.h
  include/tournament_runner_gui/mainframe.h
  include/tournament_runner_gui/finalframe.h
  include/tournament_runner_gui/kataround_overview_frame.h
  include/tournament_runner_gui/participant_model.h
  include/tournament_runner_gui/participant_ranking_model.h
  include/tournament_runner_gui/kata_round_participant_model.h
  include/tournament_runner_gui/add_participant_dialog.h
  include/tournament_runner_gui/participant_kata_round_WizardPage.h
)

set(${PROJECT_NAME}_GUI_FORMS 
  ui/about_dialog.ui
  ui/mainwindow.ui
  ui/new_tournament_dialog.ui
  ui/edit_tournament_dialog.ui
  ui/mainframe.ui
  ui/kataround_overview_frame.ui
  ui/finalframe.ui
  ui/add_participant_dialog.ui
)

qt5_wrap_cpp(${PROJECT_NAME}_GUI_HEADERS_MOC ${@PROJECT_NAME@_GUI_HEADERS})
qt5_wrap_ui(${PROJECT_NAME}_GUI_FORMS_HEADERS ${@PROJECT_NAME@_GUI_FORMS})

# Create a library

add_library(${PROJECT_NAME}_GUI-library
  STATIC 
  ${@PROJECT_NAME@_GUI_SOURCES} 
  ${@PROJECT_NAME@_GUI_HEADERS_MOC}
  ${@PROJECT_NAME@_GUI_FORMS_HEADERS}
)

target_link_libraries(${PROJECT_NAME}_GUI-library
                      ${@PROJECT_NAME@_LIBRARY}
                      ${Qt5Widgets_LIBRARIES})

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)
  include(CodeCoverage)
  
  add_library(${PROJECT_NAME}_GUI-library_coverage
    STATIC
    ${@PROJECT_NAME@_GUI_SOURCES} 
    ${@PROJECT_NAME@_GUI_HEADERS_MOC}
    ${@PROJECT_NAME@_GUI_FORMS_HEADERS}
  )
  
  set_target_properties(${PROJECT_NAME}_GUI-library_coverage PROPERTIES
    COMPILE_FLAGS "-fprofile-arcs -ftest-coverage"
  )
  
  target_link_libraries(${PROJECT_NAME}_GUI-library_coverage
                        ${@PROJECT_NAME@_LIBRARY}_coverage
                        ${Qt5Widgets_LIBRARIES})
  
endif(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)



# Create Main Qt Application

#set(QT_USE_QMAIN true)

if (WIN32)
  set(GUI_TYPE WIN32)
endif(WIN32)

add_executable(${PROJECT_NAME} ${GUI_TYPE} src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_GUI-library 
                                          ${@PROJECT_NAME@_LIBRARY}
                                          ${Qt5Widgets_LIBRARIES}
                                          ${Qt5Core_QTMAIN_LIBRARIES})

find_package(GMock)

if(GMOCK_FOUND)
  add_subdirectory(tests/unit)
endif(GMOCK_FOUND)

find_package(Cuke)

if(CUKE_FOUND)
  add_subdirectory(tests/feature)
endif(CUKE_FOUND)

# Qt translations

find_package(Qt5LinguistTools)

if(Qt5LinguistTools_FOUND)
  set(FILES_TO_TRANSLATE ${@PROJECT_NAME@_GUI_SOURCES} ${@PROJECT_NAME@_GUI_HEADERS} ${@PROJECT_NAME@_GUI_FORMS})

  file (GLOB TRANSLATIONS_FILES translations/*.ts)

  option (UPDATE_TRANSLATIONS "Update source translation translations/*.ts
    files (WARNING: make clean will delete the source .ts files! Danger!)")

  if(UPDATE_TRANSLATIONS)
    qt5_create_translation(QM_FILES ${FILES_TO_TRANSLATE} ${TRANSLATIONS_FILES})
  else(UPDATE_TRANSLATIONS)
    qt5_add_translation(QM_FILES ${TRANSLATIONS_FILES})
  endif(UPDATE_TRANSLATIONS)

  add_custom_target(translations DEPENDS ${QM_FILES})

  install(FILES ${QM_FILES} 
	DESTINATION bin/translations
	COMPONENT app)
					 
endif(Qt5LinguistTools_FOUND)

install(TARGETS ${PROJECT_NAME}
   RUNTIME
   DESTINATION bin
   COMPONENT app)

if (WIN32 AND NOT UNIX)
install (FILES 
  ${_qt5Widgets_install_prefix}/bin/Qt5Widgetsd.dll
  ${_qt5Widgets_install_prefix}/bin/Qt5Cored.dll
  ${_qt5Widgets_install_prefix}/bin/Qt5Guid.dll
  DESTINATION bin
  COMPONENT app)
endif(WIN32 AND NOT UNIX)

