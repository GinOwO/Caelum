cmake_minimum_required(VERSION 3.5)

project(Caelum VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB MAIN_SOURCES "${CMAKE_SOURCE_DIR}/src/main/*.cpp")
fiLe(GLOB HEADER_SRC "${CMAKE_SOURCE_DIR}/include/*.h")

set(PROJECT_SOURCES
    main.cpp
    ${HEADER_SRC}
    ${MAIN_SOURCES}
    src/ui/mainwindow.ui src/ui/mainwindow.h src/ui/mainwindow.cpp
    resources/icons.qrc resources/themes.qrc
)

qt_add_executable(Caelum
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    resources/themes.qrc
    src/ui/outputview.h src/ui/outputview.cpp src/ui/outputview.ui
)

target_link_libraries(Caelum PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(Caelum PRIVATE
    ${CMAKE_SOURCE_DIR}/include/
)

include(GNUInstallDirs)
install(TARGETS Caelum
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Caelum)
endif()
