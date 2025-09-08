#!/bin/bash

HOST="127.0.0.1"
PORT=6667
CHANNEL="#42"

# Kaç client bağlanacak?
NUM_CLIENTS=5
PASS=pass

for i in $(seq 1 $NUM_CLIENTS); do
  (
    NICK="test$i"
    USER="user$i"
    {
      sleep 1
      echo "PASS $PASS"
      echo "NICK $NICK"
      echo "USER $USER"
      sleep 1
      echo "JOIN $CHANNEL"
      sleep 1
      echo "PRIVMSG $CHANNEL :Hello from $NICK"
      sleep 2
    } | nc $HOST $PORT
  ) &
done

wait
