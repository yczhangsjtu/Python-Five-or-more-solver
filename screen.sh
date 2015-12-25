#!/bin/bash

tmp=/tmp/$(uuidgen).png
gnome-screenshot -f $tmp
echo $tmp
convert $tmp[50x50+50+50] txt:
