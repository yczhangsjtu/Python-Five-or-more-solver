#!/bin/bash


eb="........."
eb=`echo -e "normal\n0\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb"`

bd=`echo "$eb" | ./addrand.py`

echo "$bd"

while true; do
	if [[ "$bd" == *"over"* ]]; then
		break
	fi
	bd=`echo "$bd" | ./fomanl | ./cancel.py | ./addrand.py | ./cancel.py`
	echo "$bd"
done
