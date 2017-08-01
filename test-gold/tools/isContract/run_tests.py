#!/usr/bin/env python

import os
os.system('echo "-d option" | tee tests/results.txt')
os.system('isContract --nocolors 0x000001f568875f378bf6d170b790967fe429c81a -d 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors 0x01d04122aeccab138a47103b500f7b3d3b356a27 -d 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "-n option" | tee -a tests/results.txt')
os.system('isContract --nocolors 0x05f65ab88dc474a83a98994291c7a0abd13c9ae1 -n 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors 0x0a43edfe106d295e7c1e591a4b04b5598af9474c -n 0x0e7100e836150589b2be7030523d23c86a2093b5 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "three addresses" | tee -a tests/results.txt')
os.system('isContract --nocolors 0x1267895780369b48a02781eb08d353e426e5f256 0x1492cdd605d91658f228eab14d8444b60e6da8df 0x17f8e4bd5a79f095d7b680317de6a617d202b235 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "no options" | tee -a tests/results.txt')
os.system('isContract --nocolors 0x1b725d9bcbd26765fe4836d850631037dfb37e0a 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors 0x1dce4922337d44c8a187ce5bee12d26107340e4b 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors 0x207d0c591119bc4f01a25b6849ad375f6ea6af43 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors 0x237f29bbfd52c768a02980ea8d4d983a1d234edc 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "verbose" | tee -a tests/results.txt')
os.system('isContract --nocolors 0x26c4e381ffbdc825be9b986c3ba40525f0771e8d -v 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors 0x2935aa0a2d2fbb791622c29eb1c117b65b7a9085 -v 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors 0x2c19c7f9ae8b751e37aeb2d93a699722395ae18f -v 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "four addresses" | tee -a tests/results.txt')
os.system('isContract --nocolors 0x3003208e77edf3b088b122b5de3a6fc8c8ef679d 0x314159265dd8dbb310642f98f50c066173c1259b 0x3364ed250ea774146a0fbbc1da0ffa6a81514ca7 0x3682ae583f8c542ede42a9ca41105e5740b80d55 0x3cc0356217a8e03b94cf6a975683fc32db2080c4 0x40b9b889a21ff1534d018d71dc406122ebcf3f5a 0x437631e209736187b21090c0269e7a5f443811c3 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "diff on more than two addrs" | tee -a tests/results.txt')
os.system('isContract --nocolors 0x4671ebe586199456ca28ac050cc9473cbac829eb -n 0x48b4cb193b587c6f2dab1a9123a7bd5e7d490ced 0x4bab4e253813b7c831b67d57f2b6589b9542fc41 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "no options" | tee -a tests/results.txt')
os.system('isContract --nocolors 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors 0x513f22abc27e1f539dff98396acc78638dcd9c1a 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors 0x5524c55fb03cf21f549444ccbecb664d0acad706 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "-d option" | tee -a tests/results.txt')
os.system('isContract --nocolors 0x580114cbeb41c62f55ff55ca9e6f028ceb92ae91 -d 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors 0x655f01313bb55c04e8ed36a5098984f3f3de57bc -d 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "-dn option" | tee -a tests/results.txt')
os.system('isContract --nocolors 0x6d805b5de59d3f1779e4180b8547bcf728ff91ea -dn 2>&1 | tee -a tests/results.txt')
os.system('isContract --nocolors -dn 0x705f04a4f1e7a1e23caea390b8903bd43d96e18e 0x73cbe96839b723bc913d10ba32b58fc476988a36 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "multiple" | tee -a tests/results.txt')
os.system('isContract --nocolors 0x7753a9d834844cfde5c211ec3912b49f0d8b8e11 0x79d94aae97baf2017c1cd39f3b406d60712d086f 0x7d319a4b48aa12ad04604a4f0d3792b9574feaad 0x80d9c4f6209044de069c545f68644419b520a02f 2>&1 | tee -a tests/results.txt')

os.system('echo | tee -a tests/results.txt')
os.system('echo "from a --file" | tee -a tests/results.txt')
os.system('isContract --nocolors --file:tests/cmd_file 2>&1 | tee -a tests/results.txt')
