#/!bin/bash

for i in /dev/ttyUSB*; do
    if avrdude -P $i -p m328p -c arduino ; then
	echo $i
	exit 0
    fi
done
exit 1
