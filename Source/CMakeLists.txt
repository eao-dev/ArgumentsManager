cmake_minimum_required (VERSION 3.15)

project(ArgsManager)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

add_library(
	${PROJECT_NAME}
	
	ArgsManager.h
	ArgsManager.cpp

	Argument.h
	Argument.cpp
	
	InvalidArg.h
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)