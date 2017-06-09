#!/bin/bash

read tmp

while true; do
	read -r s
	if [[ $s == *"#7590AE"* ]]; then
		lct=${s:0:$((`expr index "$s" :`-1))}
		break
	fi
done

echo normal
echo 0

i=`expr index "$lct" ,`
x=$((${lct:0:$((i-1))}+50))
y=$((${lct:i}+50))
prd=55
offx=16
offy=30
x=$(($x+$offx))
y=$(($y+$offy))

xs=`seq $x $prd $(($x+$prd*8))`
ys=`seq $y $prd $(($y+$prd*8))`

for Y in $ys; do
	l=""
	for X in $xs; do
		r=`convert $tmp[1x1+$X+$Y] txt:`
		c=`expr match "$r" '.*\(#[A-F0-9][A-F0-9][A-F0-9][A-F0-9][A-F0-9][A-F0-9]\).*'`
		if [[ $c == "#FFFF00" ]]; then
			l="${l}y"
		elif [[ $c == "#FF0000" ]]; then
			l="${l}r"
		elif [[ $c == "#00FF00" ]]; then
			l="${l}g"
		elif [[ $c == "#0000FF" ]]; then
			l="${l}b"
		elif [[ $c == "#FF8000" ]]; then
			l="${l}o"
		elif [[ $c == "#00FFFF" ]]; then
			l="${l}d"
		elif [[ $c == "#FF00FF" ]]; then
			l="${l}p"
		else
			l="$l."
		fi
	done
	echo $l
done

rm $tmp
