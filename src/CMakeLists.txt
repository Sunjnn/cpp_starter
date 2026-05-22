add_library(mylib
    mylib.cpp
)

target_include_directories(mylib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(myproject main.cpp)
target_link_libraries(myproject PRIVATE mylib)
