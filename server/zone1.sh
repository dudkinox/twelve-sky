#!/bin/sh
cd /home/guitar/12sky/twelve-sky/server/service/bin
sleep 1
./ts2_centerd
sleep 1
./ts2_gamelogd
sleep 1
./ts2_player_userd
sleep 1
./ts2_logind
sleep 1
./ts2_relayd
sleep 1
./ts2_extrad
sleep 1
./ts2_zoned -f /home/guitar/12sky/twelve-sky/server/service/etc/zone/ts2_zoned_1.conf -l /home/guitar/12sky/twelve-sky/server/service/logs/ts2_zoned/ts2_zoned_1_
sleep 1