#!/usr/bin/env python

import os
os.system('cat tests/in.txt tests/out.txt                  >tests/results.txt')
os.system('echo "Testing"                                 >>tests/results.txt')
os.system('whenBlock --file:tests/in.txt  | tee x ; cat x >>tests/results.txt')
os.system('echo "-----------------------" | tee x ; cat x >>tests/results.txt')
os.system('whenBlock --file:tests/out.txt | tee x ; cat x >>tests/results.txt')
os.system('rm -f x')

exit(0)
