cmake_minimum_required(VERSION 3.6)

project(stick VERSION 1.0.0 DESCRIPTION "led stick library")

file(GLOB SOURCES
    *.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(FILES ${PROJECT_NAME}.h DESTINATION include)