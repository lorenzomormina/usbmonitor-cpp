cmake_minimum_required(VERSION 3.27)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(main
    main.cpp
    Device.cpp
    Device.h
    Target.cpp
    Target.h
    USBMonitor.cpp
    USBMonitor.h
    lmlib/container.h
    lmlib/string.cpp
    lmlib/string.h
    lmlib/system.cpp
    lmlib/system.h
)

target_link_libraries(main Setupapi.lib)