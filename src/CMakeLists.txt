aux_source_directory(./ SRC_LIST)

add_library(mprpc ${SRC_LIST})      #使用的muduo_net库是静态库，我们如果想生成动态库就会有问题

target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)