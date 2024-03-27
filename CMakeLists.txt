cmake_minimum_required(VERSION 3.10)

project(anoUdp)

# Add your source files here
set(SOURCE_FILES main.c Ano-Protocol/ano-base.c Ano-Protocol/ano-parameter.c Ano-Protocol/ano-update.c Ano-Protocol/ano-command.c Ano-Protocol/ano-control-info.c)
include_directories(.)

# Create an executable from the source files
add_executable(anoUdp ${SOURCE_FILES})
