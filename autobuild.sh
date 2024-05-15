#!/bin/bash
set -e  # 任何一行脚本失败将会立即退出,而不会继续向下执行
rm -rf `pwd`/build/*
cd `pwd`/build && cmake .. && make 
cd ..
cp -r `pwd`/src/include `pwd`/lib