add_library(MainWindow STATIC
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
)
target_link_libraries(MainWindow PUBLIC Qt::Core Qt::Gui Qt::Widgets)
target_include_directories(MainWindow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
