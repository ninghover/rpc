# rpc分布式通信框架
rpc调用原理实现
protobuf数据序列化和反序列化
zookeeper分布式一致性协调服务应用
muduo网络库
conf配置文件读取
异步日志
cmake构建集成编译环境

# 运行方式及测试

1.执行 `autobuild.sh` 文件 : `./autobuild.sh`
2.启动 `zookeeper` 服务器 : `cd /opt/zookeeper-3.4.10/bin/` , 执行 `./zkServer.sh start`
3.启动provider : `./provider -i test.conf`
4.启动consumer : `./consumer -i test.conf`