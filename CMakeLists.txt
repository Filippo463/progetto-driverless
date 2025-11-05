# Versione minima di CMake
cmake_minimum_required(VERSION 3.10)

# Nome del tuo progetto (puoi cambiarlo)
project(DriverlessControl)

# 1. Trova tutti i tuoi file sorgente
# Questa riga cerca in "src/" e trova main.cpp e KinematicModel.cpp
file(GLOB SOURCES "src/*.cpp")

# 2. Crea un eseguibile
# Si chiamerà "my_car_simulator" e sarà creato usando i file in SOURCES
add_executable(my_car_simulator ${SOURCES})

# 3. LA RIGA PIÙ IMPORTANTE (per il tuo errore)
# Dice al compilatore: "Quando cerchi file .h, 
# guarda ANCHE dentro la cartella 'include'".
target_include_directories(my_car_simulator PUBLIC include)

# (Buona pratica, usa C++ moderno)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)