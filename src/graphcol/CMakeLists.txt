set(appname graphcol)

add_executable(${appname} graphcol/main.cpp)

set_compile_options(${appname})
target_link_libraries(${appname} cga cxxopts)