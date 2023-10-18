set(app_name encrypt)

add_executable(${app_name})

include(CompileOptions)
set_compile_options(${app_name})

target_sources(
    ${app_name}
    PRIVATE
        encrypt/main.cpp
)

target_link_libraries(
    ${app_name}
    PRIVATE
        cga
        cxxopts
)