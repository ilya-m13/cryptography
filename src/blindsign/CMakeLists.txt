set(appname blindsign)

add_executable(${appname} blindsign/main.cpp)

set_compile_options(${appname})
target_link_libraries(${appname} cga)