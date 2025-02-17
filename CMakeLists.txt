cmake_minimum_required(VERSION 3.30)
project(Tile)

set(CMAKE_C_STANDARD 99) # Set the C99 standard to C99
set(CMAKE_C_STANDARD_REQUIRED TRUE) # Ensure the compiler supports the specified standard

# --- start raylib ---
# find raylib or fetch it
# Dependencies
#set(RAYLIB_VERSION 5.0)
#find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
#if (NOT raylib_FOUND) # If there's none, fetch and build raylib
#  include(FetchContent)
#  FetchContent_Declare(
#    raylib
#    DOWNLOAD_EXTRACT_TIMESTAMP OFF
#    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
#  )
#  FetchContent_GetProperties(raylib)
#  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
#    set(FETCHCONTENT_QUIET NO)
#    FetchContent_MakeAvailable(raylib)
#    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
#  endif()
#endif()

# Web Configurations
#if (${PLATFORM} STREQUAL "Web")
#    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
#endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
#if (APPLE)
#    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
#    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
#    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
#endif()
# --- end raylib ---




set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)

set(sources
        ${PROJECT_SOURCE_DIR}/src/main.c
            
        ${PROJECT_SOURCE_DIR}/src/board_input_impl.c
        ${PROJECT_SOURCE_DIR}/src/board_move_impl_little_endian.c
        ${PROJECT_SOURCE_DIR}/src/board_render_impl.c
        ${PROJECT_SOURCE_DIR}/src/board_solve_impl.c
)

if(CMAKE_HOST_WIN32)
    add_executable(${PROJECT_NAME} ${sources})
else()

endif()
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE -TC -Wall)


#target_include_directories(${PROJECT_NAME} PUBLIC
#                           "${PROJECT_BINARY_DIR}"
#                           )
                           
target_include_directories(${PROJECT_NAME}
        PRIVATE
            ${PROJECT_INCLUDE_DIR}
)
      
#set(raylib_VERBOSE 1)      
target_link_libraries(${PROJECT_NAME}
        PRIVATE
            #raylib
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)