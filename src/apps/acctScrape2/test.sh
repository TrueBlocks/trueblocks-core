#!/bin/sh

acctScrape2 0xff488fd296c38a24cccc60b43dd7254810dab64e --fmt json > json.txt
acctScrape2 0xff488fd296c38a24cccc60b43dd7254810dab64e --fmt csv  > csv.txt
acctScrape2 0xff488fd296c38a24cccc60b43dd7254810dab64e --fmt txt  > txt.txt
