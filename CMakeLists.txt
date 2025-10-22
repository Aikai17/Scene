cmake_minimum_required(VERSION 3.16)

project(basics VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Quick3D)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appbasics
    src/main.cpp
)

qt_add_qml_module(appbasics
    URI basics
    VERSION 1.0
    QML_FILES
        src/Main.qml
        QML_FILES src/Inspector.qml
        QML_FILES
        QML_FILES
        QML_FILES
        SOURCES src/Primitive.h src/Primitive.cpp
        SOURCES src/SceneManager.h src/SceneManager.cpp
        QML_FILES src/OrbitController.qml
        SOURCES src/PrimitiveFactory.h src/PrimitiveFactory.cpp
        SOURCES src/ThreadManager.h src/ThreadManager.cpp
        QML_FILES
        QML_FILES src/Scene.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appbasics PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appbasics
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appbasics
    PRIVATE Qt6::Quick
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Quick3D
)

include(GNUInstallDirs)
install(TARGETS appbasics
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
