cmake_minimum_required(VERSION 3.16)

project(OSG_Test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OSGEARTH_ROOT /home/canhdx/Setup/OSG/osgearth/install)
set(OSG_ROOT /home/canhdx/Setup/OSG/OpenSceneGraph/install)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(GDAL REQUIRED)
# target_link_libraries(appOSG_Test PRIVATE GDAL::GDAL)

include_directories(${OSGEARTH_ROOT}/include)
link_directories(${OSGEARTH_ROOT}/lib64)

include_directories(${OSG_ROOT}/include)
link_directories(${OSG_ROOT}/lib)

link_directories(/home/canhdx/Setup/GDAL/install/lib/)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appOSG_Test
    main.cpp
    SimpleOsgEarthItem.cpp
    # OsgEarthView.cpp
    # OsgEarthRenderer.cpp
)

qt_add_qml_module(appOSG_Test
    URI OSG_Test
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES
        SOURCES
        SOURCES
        SOURCES SimpleOsgEarthItem.h SimpleOsgEarthItem.cpp
        RESOURCES Test.earth
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appOSG_Test PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appOSG_Test
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appOSG_Test
    PRIVATE Qt6::Quick

    osg
    osgDB
    osgGA
    osgViewer
    osgUtil
    osgText
    osgEarth

    gdal
    proj
    sqlite3
)

include(GNUInstallDirs)
install(TARGETS appOSG_Test
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
