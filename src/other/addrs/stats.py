#!/usr/bin/python
from __future__ import print_function

import os
import sys
import glob

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

#os.system("bin/addrs") # 2>&1 | tee output-tmp/results")

if len(glob.glob1("sorted-zips","*.txt")) > 0:
    os.system("cd sorted-zips >/dev/null ; gzip -v *.txt ; cd - >/dev/null")
    os.system("cd finalized-zips >/dev/null ; gzip -v *.bin ; cd - >/dev/null")
    os.system("cd blooms-zips >/dev/null ; gzip -v *.bloom ; cd - >/dev/null")

eprint("Building stats...")

os.system("ls -l sorted/         | sed 's/$/|orig+/'        >output-tmp/file")
os.system("ls -l sorted/         | sed 's/$/|ascii+/'      >>output-tmp/file")
os.system("ls -l finalized/      | sed 's/$/|final+/'      >>output-tmp/file")
os.system("ls -l blooms/         | sed 's/$/|bloom+/'      >>output-tmp/file")
os.system("ls -l sorted-zips/    | sed 's/$/|sorted-zip+/' >>output-tmp/file")
os.system("ls -l finalized-zips/ | sed 's/$/|final-zip+/'  >>output-tmp/file")
os.system("ls -l blooms-zips/    | sed 's/staff/staff /' | sed 's/$/|bloom-zip+/'  >>output-tmp/file")
os.system("more output-tmp/file")

os.system("sort -u output-tmp/file | grep -v ^total >x ; cat x >output-tmp/file.a")
os.system("open -a /Applications/TextEdit.app output-tmp/file.a");
exit
os.system("cat output-tmp/file.a | cut -c28-36,50-1000 | grep ' 00' >output-tmp/file1")
os.system("more output-tmp/file.1");
os.system("cat output-tmp/file1  | sed 's/ 00/|00/' | sed 's/ //g' | tr '|' '\t' >output-tmp/file2")
os.system("more output-tmp/file.2");
os.system("cat output-tmp/file2  | awk '{print $2 $3 $1}' | sed 's/+/|/' >output-tmp/file3")
os.system("more output-tmp/file.3");
os.system("cat output-tmp/file3  | sed 's/.gz//' | sed 's/.bin/.bin|/' | sed 's/.txt/.txt|/' | sed 's/.bloom/.bloom|/' | tr '|' '\t' | sort -u >output-tmp/file4")
os.system("more output-tmp/file.4");
os.system("cat output-tmp/file4  | sed 's/.txt//' | sed 's/.bin//' | sed 's/.bloom//' >output-tmp/file5")
os.system("more output-tmp/file.5");
exit

os.system("cat output-tmp/file5 | grep ascii | grep -v zip | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/ascii.file")
os.system("cat output-tmp/file5 | grep bin   | grep -v zip | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/bin.file")
os.system("cat output-tmp/file5 | grep bloom | grep -v zip | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/bloom.file")
os.system("cat output-tmp/file5 | grep orig  | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/orig.file")
os.system("cat output-tmp/file5 | grep sorted-zip  | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/sorted-zip.file")
os.system("cat output-tmp/file5 | grep final-zip | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/final-zip.file")
os.system("cat output-tmp/file5 | grep bloom-zip  | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/bloom-zip.file")

os.system("find output-ascii | grep '00' | cut -d'/' -f2 | sed 's/^/.\/counts.1.sh /' | sed 's/.txt//' >counts.sh")
os.system("chmod uog+x counts* ; ./counts.sh | sort -u >output-tmp/counts.txt")

os.system("join output-tmp/orig.file output-tmp/ascii.file >output-tmp/f1")
os.system("join output-tmp/f1        output-tmp/bin.file   >output-tmp/f2")
os.system("join output-tmp/f2        output-tmp/bloom.file >output-tmp/f3")
os.system("join output-tmp/f3        output-tmp/sorted-zip.file  >output-tmp/f4")
os.system("join output-tmp/f4        output-tmp/final-zip.file  >output-tmp/f5")
os.system("join output-tmp/f5        output-tmp/bloom-zip.file  >output-tmp/f6")
os.system("join output-tmp/f6        output-tmp/counts.txt | tr ' ' ',' | tr '-' ','")
