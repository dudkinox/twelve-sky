#!/bin/bash
NOW=$(date +"%m-%d-%Y %H:%M")
echo "backup run!"
mysqldump -u backuper ts2_gamedb > /user/mysql_backups/ts2_game_$NOW.sql
echo "backup end!"


