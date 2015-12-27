#!/bin/bash

function test_para {
	eb="........."
	eb=`echo -e "normal\n0\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb"`
	bd=`echo "$eb" | ./addrand.py`
	while true; do
		if [[ "$bd" == *"over"* ]]; then
			break
		fi
		bd=`echo "$bd" | ./fomanl | ./cancel.py | ./addrand.py #| ./cancel.py`
	done
	score=`echo "$bd" | grep -E '[0-9]+'`
}

function update_para {
	param=`cat param`
	nparam=`echo $param | ./disturb.py`
	if [[ "$curr_score" == "" ]]; then
		echo "Testing current parameter"
		echo $param
		s=""
		for i in `seq 1 10`; do
			printf "Test $i "
			test_para
			echo "Score: $score"
			s="$s $score"
		done
		s=`echo $s | ./mid.py`
		echo "Mid: $s"
		curr_score=$s
	fi
	echo "curr_score: $curr_score"
	echo "curr_param: $param"
	echo "Testing updated parameter"
	echo "$nparam" > param
	echo "new_param: $nparam"
	t=0
	for i in `seq 1 10`; do
		printf "Test $i "
		test_para
		echo "Score: $score"
		t="$t $score"
		if [[ "$i" -gt 5 ]]; then
			maxcurr=`echo $t | ./currmax.py`
			if [[ "$maxcurr" -lt "$curr_score" ]]; then
				break
			fi
		fi
	done
	t=`echo $t | ./mid.py`
	echo "Mid: $t"
	if [ "$curr_score" -gt "$t" ]; then
		echo "New parameter is worse, change back"
		echo "$param" > param
	else
		echo "New paramter is better"
		curr_score=$t
	fi
}

curr_score=""
for i in `seq 1 20`; do
	update_para
done
