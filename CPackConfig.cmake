### versions
set(CPACK_PACKAGE_VERSION_MAJOR "${kata_tournament_runner_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${kata_tournament_runner_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${kata_tournament_runner_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

### general settings
set(CPACK_PACKAGE_NAME "${APPLICATION_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Kata Tournament Runner.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VENDOR "Michel Estermann")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "Kata_Tournament_Runner")
set(CPACK_OUTPUT_FILE_PREFIX "../install")

set(CPACK_COMPONENTS_ALL app doc)
set(CPACK_COMPONENT_APP_DISPLAY_NAME "Kata Tournament Runner")
set(CPACK_COMPONENT_DOC_DISPLAY_NAME "Documentation")

set(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION
   "The Kata Tournament Runner Qt Application"
)

if(WIN32 AND NOT UNIX)
  ### nsis generator
  set(CPACK_GENERATOR "NSIS")  
  set(CPACK_NSIS_DISPLAY_NAME "${APPLICATION_NAME}")
  set(CPACK_NSIS_COMPRESSOR "/SOLID zlib")   

  set(CPACK_NSIS_CONTACT "estermann.michel@gmail.com")
  set(CPACK_NSIS_HELP_LINK "https://github.com/meshell/KataTournamentRunner")
  set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/meshell/KataTournamentRunner")
  
  set(CPACK_NSIS_CREATE_ICONS "
    SetOutPath \\\"$INSTDIR\\\\bin\\\"
    CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\TournamentRunner.lnk\\\" \\\"$INSTDIR\\\\bin\\\\Kata_Tournament_Runner.exe\\\"
    CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Documentation.lnk\\\" \\\"$INSTDIR\\\\doc\\\\html\\\\index.html\\\"
    CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\License.lnk\\\" \\\"$INSTDIR\\\\doc\\\\LICENSE\\\"
    CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\README.lnk\\\" \\\"$INSTDIR\\\\doc\\\\README.md\\\"
  ")
  set(CPACK_NSIS_DELETE_ICONS "
    Delete \\\"$SMPROGRAMS\\\\$MUI_TEMP\\\\Documentation.lnk\\\"
    Delete \\\"$SMPROGRAMS\\\\$MUI_TEMP\\\\License.lnk\\\"
    Delete \\\"$SMPROGRAMS\\\\$MUI_TEMP\\\\README.lnk\\\"
  ")
  
  set(CPACK_NSIS_MENU_LINKS "doc/html/index.html" "Documentation")

else(WIN32 AND NOT UNIX)
  # Determine current architecture
  macro(dpkg_arch VAR_NAME)
    find_program(DPKG_PROGRAM dpkg DOC "dpkg program of Debian-based systems")
    if (DPKG_PROGRAM)
      execute_process(
        COMMAND ${DPKG_PROGRAM} --print-architecture
        OUTPUT_VARIABLE ${VAR_NAME}
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    endif(DPKG_PROGRAM)
  endmacro(dpkg_arch)
  
  # DEB package config
  set(CPACK_DEBIAN_PACKAGE_NAME ${LOWER_CASE_PROJECT_NAME})
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Michel Estermann <estermann.michel@gmail.com>")
  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/meshell/KataTournamentRunner")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5,libboost-date-time,libboost-serialization,libboost-filesystem")
  #set(CPACK_DEBIAN_PACKAGE_SECTION "")
  #set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
  set(CPACK_GENERATOR "DEB")
  set(CPACK_SET_DESTDIR true)
  set(CPACK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
          
  dpkg_arch(CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
  if (CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
    set(CPACK_PACKAGE_FILE_NAME ${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})
  else (CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
   set(CPACK_PACKAGE_FILE_NAME ${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${CMAKE_SYSTEM_NAME})
  endif (CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
endif (WIN32 AND NOT UNIX)


include(CPack)
include(InstallRequiredSystemLibraries)
