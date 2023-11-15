include(CompileOptions)

set(appname mentalpoker)

add_executable(${appname} mental_poker/main.cpp)

set_compile_options(${appname})
target_link_libraries(
    ${appname}
        cga
        cxxopts
)