cmake_minimum_required(VERSION 3.16)

project(Practica_4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(Practica_4 main.cpp

    red.h red.cpp
    enrutador.h enrutador.cpp
)

target_link_libraries(Practica_4 PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS Practica_4
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
