from quickblocks import QuickBlocks
##add a node source in the foloowing line
qb = QuickBlocks('http://localhost:36963', cache=True)
print(qb.getTransaction(txhash='0x8b5df1ea9d2c7c3bde45cb4df13e51797caa89ab0909b408f6d5075c7f45747c'))

##for i in range(5200000):
  #  block = qb[i]
   # if i % 1000 == 0:
    #    print(block)
