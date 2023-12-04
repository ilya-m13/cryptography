set(CRYPTOPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "cryptopp include dirs")
set(cryptopp_lib ${CMAKE_CURRENT_SOURCE_DIR}/cryptopp/libcryptopp.a CACHE FILEPATH "cryptopp lib")

message(STATUS "Cryptopp building")
execute_process(WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cryptopp
                COMMAND make
                ERROR_VARIABLE CRYPTOPP_ERRORS)
message(STATUS "Cryptopp building - done")