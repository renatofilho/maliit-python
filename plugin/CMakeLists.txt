project(python_plugin)

set(python_plugin_SRC
    minputmethodpythonplugin.cpp
    minputmethodpythonplugin.h
)

set(python_plugin_INCLUDE_DIRECTORIES
    ${SHIBOKEN_INCLUDE_DIR}
    ${SHIBOKEN_PYTHON_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${MALIIT_INCLUDE_DIRS}
)

set(python_plugin_LINK_LIBRARIES
    ${SHIBOKEN_LIBRARY}
    ${SHIBOKEN_PYTHON_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${MALIIT_LIBRARIES}
)

add_definitions(-DQT_SHARED)
include_directories(python_plugin ${python_plugin_INCLUDE_DIRECTORIES})
add_library(python_plugin SHARED ${python_plugin_SRC})
target_link_libraries(python_plugin ${python_plugin_LINK_LIBRARIES})

install(TARGETS python_plugin LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
