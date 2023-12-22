cmake_minimum_required(VERSION 3.16)

project(contactlist VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appcontactlist
    main.cpp
    mylistmodel.h
    mylistmodel.cpp
    android/src/com/contactlist/MainActivity.java
    android/AndroidManifest.xml
    android/build.gradle
    MANUAL_FINALIZATION
)

qt_add_qml_module(appcontactlist
    URI contactlist
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES NewContactView.qml
    RESOURCES assets/delete.svg
    RESOURCES assets/edit.svg
    RESOURCES assets/edit.png
    RESOURCES assets/back.png
    QML_FILES Popup.qml
    QML_FILES EditContactView.qml
)

set_target_properties(appcontactlist PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appcontactlist
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set_property(TARGET appcontactlist APPEND PROPERTY
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

target_link_libraries(appcontactlist
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appcontactlist
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(appcontactlist)
