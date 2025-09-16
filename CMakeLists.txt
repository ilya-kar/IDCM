cmake_minimum_required(VERSION 4.0)
project(IDCM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS REQUIRED
        Core
        Gui
        Widgets
)

add_subdirectory(src/MainWindow)
add_subdirectory(src/BatteryInfo)

add_executable(myapp src/main.cpp resources/resources.qrc)
target_link_libraries(myapp PRIVATE MainWindow)

set(DEBUG_SUFFIX)
if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
    set(DEBUG_SUFFIX "d")
endif ()

if (NOT CMAKE_BUILD_TYPE MATCHES "Debug")
    set_target_properties(myapp PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()

set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
    set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
    endif ()
endif ()

set(QT_PLUGINS
        "platforms\;qwindows${DEBUG_SUFFIX}.dll"
        "styles\;qmodernwindowsstyle${DEBUG_SUFFIX}.dll"
        "imageformats\;qjpeg${DEBUG_SUFFIX}.dll"
)

foreach(PLUGIN ${QT_PLUGINS})
    list(GET PLUGIN 0 SUBDIR)
    list(GET PLUGIN 1 FILE)
    set(SRC "${QT_INSTALL_PATH}/plugins/${SUBDIR}/${FILE}")
    set(DST "$<TARGET_FILE_DIR:myapp>/plugins/${SUBDIR}/")

    if (EXISTS "${SRC}")
        add_custom_command(TARGET myapp POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "${DST}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SRC}" "${DST}"
        )
    else()
        message(WARNING "Qt plugin not found: ${SRC}")
    endif()
endforeach()

foreach (QT_LIB Core Gui Widgets)
    add_custom_command(TARGET myapp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:myapp>")
endforeach (QT_LIB)
