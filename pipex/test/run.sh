#!/bin/bash

# colours
RED="\033[1;31m"
GREEN="\033[1;32m"
ORANGE="\033[1;33m"
BLUE="\033[1;34m"
CYAN="\033[1;36m"
NC="\033[0m"

# tr -dc "A-Za-z 0-9" < /dev/urandom | fold -w100|head -n 100000 > bigfile.txt

# name of prog
prog="../pipex"

# input file
file_in="../src/main.c"

# output from pipex
file_out="output"

# original output from bash
file_out_orig="output_orig"

chmod 777 $file_in

# waits till user press any key
function wait_for_stdin()
{
    while [ true ] ; do
    	read -t 3 -n 1
    	if [ $? = 0 ] ; then
			break ;
		fi
	done
}

# prints current command that should be runned
function print_command()
{
	wait_for_stdin
	echo -en ${ORANGE}
	echo -n "CURRENT COMMAND FOR PIPEX: "
	echo -en ${BLUE}
	echo "$@"
	echo -en ${NC}
}

function parse_run()
{
	n=0
	final=()

	print_command "${@}"

	echo -en ${ORANGE}
	echo -n "$prog output:"
	echo -en ${CYAN}
	echo "================================|"
	echo -en ${NC}

	# run pipex
	eval "$prog $file_in ${@} $file_out"
	err=$?

	echo -en ${ORANGE}
	echo -n "ORIG output:"
	echo -en ${CYAN}
	echo "===================================|"
	echo -en ${NC}

	# run bash
	eval "for word in "${@}"; do if (( n != 0 )); then final+=${final}' | '\$word; else final+=${final}' '\$word; fi; (( n++ )); done"
	eval "< $file_in $final > $file_out_orig"
	err=$?

	echo -en ${CYAN}
	echo "===============================================|"
	if [[ $err -eq 0 ]];then
		diff --normal $file_out $file_out_orig &>/dev/null
		if [[ $? -ne 0 ]]; then
			echo -en ${RED}
			echo "DIFF IS DIFFERENT"
		else
			echo -en ${GREEN}
			echo "DIFF IS OK"
		fi
	fi
	echo -en ${NC}
}

function cycle_for_commands()
{
	arr=("$@")

	for (( c=0; c<=${#arr[@]}; c++ ));do
		if [ -z "${arr[c]}" ];then
			break
		fi
		parse_run ${arr[c]}
	done
}

# STANDART TEST HERE -----------------------------------------------------------

# prepare
chmod 777 $file_in
make -C ..

declare -a array=(
'"cat" "grep error"'
'"cat" "salam"'
)

# 1 standart

cycle_for_commands "${array[@]}"

# 2 rights

echo
echo -en ${RED}
echo "chmod 000 $file_in"
echo -en ${NC}
chmod 000 $file_in
cycle_for_commands "${array[@]}"
chmod 777 $file_in

# 2 unset PATH from env

source ~/.zshrc
OLD_PATH=$PATH

echo
echo -en ${RED}
echo "unset PATH"
unset PATH
echo -en ${NC}
cycle_for_commands "${array[@]}"
PATH=$OLD_PATH
source ~/.zshrc

# 3 BONUS TEST HERE ------------------------------------------------------------

# prepare
chmod 777 $file_in
# make -C .. bonus
file_in="bigfile.txt"

declare -a array=(
'"cat" "cat" "cat" "cat" "grep 1___________________1"'
'"cat" "cat" "cat" "cat" "grep 1___________________1" "salam"'
)

# 1 multipipes

echo
echo -en ${RED}
echo "BONUSES: MULTIPIPES"
echo -en ${NC}
cycle_for_commands "${array[@]}"

# FINISHED ---------------------------------------------------------------------
chmod 777 $file_in
rm -rf $file_out
rm -rf $file_out_orig