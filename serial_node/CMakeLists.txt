cmake_minimum_required(VERSION 3.0.2)
project(serial_node)
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  serial
  roscpp
  rospy
  std_msgs
  message_generation
)
add_message_files(
        FILES
        BoundingBox.msg
)

generate_messages(
        DEPENDENCIES
        std_msgs
)

catkin_package(
        #  INCLUDE_DIRS include
        #  LIBRARIES tr_move_pkg
        CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
        #  DEPENDS system_lib
)

include_directories(
        include
            ${catkin_INCLUDE_DIRS}
)

add_executable(serial_node
        src/serial_node.cpp)
target_link_libraries(serial_node
        ${catkin_LIBRARIES}
        )
