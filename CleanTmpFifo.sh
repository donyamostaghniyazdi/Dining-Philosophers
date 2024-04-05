#!/bin/bash
Item=0
while [ $Item -lt $1 ]
do
	rm -f /tmp/fifo_request$Item
	rm -f /tmp/fifo_rlease$Item	
	let Item+=1	
done