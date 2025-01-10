cmake_minimum_required(VERSION 3.10)

project(anoUdp)

# Add your source files here
set(SOURCE_FILES ../Ano-Protocol/ano-base.c ../Ano-Protocol/ano-parameter.c 
    ../Ano-Protocol/ano-update.c ../Ano-Protocol/ano-command.c ../Ano-Protocol/ano-control-info.c)
include_directories(.)
include_directories(../)

# Create an executable from the source files
add_executable(command_anoUdp ${SOURCE_FILES} main_command.c)

add_executable(update_anoUdp ${SOURCE_FILES} main_update.c)

add_executable(parameter_anoUdp ${SOURCE_FILES} main_parameter.c)

