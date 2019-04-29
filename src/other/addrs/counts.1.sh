echo "$1|" | tr '|' '\t' | tr '\n' ' '
head -1 output-ascii/$1.txt | cut -f3,4
