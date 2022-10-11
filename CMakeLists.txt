cmake_minimum_required (VERSION 3.13)
project(LeanHRPT-Demod)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE CXX_SOURCE_FILES src/*.cpp src/*.c)
if(WIN32)
    add_executable(LeanHRPT-Demod WIN32 ${CXX_SOURCE_FILES})
else()
    add_executable(LeanHRPT-Demod ${CXX_SOURCE_FILES})
endif()

set_property(TARGET LeanHRPT-Demod PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET LeanHRPT-Demod PROPERTY CXX_STANDARD 11)

target_include_directories(LeanHRPT-Demod PUBLIC src)

if (NOT MSVC)
    target_compile_options(LeanHRPT-Demod PRIVATE -Wall -Wextra -Wpedantic -ffast-math)

    # If you CPU doesn't have SSE 4.1 then get a newer CPU
    target_compile_options(LeanHRPT-Demod PRIVATE -msse4.1)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
target_link_libraries(LeanHRPT-Demod PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Bodge fix for GitHub Actions
if (WAR_CRIMES)
    target_link_directories(LeanHRPT-Demod PRIVATE /tmp/build/lib)
    target_link_directories(LeanHRPT-Demod PRIVATE /tmp/winbuild/lib)
endif()

if(MSVC)
    target_link_directories(LeanHRPT-Demod PRIVATE "C:/Program Files/PothosSDR/lib/")
    target_include_directories(LeanHRPT-Demod PRIVATE "C:/Program Files/PothosSDR/include/")
    target_link_directories(LeanHRPT-Demod PRIVATE "C:/Program Files (x86)/Correct/lib/")
    target_include_directories(LeanHRPT-Demod PRIVATE "C:/Program Files (x86)/Correct/include/")
    target_link_directories(LeanHRPT-Demod PRIVATE "C:/Program Files (x86)/fftw/lib/")
    target_include_directories(LeanHRPT-Demod PRIVATE "C:/Program Files (x86)/fftw/include/")
    target_link_directories(LeanHRPT-Demod PRIVATE "C:/Program Files/libsndfile/lib/")
    target_include_directories(LeanHRPT-Demod PRIVATE "C:/Program Files/libsndfile/include/")
endif()

target_link_libraries(LeanHRPT-Demod PRIVATE correct)
target_link_libraries(LeanHRPT-Demod PRIVATE sndfile)
target_link_libraries(LeanHRPT-Demod PRIVATE fftw3f)
target_link_libraries(LeanHRPT-Demod PRIVATE SoapySDR)

if(USE_ADDRESS_SANITIZER)
    target_compile_options(LeanHRPT-Demod PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(LeanHRPT-Demod PRIVATE -fsanitize=address)
endif()

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(LeanHRPT-Demod PRIVATE OpenMP::OpenMP_CXX)
endif()

# Get version from git
find_package(Git)
if (GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --tag WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} OUTPUT_VARIABLE GIT_TAG OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(VERSION "${GIT_TAG}")
else()
    set(VERSION "Unknown")
endif()
target_compile_definitions(LeanHRPT-Demod PRIVATE "VERSION=\"${VERSION}\"")

option(EXPERIMENTAL "Enable experimental features" OFF)
if(EXPERIMENTAL)
    target_compile_definitions(LeanHRPT-Demod PRIVATE -DEXPERIMENTAL)
    target_compile_options(LeanHRPT-Demod PRIVATE -march=native)
endif()

# Binaries
install(TARGETS LeanHRPT-Demod)
# Desktop file and icon
install(FILES ${CMAKE_SOURCE_DIR}/LeanHRPT-Demod.desktop DESTINATION share/applications)
install(FILES ${CMAKE_SOURCE_DIR}/logo128.png DESTINATION share/icons/hicolor/128x128/apps RENAME LeanHRPT-Demod.png)
install(FILES ${CMAKE_SOURCE_DIR}/logo.svg DESTINATION share/icons/hicolor/scalable/apps RENAME LeanHRPT-Demod.svg)

# Package information
string(SUBSTRING ${VERSION} 1 -1 CPACK_PACKAGE_VERSION)
set(CPACK_PACKAGE_NAME "LeanHRPT-Demod")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An easy to use HRPT demodulator")
set(CPACK_PACKAGE_DESCRIPTION "LeanHRPT is an easy to use and powerful tool for the manipulation of Level 0 HRPT data.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/Xerbo/LeanHRPT-Demod")
set(CPACK_PACKAGE_CONTACT "Xerbo (xerbo@protonmail.com)")
# Debian
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libcorrect")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

# Only allow packaging if we know what we are packaging
if (NOT VERSION STREQUAL "Unknown")
    include(CPack)
endif()
