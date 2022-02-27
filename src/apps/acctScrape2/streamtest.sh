
#!/bin/sh

acctScrape2 0xff488fd296c38a24cccc60b43dd7254810dab64e --fmt json > nostream.json --log --max 5
acctScrape2 0xff488fd296c38a24cccc60b43dd7254810dab64e --fmt json > stream.json --log --max 5 --stream
acctScrape2 0xff488fd296c38a24cccc60b43dd7254810dab64e --fmt csv > stream.csv --log --max 5 --stream
acctScrape2 0xff488fd296c38a24cccc60b43dd7254810dab64e --fmt csv > nostream.csv --log --max 5 
acctScrape2 0xff488fd296c38a24cccc60b43dd7254810dab64e --fmt txt > nostream.txt --log --max 5 
acctScrape2 0xff488fd296c38a24cccc60b43dd7254810dab64e --fmt txt > stream.txt --log --max 5 --stream

cat stream.txt | wc -l 
cat nostream.txt | wc -l 
cat stream.csv | wc -l 
cat nostream.csv | wc -l 
