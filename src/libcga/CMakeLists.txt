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
        libcga/base_functions/base_functions.cpp
        libcga/base_functions/diffie_hellman.cpp
        libcga/encryption/shamir.cpp
        libcga/encryption/elgamal.cpp
)

target_include_directories(
    ${lib_name}
    PUBLIC
        .
)