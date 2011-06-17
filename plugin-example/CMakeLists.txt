project(plugin_example)

set(plugin_example_INCLUDE_DIRECTORIES
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${MALIIT_INCLUDE_DIRS}
    ${python_plugin_SOURCE_DIR}
)

set(plugin_example_LINK_LIBRARIES
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${MALIIT_LIBRARIES}
    python_plugin
)

QT4_WRAP_CPP(plugin_example_MOC pymaliit.h OPTIONS -I${python_plugin_SOURCE_DIR} -I${MALIIT_INCLUDE_DIRS})

set(plugin_example_SRC
    pymaliit.cpp
    pymaliit.h
    ${plugin_example_MOC}
)

add_definitions(-DPYTHON_PLUGIN_FILENAME="${MALITT_PLUGINS_DIR}/python/pymaliit.py")
add_definitions(-DQT_PLUGIN)
include_directories(pymaliit ${plugin_example_INCLUDE_DIRECTORIES})
add_library(pymaliit SHARED ${plugin_example_SRC})
set_target_properties(pymaliit PROPERTIES PREFIX "")
target_link_libraries(pymaliit ${plugin_example_LINK_LIBRARIES})

install(TARGETS pymaliit LIBRARY DESTINATION ${MALITT_PLUGINS_DIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/pymaliit.py
        DESTINATION ${MALITT_PLUGINS_DIR}/python)
