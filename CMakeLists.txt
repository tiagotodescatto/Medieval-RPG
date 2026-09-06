cmake_minimum_required (VERSION 3.10)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Medieval RPG")

set(VCPKG_MANIFEST_MODE ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable (MedievalRPG "main.cpp")

target_link_libraries(MedievalRPG PRIVATE fmt::fmt)
target_link_libraries(MedievalRPG PRIVATE glfw)     
target_link_libraries(MedievalRPG PRIVATE glad::glad)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MedievalRPG PROPERTY CXX_STANDARD 20)
endif()