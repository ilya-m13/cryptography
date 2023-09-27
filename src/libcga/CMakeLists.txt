set(lib_name cga)

add_library(
    ${lib_name}
    STATIC
)

include(CompileOptions)
set_compile_options(${lib_name})

target_sources(
    ${lib_name}
    PRIVATE
        libcga/cryptosystems/cryptosystems.cpp
        libcga/cryptosystems/df_key_exchange.cpp
)

target_include_directories(
    ${lib_name}
    PUBLIC
        .
)