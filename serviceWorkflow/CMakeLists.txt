cmake_minimum_required(VERSION 3.0.0)
project(serviceBlog VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
include(CTest)
enable_testing()

find_library(LIBRT rt)
find_package(OpenSSL REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(workflow REQUIRED CONFIG HINTS ..)

message(STATUS  "WORKFLOW_LIB_DIR = ${WORKFLOW_LIB_DIR} JsonCpp_INCLUDE_DIR = ${JsonCpp_INCLUDE_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -fno-exceptions -Wno-invalid-offsetof -g")
include_directories(${OPENSSL_INCLUDE_DIR} ${WORKFLOW_INCLUDE_DIR} /usr/local/include/json/)
link_directories(${WORKFLOW_LIB_DIR} /usr/local/lib/)
set(WORKFLOW_LIB workflow pthread OpenSSL::SSL OpenSSL::Crypto jsoncpp)

add_executable(serviceBlog main.cc)
target_link_libraries(serviceBlog ${WORKFLOW_LIB})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

