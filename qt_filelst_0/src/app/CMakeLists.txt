set(app ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE UI_FILES *.ui)
file(GLOB_RECURSE CODE_FILES *.cpp *.hpp)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})
qt5_add_resources(RESOURCE_FILES ../resources/resources.qrc)

set(LIBS ${CMAKE_PROJECT_NAME}_lib) 

set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost 1.59.0 COMPONENTS system filesystem REQUIRED)
find_package(Qt5Core REQUIRED)

include_directories ("${PROJECT_SOURCE_DIR}/lib" ${Boost_INCLUDE_DIR})
link_directories ("${PROJECT_SOURCE_DIR}/lib" ${Boost_LIBRARY_DIR})

add_executable (${app} WIN32
  ${UI_HEADERS}
  ${CODE_FILES}
  ${RESOURCE_FILES}
)

target_link_libraries (${app} ${LIBS} 
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY} 
	Qt5::Widgets
)

install (TARGETS ${app}
         RUNTIME DESTINATION .)
