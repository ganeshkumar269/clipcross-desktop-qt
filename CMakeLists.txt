cmake_minimum_required(VERSION 3.0)

project(clippycross)
set(CMAKE_CXX_STANDARD 17)  # Assuming C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Sql Network WebSockets Widgets Concurrent)
qt_standard_project_setup()

# Include directories for header files
include_directories(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/dependencies
        ${PROJECT_SOURCE_DIR}/dependencies/framelesswindow
        ${PROJECT_SOURCE_DIR}/dependencies/Log4Qt/src
        ${PROJECT_SOURCE_DIR}/dependencies/Log4Qt/src/log4qt
        /Users/ganesh.maddula/Documents/projects/QHotkey/QHotKey  # Assuming relative path
)

## Link libraries
link_directories(
        ${PROJECT_SOURCE_DIR}/dependencies/Log4Qt/bin
        /Users/ganesh.maddula/Documents/projects/QHotkey/QHotKey  # Assuming relative path
)
set(CMAKE_OSX_ FRAMEWORK Carbon)  # Link Carbon framework on macOS
#
## Define preprocessor symbols
add_definitions(-DLOG4QT_LIBRARY -DLOG4QT_STATIC -Dfpermissive)  # Assuming C++11 flag
#
## Add resources
qt_add_resources(RESOURCES_clippycross resources/framelesswindow.qrc resources/darkstyle.qrc resources/clippycross.qrc)
#
## Add UI files (if using Qt Designer)
#qt_gui_create(FORMS dependencies/framelesswindow/framelesswindow.ui)  # Uncomment if using Qt Designer
#
## Source files
set(SOURCE_FILES
    main.cpp
    ${PROJECT_SOURCE_DIR}/src/database/database.cpp
    ${PROJECT_SOURCE_DIR}/src/database/getDatabaseConnection.cpp
    ${PROJECT_SOURCE_DIR}/src/database/getRowCount.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/formatQString.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/getMd5Hash.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/getOAuthUrl.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/getTimestamp.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/quotedQString.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/simulatePasteKeySequence.cpp
    ${PROJECT_SOURCE_DIR}/src/handler.cpp
    ${PROJECT_SOURCE_DIR}/src/vcb.cpp
    ${PROJECT_SOURCE_DIR}/src/getQueryParamValue.cpp
    ${PROJECT_SOURCE_DIR}/src/vcbhandler.cpp
    ${PROJECT_SOURCE_DIR}/dependencies/framelesswindow/framelesswindow.cpp
    ${PROJECT_SOURCE_DIR}/dependencies/framelesswindow/windowdragger.cpp
    ${PROJECT_SOURCE_DIR}/dependencies/framelesswindow/DarkStyle.cpp
    ${PROJECT_SOURCE_DIR}/src/eventlisteners/rightarroweventlistener.cpp
    ${PROJECT_SOURCE_DIR}/src/eventlisteners/leftarroweventlistener.cpp
    ${PROJECT_SOURCE_DIR}/src/eventlisteners/menuiconeventlistener.cpp
    ${PROJECT_SOURCE_DIR}/src/mainwindow.cpp
    ${PROJECT_SOURCE_DIR}/src/prefs.cpp
)
#
## Add header files
set(HEADER_FILES
    ${PROJECT_SOURCE_DIR}/include/database.h
    ${PROJECT_SOURCE_DIR}/include/handler.h
    ${PROJECT_SOURCE_DIR}/include/utils.h
    ${PROJECT_SOURCE_DIR}/include/clip.h
    ${PROJECT_SOURCE_DIR}/include/vcb.h
    ${PROJECT_SOURCE_DIR}/include/vcbhandler.h
    ${PROJECT_SOURCE_DIR}/include/constants.h
    ${PROJECT_SOURCE_DIR}/include/mainwindow.h
    ${PROJECT_SOURCE_DIR}/include/rightarroweventlistener.h
    ${PROJECT_SOURCE_DIR}/include/leftarroweventlistener.h
    ${PROJECT_SOURCE_DIR}/include/menuiconeventlistener.h
    ${PROJECT_SOURCE_DIR}/include/prefs.h
    ${PROJECT_SOURCE_DIR}/dependencies/framelesswindow/framelesswindow.h
    ${PROJECT_SOURCE_DIR}/dependencies/framelesswindow/windowdragger.h
    ${PROJECT_SOURCE_DIR}/dependencies/framelesswindow/DarkStyle.h
)
#
## Create executable
qt_add_executable(clippycross ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCES_clippycross})
target_link_libraries(clippycross PRIVATE Qt6::Core Qt6::Gui Qt6::Sql Qt6::Network Qt6::WebSockets Qt6::Widgets Qt6::Concurrent log4qt QHotkey)

if(APPLE)
    target_link_libraries(clippycross PRIVATE
            "-framework Carbon"
    )
endif()