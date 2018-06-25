from quickblocks import QuickBlocks

qb = QuickBlocks('http://localhost:8545', cache=True)
for i in range(5200000):
    block = qb[i]
    if i % 1000 == 0:
        print(block)
