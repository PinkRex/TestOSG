cmake_minimum_required(VERSION 3.16)

project(OSG_Test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OSGEARTH_ROOT /home/canhdx/Setup/VISG/OSG/OSGEarth-3.7/osgearth/install)
set(OSG_ROOT /home/canhdx/Setup/VISG/OSG/OSG-3.6.5/OpenSceneGraph/install)
set(GDAL_ROOT /home/canhdx/Setup/VISG/GDAL/gdal-3.9.2/install)
set(GEOS_ROOT /home/canhdx/Setup/VISG/GEOS/geos-3.12.3/install)

find_package(Qt6 REQUIRED COMPONENTS Quick)
# find_package(GDAL REQUIRED)

include_directories(${OSGEARTH_ROOT}/include)
link_directories(${OSGEARTH_ROOT}/lib64)

include_directories(${OSG_ROOT}/include)
link_directories(${OSG_ROOT}/lib)

include_directories(${GDAL_ROOT}/include)
link_directories(${GDAL_ROOT}/lib)

include_directories(${GEOS_ROOT}/include)
link_directories(${GEOS_ROOT}/lib)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appOSG_Test
    main.cpp
)

qt_add_resources(appOSG_Test "Resource"
    PREFIX "/"
    FILES images/background.jpg images/leftBtn.jpg images/rightBtn.jpg
)

qt_add_qml_module(appOSG_Test
    URI OSG_Test
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES osgearthitem2.h osgearthitem2.cpp
        SOURCES osgearthrenderer2.h osgearthrenderer2.cpp
        SOURCES Template.cpp
        RESOURCES images/background.jpg images/leftBtn.jpg images/rightBtn.jpg
        RESOURCES Resource.qrc
        QML_FILES Views/StartUpUI.qml
        QML_FILES Views/ModeChooserUI.qml
        QML_FILES Views/3DViewer_Test.qml
)

set_target_properties(appOSG_Test PROPERTIES
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
