set(appname sign)

add_executable(${appname} sign/main.cpp)
include(CompileOptions)
set_compile_options(${appname})
target_link_libraries(
    ${appname}
        cga
        cxxopts    
)