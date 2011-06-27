project(pymaliit-plugin)

QT4_WRAP_CPP(pymaliit_MOC minputmethodpythonfactoryplugin.h OPTIONS -I${MALIIT_PLUGINS_INCLUDE_DIRS})

set(pymaliit_SRC
    minputmethodpythonfactoryplugin.cpp
    minputmethodpythonfactoryplugin.h
    ${pymaliit_MOC}
)

set(pymaliit_INCLUDE_DIRECTORIES
    ${SHIBOKEN_INCLUDE_DIR}
    ${SHIBOKEN_PYTHON_INCLUDE_DIR}
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${MALIIT_INCLUDE_DIRS}
    ${MALIIT_PLUGINS_INCLUDE_DIRS}
)

set(pymaliit_LINK_LIBRARIES
    ${SHIBOKEN_LIBRARY}
    ${SHIBOKEN_PYTHON_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${MALIIT_LIBRARIES}
    ${MALIIT_PLUGINS_LIBRARIES}
)

add_definitions(-DQT_PLUGIN)
add_definitions(-DMALITT_PLUGINS_DIR="${MALITT_PLUGINS_DIR}")
include_directories(pymaliit ${pymaliit_INCLUDE_DIRECTORIES})
add_library(pymaliit SHARED ${pymaliit_SRC})
target_link_libraries(pymaliit ${pymaliit_LINK_LIBRARIES})

install(TARGETS pymaliit LIBRARY DESTINATION ${MALITT_PLUGINS_DIR})
