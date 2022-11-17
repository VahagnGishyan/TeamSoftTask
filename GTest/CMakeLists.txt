
cmake_minimum_required(VERSION 2.6)
project(RunTestsForTeamSoftTask.exe)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS}
        ../
)

add_executable(${PROJECT_NAME}
	../TaskList.cpp
	../UserListener.cpp
	GTest-TaskList-Task-Date.cpp
	GTest-TaskList-Task.cpp
	GTest-UserListener.cpp
	Run.cpp
)

target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
