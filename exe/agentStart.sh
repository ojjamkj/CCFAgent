#!/bin/sh

cd /home/ojjamkj/CAgent
nohup ./cfagent 1>/dev/null 2>&1 &
sleep 1
ps -ef|grep cfagent
