cmake_minimum_required(VERSION 3.15)

project(MSPDBX)

include (CBuildKit)

option(USE_STATIC_LINKING "Use static linking for the runtime" ON)

if (USE_STATIC_LINKING)
    if (MSVC)
        # MSVC specific static linking flag
        add_compile_options(/MT$<$<CONFIG:Debug>:d>)
    else()
        # GCC/Clang specific static linking flag
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    endif()
endif()

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