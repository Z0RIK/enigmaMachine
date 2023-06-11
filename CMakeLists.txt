cmake_minimum_required (VERSION 3.8)

project (Enigma C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setup output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/bin") # .exe and .dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib") # .so and .dylib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib") # .lib and .a

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/deps)

add_executable (Enigma
  src/main.cpp
  src/enigma_machine.h
  src/enigma_machine.cpp
  src/enigma_rotor.cpp
  src/enigma_rotor.h
  src/enigma_gui.cpp
  src/enigma_gui.h
  src/enigma_global.h)

target_link_libraries(Enigma PRIVATE app_external_deps)

target_include_directories(Enigma PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/deps
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/include
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/gl/include
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/stb)

if(WIN32)
  set_target_properties(${PROJECT_NAME}
  PROPERTIES
      LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
      LINK_FLAGS_RELEASE "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
      LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
      LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows /ENTRY:mainCRTStartup"
  )
endif(WIN32)

file(
  COPY ${CMAKE_SOURCE_DIR}/Logo.png
  DESTINATION ${CMAKE_BINARY_DIR}/bin)