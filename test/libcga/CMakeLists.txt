include(CompileOptions)
include(GoogleTest)

set(test_name tests)

add_executable(${test_name})

set_compile_options(${test_name})

target_sources(
    ${test_name}
    PRIVATE
        libcga/cryptosystems/cryptosystems.test.cpp
        libcga/cryptosystems/df_key_exchange.cpp
)

target_link_libraries(
    ${test_name}
    PRIVATE
        cga
        GTest::gtest_main
)

gtest_discover_tests(${test_name})