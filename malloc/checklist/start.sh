#!/bin/bash

# Https://nouveau.freedesktop.org/Valgrind-mmt.html -- проверка на утечки

resdir="results"

lib_path=".."
lib_inc="../include"
lib_name="ft_malloc"
lib_full_name="libft_malloc.so"

cc="gcc"
flags="-l$lib_name -L$lib_path -I$lib_inc"

D0="test0"
D1="test1"
D2="test2"
D3="test3"
D4="test4"
D5="test5"
D6="test6"
D7="test7"
D8="test8"
D9="test9"
D10="test10"
D11="test11"
D=($D0 $D1 $D2 $D3 $D4 $D5 $D6 $D7 $D8 $D9 $D10 $D11)

function func_compile_run() {
	for i in $@; do
		echo "$i"
		$cc "$i.c" $flags
		if [ $? -ne 0 ];then
			return 1
		fi
		if [[ "$i" = "test"[0-2] ]];then
			LD_LIBRARY_PATH=$lib_path LD_PRELOAD=$lib_full_name time -v ./a.out &> $resdir/$i
		else
			LD_LIBRARY_PATH=$lib_path LD_PRELOAD=$lib_path/$lib_full_name ./a.out &> $resdir/$i
		fi
	done
}

rm -rf $resdir && mkdir -p $resdir

if ! func_compile_run "${D[@]}";then
	rm -rf a.out
	exit 1
fi

rm -rf a.out
