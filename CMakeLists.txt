
cmake_minimum_required(VERSION 2.6)
project(TeamSoftTask.exe)

add_executable(${PROJECT_NAME}
	Main.cpp
	TaskList.cpp
	UserListener.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
