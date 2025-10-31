cmake_minimum_required(VERSION 3.15)
project(SistemaGestionSensoresIoT 
        VERSION 1.0.0
        DESCRIPTION "Sistema de Gestión Polimórfica de Sensores para IoT"
        LANGUAGES CXX)

# Configuración del compilador C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

# Directorios
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(DOCS_DIR "${PROJECT_SOURCE_DIR}/docs")

# Archivos fuente
set(SOURCES
    ${SOURCE_DIR}/main.cpp
)

# Archivos de encabezado
set(HEADERS
    ${INCLUDE_DIR}/ListaSensor.hpp
    ${INCLUDE_DIR}/SensorBase.hpp
    ${INCLUDE_DIR}/SensorTemperatura.hpp
    ${INCLUDE_DIR}/SensorPresion.hpp
    ${INCLUDE_DIR}/GestorSensores.hpp
    ${INCLUDE_DIR}/ComunicacionSerial.hpp
)

# Crear ejecutable
add_executable(SistemaIoT ${SOURCES} ${HEADERS})

# Incluir directorios
target_include_directories(SistemaIoT PRIVATE ${INCLUDE_DIR})

# Configuración de plataforma específica
if(WIN32)
    target_link_libraries(SistemaIoT PRIVATE ws2_32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
    message(STATUS "Compilando para Windows")
elseif(UNIX AND NOT APPLE)
    # Linux/Debian
    message(STATUS "Compilando para Linux/Debian")
    target_link_libraries(SistemaIoT PRIVATE pthread)
elseif(APPLE)
    message(STATUS "Compilando para macOS")
endif()

# Propiedades del ejecutable
set_target_properties(SistemaIoT PROPERTIES
    OUTPUT_NAME "SistemaIoT"
    VERSION ${PROJECT_VERSION}
)

# Opción para generar documentación con Doxygen
option(BUILD_DOCS "Generar documentación con Doxygen" ON)

if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUTPUT_DIRECTORY ${DOCS_DIR})
        set(DOXYGEN_HTML_OUTPUT html)
        set(DOXYGEN_LATEX_OUTPUT latex)
        set(DOXYGEN_GENERATE_LATEX YES)
        set(DOXYGEN_USE_MATHJAX YES)
        set(DOXYGEN_HAVE_DOT NO)
        
        add_custom_target(docs ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Generando documentación con Doxygen..."
            VERBATIM)
        
        message(STATUS "Doxygen encontrado. Se generará documentación.")
    else()
        message(WARNING "Doxygen no encontrado. No se generará documentación.")
    endif()
endif()

# Información de compilación
message(STATUS "========================================")
message(STATUS "Proyecto: ${PROJECT_NAME}")
message(STATUS "Versión: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Include Directory: ${INCLUDE_DIR}")
message(STATUS "Source Directory: ${SOURCE_DIR}")
message(STATUS "========================================")
