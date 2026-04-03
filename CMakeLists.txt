cmake_minimum_required(VERSION 3.26)
project(ProjetOS C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(
        inc
        ${GTK3_INCLUDE_DIRS}
)

link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

# CLI executable
add_executable(ProjetOS
        src/main.c
        src/Calcul_temps.c
        src/lecture_fichier.c
        src/Liste.c
        src/ordonnancement.c
        src/ProcStruct.c
        src/SJF.c
        src/SJRF.c
)

# GUI executable
add_executable(ProjetOS_GUI
        src/gui_main.c
        src/gui.c
        src/simulation_gui.c
        src/csv_export.c
        src/Calcul_temps.c
        src/lecture_fichier.c
        src/Liste.c
        src/ProcStruct.c
        src/SJF.c
        src/SJRF.c
)

target_link_libraries(ProjetOS_GUI ${GTK3_LIBRARIES})