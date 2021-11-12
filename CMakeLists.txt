cmake_minimum_required (VERSION 3.0.0)
project(LeanHRPT-Demod)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE CXX_SOURCE_FILES src/*.cpp src/*.c)
IF (WIN32)
add_executable(LeanHRPT-Demod WIN32 ${CXX_SOURCE_FILES})
ELSE()
add_executable(LeanHRPT-Demod ${CXX_SOURCE_FILES})
ENDIF()

set_property(TARGET LeanHRPT-Demod PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET LeanHRPT-Demod PROPERTY CXX_STANDARD 11)

target_include_directories(LeanHRPT-Demod PUBLIC src)

target_compile_options(LeanHRPT-Demod PRIVATE -Wall -Wextra -Wpedantic -ffast-math)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
target_link_libraries(LeanHRPT-Demod PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Bodge fix for GitHub Actions
if (WAR_CRIMES)
    target_link_directories(LeanHRPT-Demod PRIVATE /tmp/build/lib)
    target_link_directories(LeanHRPT-Demod PRIVATE /tmp/winbuild/lib)
endif()

target_link_libraries(LeanHRPT-Demod PRIVATE correct)
target_link_libraries(LeanHRPT-Demod PRIVATE sndfile)
if(WIN32 AND WAR_CRIMES)
target_link_libraries(LeanHRPT-Demod /tmp/winbuild/lib/libfftw3f-3.dll)
ELSE()
target_link_libraries(LeanHRPT-Demod PRIVATE fftw3f)
ENDIF()
target_link_libraries(LeanHRPT-Demod PRIVATE SoapySDR)

if (USE_ADDRESSS_SANITIZER)
    target_compile_options(LeanHRPT-Demod PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(LeanHRPT-Demod PRIVATE -fsanitize=address)
endif()

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(LeanHRPT-Demod PRIVATE OpenMP::OpenMP_CXX)
endif()

find_package(Git)
if (GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --tag WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} OUTPUT_VARIABLE GIT_TAG OUTPUT_STRIP_TRAILING_WHITESPACE)
    target_compile_definitions(LeanHRPT-Demod PRIVATE "VERSION=\"${GIT_TAG}\"")
else()
    target_compile_definitions(LeanHRPT-Demod PRIVATE "VERSION=\"Unknown\"")
endif()
