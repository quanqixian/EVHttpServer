#!/bin/bash
CURRENT_PATH=`pwd`

#############################
#func  :编译安装googletest
#############################
function build()
{
	#下载源码
	git clone --depth 1 --branch release-2.1.12-stable https://github.com/libevent/libevent.git

	#创建安装目录install
	mkdir install

	#进入源码目录
	cd libevent

	#创建编译目录build
	mkdir build

	#编译安装
	cd build
	cmake -DCMAKE_INSTALL_PREFIX=../../install ..
	make
	make install

}

#############################
#func  :清除编译
#############################
function clean()
{
	rm -rf ./libevent
	rm -rf ./install/
}

#########################
#func: 菜单
#########################
function menu()
{

	if [ $# -ne 1 ]
	then
		echo "usage : $0 build/clean"
		exit
	fi

	if   [ $1 = "build" ]
	then
		build
	elif [ $1 = "clean" ]
	then
		clean
	else
		echo "usage : $0 build/clean"
	fi
}

function main()
{
	menu $*
}
main $*
