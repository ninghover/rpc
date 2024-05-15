set(SRC_LIST userService.cpp ../user.pb.cc)
# set(SRC_LIST friendService.cpp ../friend.pb.cc )

add_executable(provider ${SRC_LIST})

target_link_libraries(provider mprpc protobuf )