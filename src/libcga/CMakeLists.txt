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
        libcga/encryption/rsa.cpp
        libcga/encryption/vernam.cpp
        libcga/signature/rsa.cpp
        libcga/signature/elgamal.cpp
        libcga/signature/gost.cpp
        libcga/mental_poker/table.cpp
        libcga/mental_poker/user.cpp
        libcga/blind_signature/client.cpp
        libcga/blind_signature/client.hpp
        libcga/blind_signature/server.cpp
        libcga/blind_signature/server.hpp
)

target_include_directories(
    ${lib_name}
    PUBLIC .
    SYSTEM PRIVATE ${CRYPTOPP_INCLUDE_DIRS}
)

target_link_libraries(
    ${lib_name}
        ${cryptopp_lib}
)