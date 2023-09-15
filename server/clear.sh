#!/bin/sh
cd /user/service/logs/ts2_centerd &&  rm -rf *.log
cd /user/service/logs/ts2_extrad && rm -rf *.log
cd /user/service/logs/ts2_gamelogd && rm -rf *.log
cd /user/service/logs/ts2_logind && rm -rf *.log
cd /user/service/logs/ts2_player_userd && rm -rf *.log
cd /user/service/logs/ts2_relayd && rm -rf *.log
cd /user/service/logs/ts2_zoned && rm -rf *.log
