cmake_minimum_required(VERSION 3.5)

project(do_control)

include_directories("./")
# set(target_sources do_control
#     main.cpp 
#     Creature.cpp 
#     Monster.cpp 
#     Player.cpp
# )
add_executable(do_control main.cpp RTMode.cpp SSMode.cpp)