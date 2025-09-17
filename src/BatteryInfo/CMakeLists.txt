set(TARGET_NAME "BatteryInfo")

add_library(BatteryInfo STATIC
        BatteryInfo.cpp
        BatteryInfo.h
)

target_link_libraries(BatteryInfo PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        kernel32
        PowrProf
        setupapi
)

target_include_directories(BatteryInfo PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_autogen/include
)