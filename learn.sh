#!/bin/bash

function test_para {
	bd=`./fomanl -ae -i $i`
	score=`echo "$bd" | grep -E '[0-9]+'`
}

function update_para {
	param=`cat param`
	nparam=`echo $param | ./disturb.py`
	if [[ "$curr_score" == "" ]]; then
		echo "Testing current parameter"
		echo $param
		repeat_test
		curr_score=$test_score
	fi
	echo "curr_score: $curr_score"
	echo "curr_param: $param"
	echo "Testing updated parameter"
	echo "$nparam" > param
	echo "new_param: $nparam"
	repeat_test
	if [ "$curr_score" -gt "$test_score" ]; then
		echo "New parameter is worse, change back"
		echo "$param" > param
	else
		echo "New paramter is better"
		curr_score=$test_score
	fi
}

function learn {
	curr_score=""
	for i in `seq 1 20`; do
		update_para
	done
}

function repeat_test {
	test_score=""
	for i in `seq 1 20`; do
		printf "Test $i "
		test_para
		echo "Score: $score"
		test_score="$test_score $score"
	done
	test_score=`echo $test_score | ./mid.py`
	echo "Mid: $test_score"
}

eb="........."
eb=`echo -e "normal\n0\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb\n$eb"`
# echo "$eb" | ./addrand.py
repeat_test
# learn
