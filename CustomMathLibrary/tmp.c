#!/bin/bash
for i in exp sin cos tan asin acos atan
do
./a.out | grep "^$i" | awk '{print $2, $8}' > /tmp/$i
done
gnuplot <<XX
set terminal jpeg
set output "trig.jpg"
plot "/tmp/sin" with linespoints, "/tmp/cos" with linespoints
set output "tan.jpg"
plot "/tmp/tan" with linespoints
set output "exp.jpg"
plot "/tmp/exp" with linespoints
set output "arc.jpg"
plot "/tmp/asin" with linespoints, "/tmp/acos" with linespoints
set output "atan.jpg"
plot "/tmp/atan" with linespoints
XX
