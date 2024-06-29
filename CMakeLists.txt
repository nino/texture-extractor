cmake_minimum_required(VERSION 3.5)

project(texture_extractor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)


add_compile_options(-Wall -Wextra -Wpedantic)

set(TS_FILES texture_extractor_en_US.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.h
        src/mainwindow.cpp
        src/documentwindow.h
        src/documentwindow.cpp
        src/SourceImageView.h
        src/SourceImageView.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(texture_extractor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(texture_extractor SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(texture_extractor
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(texture_extractor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Set application properties
set_target_properties(texture_extractor PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Add custom Info.plist for macOS
if(APPLE)
    set_target_properties(texture_extractor PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER com.ninoan.texture_extractor
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
    
    # Copy and configure Info.plist
    set(INFO_PLIST_IN "${CMAKE_SOURCE_DIR}/Info.plist.in")
    set(INFO_PLIST_OUT "${CMAKE_BINARY_DIR}/Info.plist")
    configure_file(${INFO_PLIST_IN} ${INFO_PLIST_OUT} @ONLY)
    set_target_properties(texture_extractor PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${INFO_PLIST_OUT}
    )
endif()

include(GNUInstallDirs)
install(TARGETS texture_extractor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(texture_extractor)
endif()
