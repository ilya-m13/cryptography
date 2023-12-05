set(appname zkprot)

add_executable(${appname} zkprot/main.cpp)

set_compile_options(${appname})
target_link_libraries(${appname} cga cxxopts)