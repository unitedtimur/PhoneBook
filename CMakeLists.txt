cmake_minimum_required(VERSION 3.5)

project(PhoneBook)

#*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

#*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*

set (SOURCES
    src/main.cpp
    src/phonebookmanager.ui
    src/phonebookmanager.h
    src/phonebookmanager.cpp

    src/addressmodel.h
    src/addressmodel.cpp

    src/contact.h
    src/config.h

    src/editdialog.ui
    src/editdialog.h
    src/editdialog.cpp
)

#*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*

add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCES}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
