#!/bin/sh

WORLD_NUM=1
ZONE_START_NUM=1
ZONE_END_NUM=193
MAX_USER_NUM=1000
PLAYER_IP=10.1.35.12
PLAYER_PORT=11011
CENTER_IP=10.1.35.12
CENTER_PORT=11051
RELAY_IP=10.1.35.12
RELAY_PORT=11071
EXTRA_IP=10.1.35.12
EXTRA_PORT=11031
LOG_IP=10.1.35.12
LOG_PORT=11111
LEVEL_LIMIT=112

ZONE_CONF_DIR=/home/guitar/12sky/twelve-sky/server/service/etc

while [ $ZONE_START_NUM -le $ZONE_END_NUM ]; do

echo "ZONE #$ZONE_START_NUM"
ZONE_NUM=$ZONE_START_NUM
ZONE_PORT=$(($ZONE_START_NUM + 11600))
WORLD_NUM=$WORLD_NUM

sed -e "s/##ZONE_NUM##/$ZONE_NUM/g" $ZONE_CONF_DIR/ts2_zoned.conf.in | sed -e "s/##ZONE_PORT##/$ZONE_PORT/g" | sed -e "s/##WORLD_NUM##/$WORLD_NUM/g" | sed -e "s/##PLAYER_IP##/$PLAYER_IP/g"  | sed -e "s/##PLAYER_PORT##/$PLAYER_PORT/g"  | sed -e "s/##CENTER_IP##/$CENTER_IP/g" | sed -e "s/##CENTER_PORT##/$CENTER_PORT/g"  | sed -e "s/##RELAY_IP##/$RELAY_IP/g" | sed -e "s/##RELAY_PORT##/$RELAY_PORT/g" | sed -e "s/##EXTRA_IP##/$EXTRA_IP/g" | sed -e "s/##EXTRA_PORT##/$EXTRA_PORT/g" | sed -e "s/##LOG_IP##/$LOG_IP/g" | sed -e "s/##LOG_PORT##/$LOG_PORT/g" | sed -e "s/##MAX_USER_NUM##/$MAX_USER_NUM/g" | sed -e "s/##LEVEL_LIMIT##/$LEVEL_LIMIT/g" > $ZONE_CONF_DIR/ts2_zoned_$ZONE_NUM.conf

ZONE_START_NUM=$(($ZONE_START_NUM + 1))
done
