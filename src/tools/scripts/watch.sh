#!/usr/bin/env bash
# usage: watch [command] [sleep duration]
while :; do
  clear
  pwd
  date -u
  bash -c "$1"
  echo "Sleeping for $2 seconds"
  sleep $2
done
