#!/bin/bash
x=1
while [ $x -le 5 ]
do
    nc -zv localhost 9034
    x=$(($x + 1))
done