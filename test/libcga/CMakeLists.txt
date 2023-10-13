include(CompileOptions)
include(GoogleTest)

set(test_name tests)

add_executable(${test_name})

set_compile_options(${test_name})

target_sources(
    ${test_name}
    PRIVATE
        libcga/base_functions/base_functions.cpp
        libcga/base_functions/diffie_hellman.cpp
        libcga/encryption/shamir.cpp
        libcga/encryption/elgamal.cpp
)

target_link_libraries(
    ${test_name}
    PRIVATE
        cga
        GTest::gtest_main
)

gtest_discover_tests(${test_name})