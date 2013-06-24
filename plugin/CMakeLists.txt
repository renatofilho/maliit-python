project(pymaliit-plugin)

set(MALIIT_PLUGINS_INCLUDE_DIRS_FIX "")
foreach(LOOP_FIX ${MALIIT_PLUGINS_INCLUDE_DIRS})
    list(APPEND MALIIT_PLUGINS_INCLUDE_DIRS_FIX "-I${LOOP_FIX}")
endforeach(LOOP_FIX)


QT4_WRAP_CPP(pymaliit_MOC minputmethodpythonfactoryplugin.h OPTIONS ${MALIIT_PLUGINS_INCLUDE_DIRS_FIX})

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
link_directories(${MALIIT_LIBRARY_DIRS})
include_directories(pymaliit ${pymaliit_INCLUDE_DIRECTORIES})
add_library(pymaliit SHARED ${pymaliit_SRC})
target_link_libraries(pymaliit ${pymaliit_LINK_LIBRARIES})

install(TARGETS pymaliit LIBRARY DESTINATION ${MALITT_FACTORY_PLUGINS_DIR})
