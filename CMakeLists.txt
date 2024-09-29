cmake_minimum_required(VERSION 3.15)

project(MSPDBX)

include (CBuildKit)

file(GLOB MSPDBX_SRC src/*.cpp)

add_library_ns(mspdbx mspdbx STATIC ${MSPDBX_SRC})
target_include_dir_iface(mspdbx-mspdbx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include include)
target_compile_features(mspdbx-mspdbx PUBLIC cxx_std_17)
target_link_libraries(mspdbx-mspdbx wininet)
install_target_and_headers (mspdbx mspdbx)

add_subdirectory(examples)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include)

configure_file(cmake/MSPDBXConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MSPDBXConfig.cmake
    @ONLY)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MSPDBXConfig.cmake
    DESTINATION lib/cmake/mspdbx)