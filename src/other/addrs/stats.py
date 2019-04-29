#!/usr/bin/python
from __future__ import print_function

import os
import sys
import glob

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

os.system("bin/addrs") # 2>&1 | tee output-tmp/results")

if len(glob.glob1("output-azips","*.txt")) > 0:
    os.system("cd output-azips >/dev/null ; gzip -v *.txt ; cd - >/dev/null")
    os.system("cd output-bzips >/dev/null ; gzip -v *.bin ; cd - >/dev/null")
    os.system("cd output-czips >/dev/null ; gzip -v *.bloom ; cd - >/dev/null")

eprint("Building stats...")

os.system("ls -l sorted/        | sed 's/$/|orig+/'   >output-tmp/file")
os.system("ls -l output-ascii/  | sed 's/$/|ascii+/' >>output-tmp/file")
os.system("ls -l output-bin/    | sed 's/$/|bin+/'   >>output-tmp/file")
os.system("ls -l output-blooms/ | sed 's/$/|bloom+/' >>output-tmp/file")
os.system("ls -l output-azips/  | sed 's/$/|azip+/'  >>output-tmp/file")
os.system("ls -l output-bzips/  | sed 's/$/|bzip+/'  >>output-tmp/file")
os.system("ls -l output-czips/  | sed 's/staff/staff /' | sed 's/$/|czip+/'  >>output-tmp/file")
#os.system("cat output-tmp/file | grep czip")

os.system("sort -u output-tmp/file | grep -v ^total >x ; cat x >output-tmp/file.a")
#os.system("cat output-tmp/file.a | grep czip")
os.system("cat output-tmp/file.a | cut -c28-36,50-1000 | grep ' 00' >output-tmp/file1")
os.system("cat output-tmp/file1  | sed 's/ 00/|00/' | sed 's/ //g' | tr '|' '\t' >output-tmp/file2")
os.system("cat output-tmp/file2  | awk '{print $2 $3 $1}' | sed 's/+/|/' >output-tmp/file3")
os.system("cat output-tmp/file3  | sed 's/.gz//' | sed 's/.bin/.bin|/' | sed 's/.txt/.txt|/' | sed 's/.bloom/.bloom|/' | tr '|' '\t' | sort -u >output-tmp/file4")
os.system("cat output-tmp/file4  | sed 's/.txt//' | sed 's/.bin//' | sed 's/.bloom//' >output-tmp/file5")

os.system("cat output-tmp/file5 | grep ascii | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/ascii.file")
os.system("cat output-tmp/file5 | grep bin   | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/bin.file")
os.system("cat output-tmp/file5 | grep bloom | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/bloom.file")
os.system("cat output-tmp/file5 | grep orig  | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/orig.file")
os.system("cat output-tmp/file5 | grep azip  | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/azip.file")
os.system("cat output-tmp/file5 | grep bzip  | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/bzip.file")
os.system("cat output-tmp/file5 | grep czip  | awk '{print $1,$3}' OFS=, | tr ',' '\t' >output-tmp/czip.file")

os.system("find output-ascii | grep '00' | cut -d'/' -f2 | sed 's/^/.\/counts.1.sh /' | sed 's/.txt//' >counts.sh")
os.system("chmod uog+x counts* ; ./counts.sh | sort -u >output-tmp/counts.txt")

os.system("join output-tmp/orig.file output-tmp/ascii.file >output-tmp/f1")
os.system("join output-tmp/f1        output-tmp/bin.file   >output-tmp/f2")
os.system("join output-tmp/f2        output-tmp/bloom.file >output-tmp/f3")
os.system("join output-tmp/f3        output-tmp/azip.file  >output-tmp/f4")
os.system("join output-tmp/f4        output-tmp/bzip.file  >output-tmp/f5")
os.system("join output-tmp/f5        output-tmp/czip.file  >output-tmp/f6")
os.system("join output-tmp/f6        output-tmp/counts.txt | tr ' ' ',' | tr '-' ','")
