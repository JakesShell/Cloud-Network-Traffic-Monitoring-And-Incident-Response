cmake_minimum_required(VERSION 3.16)
project(NetworkTrafficSecurityMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(network_traffic_security_monitor
    main.cpp
    TrafficMonitor.cpp
)
