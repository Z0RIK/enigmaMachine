# CMakeList.txt: проект CMake для Enigma; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("Enigma")

# Setup output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/bin") # .exe and .dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib") # .so and .dylib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$<0:>${CMAKE_BINARY_DIR}/lib") # .lib and .a

include(src/CMakeLists.txt)
include(external/imgui/CMakeLists.txt)
include(external/stb/CMakeLists.txt)

foreach(FILE ${FILES})
	set(srcFiles ${srcFiles} src/${FILE})
endforeach()

foreach(FILE ${ImGui})
  set(srcFiles ${srcFiles} external/imgui/${FILE})
endforeach()

foreach(FILE ${stb})
  set(srcFiles ${srcFiles} external/imgui/${FILE})
endforeach()

add_executable (Enigma  ${srcFiles})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Enigma PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
