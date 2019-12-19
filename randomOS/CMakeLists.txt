project(randomOS)

set(CMAKE_CXX_STANDARD 14)

add_executable(randomOS randomOS.cpp VirtualMemory.h VirtualMemory.cpp MemoryManager.h MemoryManager.cpp)