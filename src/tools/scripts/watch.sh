#!/usr/bin/env bash
# usage: watch [command] [sleep duration]
while :; do
  clear
  date
  bash -c "$1"
  sleep $2
done
