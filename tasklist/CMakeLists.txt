cmake_minimum_required(VERSION 3.16)

project(tasklist VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(apptasklist
    main.cpp
)

qt_add_qml_module(apptasklist
    URI tasklist
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES Task.qml
    RESOURCES assets/checkbox-complete.svg
    RESOURCES assets/checkbox.svg
    QML_FILES NewTaskView.qml
    RESOURCES assets/back.png
    RESOURCES assets/delete.svg
    SOURCES mylistmodel.h mylistmodel.cpp
    SOURCES mylistitem.h mylistitem.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apptasklist PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptasklist
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set_property(TARGET hangman APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/android/
)

target_sources(hangman PRIVATE

)

target_link_libraries(apptasklist
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS apptasklist
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
