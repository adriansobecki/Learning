cmake_minimum_required(VERSION 3.5)
project(modelBased VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui)

# Add the directory to CMAKE_AUTOUIC_SEARCH_PATHS
set(CMAKE_AUTOUIC_SEARCH_PATHS ${UI_DIR})

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Enable automatic handling of UI, MOC, and QRC files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

# Use the "file(GLOB ...)" command to get a list of all source files in /src/
file(GLOB SOURCES ${SRC_DIR}/*.cpp)
file(GLOB HEADERS ${INC_DIR}/*.h)
file(GLOB UI ${UI_DIR}/*.ui)

message(STATUS ${SOURCES})
message(STATUS ${HEADERS})
message(STATUS ${UI})

# Generate the UI headers from .ui files
#qt6_wrap_ui(UI_HEADERS ${UI} ${UI})

add_executable(modelBased ${SOURCES} ${HEADERS} ${UI})

target_include_directories(modelBased PRIVATE ${INC_DIR})

target_link_libraries(modelBased PRIVATE Qt6::Widgets Qt6::Sql)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/database.db DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
install(FILES $<TARGET_FILE:Qt6::Sql> $<TARGET_FILE:Qt6::Widgets> DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
install(TARGETS modelBased RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)