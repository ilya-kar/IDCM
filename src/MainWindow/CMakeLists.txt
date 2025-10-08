set(TARGET_NAME "MainWindow")

add_library(MainWindow STATIC
        MainWindow.cpp
        MainWindow.h
)

target_link_libraries(MainWindow PUBLIC
        BatteryInfo
)

target_include_directories(MainWindow PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)