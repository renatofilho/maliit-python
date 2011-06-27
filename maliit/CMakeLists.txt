project(pymaliit)

set(maliit_SRC
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/mabstractinputmethod_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/mabstractinputmethod_wrapper.h
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/mabstractinputmethodhost_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/mabstractinputmethodhost_wrapper.h
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/mabstractinputmethodsettings_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/mabstractinputmethodsettings_wrapper.h
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/maliit_module_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/maliit_python.h
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/mimplugindescription_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/mimplugindescription_wrapper.h
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/minputmethod_preedittextformat_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/minputmethod_preedittextformat_wrapper.h
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/minputmethod_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/minputmethod_wrapper.h
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/minputmethodplugin_wrapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/maliit/minputmethodplugin_wrapper.h
)

set(maliit_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SHIBOKEN_INCLUDE_DIR}
    ${SHIBOKEN_PYTHON_INCLUDE_DIR}
    ${PYTHON_INCLUDE_PATH}
    ${PYSIDE_INCLUDE_DIR}
    ${PYSIDE_INCLUDE_DIR}/QtCore
    ${PYSIDE_INCLUDE_DIR}/QtGui
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${MALIIT_INCLUDE_DIRS}
    ${MALIIT_PLUGINS_INCLUDE_DIRS}
)

set(maliit_LINK_LIBRARIES
    ${SHIBOKEN_PYTHON_LIBRARIES}
    ${SHIBOKEN_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${PYSIDE_LIBRARY}
    ${MALIIT_LIBRARIES}
    ${MALIIT_PLUGINS_LIBRARIES}
)

include_directories(maliit ${maliit_INCLUDE_DIRECTORIES})
add_library(maliit MODULE ${maliit_SRC})
set_property(TARGET maliit PROPERTY PREFIX "")
target_link_libraries(maliit ${maliit_LINK_LIBRARIES})

add_custom_command(OUTPUT ${maliit_SRC}
                   COMMAND ${GENERATORRUNNER_BINARY}
                   --generatorSet=shiboken --enable-parent-ctor-heuristic --enable-pyside-extensions --enable-return-value-heuristic
                   ${CMAKE_CURRENT_SOURCE_DIR}/global.h
                   --include-paths=${QT_INCLUDE_DIR}:${MALIIT_PLUGINS_INCLUDE_DIRS}
                   --typesystem-paths=${typesystem_path}:${PYSIDE_TYPESYSTEMS}
                   --output-directory=${CMAKE_CURRENT_BINARY_DIR}
                   ${CMAKE_CURRENT_SOURCE_DIR}/typesystem_maliit.xml
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   COMMENT "Running generator for maliit..."
                  )

install(TARGETS maliit
        LIBRARY DESTINATION ${SITE_PACKAGE})

