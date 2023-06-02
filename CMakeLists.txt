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

##foreach(FILE ${FILES})
##	set(srcFiles ${srcFiles} src/${FILE})
##endforeach()

add_executable (Enigma  ${srcFiles} ${ImGui} ${stb})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Enigma PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
