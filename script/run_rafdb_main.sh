#!/bin/bash
mkdir -p log
HOME_DIR=`pwd`
./rafdb_main --v=6 --check_interval=7200 --thread_num=23 --rafdb_list=192.168.14.146:10020:1,192.168.14.146:10021:2,192.168.14.146:10022:3 --rafdb_self=192.168.14.146:10020:1 --logfile=$HOME_DIR/log/db1.log 1>$HOME_DIR/log/db1.log 2>&1 &
./rafdb_main --v=6 --check_interval=7200 --thread_num=23 --rafdb_list=192.168.14.146:10020:1,192.168.14.146:10021:2,192.168.14.146:10022:3 --rafdb_self=192.168.14.146:10021:2 --logfile=$HOME_DIR/log/db2.log 1>$HOME_DIR/log/db2.log 2>&1 &
./rafdb_main --v=6 --thread_num=23 --rafdb_list=192.168.14.146:10020:1,192.168.14.146:10021:2,192.168.14.146:10022:3 --rafdb_self=192.168.14.146:10022:3 --logfile=$HOME_DIR/log/db3.log 1>$HOME_DIR/log/db3.log 2>&1 &
