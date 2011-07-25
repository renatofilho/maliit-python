project(python-maliit)

cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Macros/
                      ${CMAKE_MODULE_PATH})

find_package(GeneratorRunner 0.6.10 REQUIRED)
find_package(Shiboken 1.0.3 REQUIRED)
find_package(Qt4 4.7.0 REQUIRED)
find_package(PySide 1.0.4 REQUIRED)
find_package(PkgConfig)
pkg_check_modules(MALIIT maliit-0.80)
pkg_check_modules(MALIIT_PLUGINS maliit-plugins-0.80)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ${SHIBOKEN_BUILD_TYPE})
endif()

set(BINDING_NAME PyMaliit)
set(BINDING_API_MAJOR_VERSION "0")
set(BINDING_API_MINOR_VERSION "0")
set(BINDING_API_MICRO_VERSION "1")
set(BINDING_API_VERSION "${BINDING_API_MAJOR_VERSION}.${BINDING_API_MINOR_VERSION}.${BINDING_API_MICRO_VERSION}" CACHE STRING "PySide version" FORCE)

# uninstall target
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
               IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}"
                  -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

if (NOT SITE_PACKAGE)
    execute_process(
        COMMAND ${SHIBOKEN_PYTHON_INTERPRETER} -c "from distutils import sysconfig; \\
            print sysconfig.get_python_lib(1,0,prefix='${CMAKE_INSTALL_PREFIX}')"
        OUTPUT_VARIABLE SITE_PACKAGE
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (NOT SITE_PACKAGE)
        message(FATAL_ERROR "Could not detect Python module installation directory.")
    elseif (APPLE)
        message(STATUS "!!! The generated bindings will be installed on ${SITE_PACKAGE}, is it right!?")
    endif()
endif()

execute_process(COMMAND pkg-config --variable factorypluginsdir maliit-plugins-0.80
                OUTPUT_VARIABLE MALITT_FACTORY_PLUGINS_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND pkg-config --variable pluginsdir maliit-plugins-0.80
                OUTPUT_VARIABLE MALITT_PLUGINS_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "Maliit plugins dir: ${MALITT_PLUGINS_DIR}")

set(GENERATOR_EXTRA_FLAGS --generator-set=shiboken --enable-parent-ctor-heuristic --enable-pyside-extensions --enable-return-value-heuristic --use-isnull-as-nb_nonzero)
add_subdirectory(maliit)
add_subdirectory(plugin)
add_subdirectory(plugin-example)
