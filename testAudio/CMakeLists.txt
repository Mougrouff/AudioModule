cmake_minimum_required(VERSION 3.25)
project(testAudio)

set(CMAKE_CXX_STANDARD 17)
set(portaudio_DIR "third-party/portaudio")

include_directories(pa_includes)
find_package(portaudio)

add_executable(
        testAudio main.cpp
        source/Module.cpp source/Module.h source/Port.cpp source/Port.h
        source/OutputModule.cpp source/OutputModule.h
        source/SoundEngine.cpp source/SoundEngine.h source/SoundUtils.h
        source/VCOModule.cpp source/VCOModule.h)

target_link_libraries(testAudio portaudio portaudio_static)