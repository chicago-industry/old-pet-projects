#!/bin/bash

rm -rf /tmp/key_sem_R
rm -rf /tmp/key_shm_R
rm -rf /tmp/key_msg_R

if [[ $(uname -v | awk -v n=1 '{print $n}') == 'Darwin' ]]; then

    mem="$(ipcs | awk '/Shared Memory/,/Semaphores/' | grep "yshawn" | awk -v n=2 '{print $n}')"

    IFS=$'\n' read -rd '' -a strarr <<<"$mem"
    for val in "${strarr[@]}";do
      printf "deleting shmmem with id: $val\n"
      ipcrm -m "$val"
    done

    mem="$(ipcs | awk '/Semaphores/,/EOF/' | grep "yshawn" | awk -v n=2 '{print $n}')"

    IFS=$'\n' read -rd '' -a strarr <<<"$mem"
    for val in "${strarr[@]}";do
      printf "deleting sem with id: $val\n"
      ipcrm -s "$val"
    done

    mem="$(ipcs | awk '/Message Queues/,/EOF/' | grep "yshawn" | awk -v n=2 '{print $n}')"

    IFS=$'\n' read -rd '' -a strarr <<<"$mem"
    for val in "${strarr[@]}";do
      printf "deleting msg queue with id: $val\n"
      ipcrm -q "$val"
    done

else
    ipcrm -a
fi