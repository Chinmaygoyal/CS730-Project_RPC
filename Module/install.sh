#! /bin/bash

make
sudo rmmod sfork.ko
sudo insmod sfork.ko

sudo dmesg -c

