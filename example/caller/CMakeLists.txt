set(SRC_LIST callUserService.cpp ../user.pb.cc)

# set(SRC_LIST callFriendService.cpp ../friend.pb.cc )

add_executable(consumer ${SRC_LIST})

target_link_libraries(consumer mprpc protobuf)