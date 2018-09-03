# Python 2.7

import json
from datetime import datetime
from datetime import timedelta
import matplotlib.pyplot as plt
import re
import progressbar

def timestamp2human(timestamp):
    human = datetime(1970, 1, 1, 00, 00) + timedelta(seconds=timestamp)
    return human

# serialize datetime formats
def myconverter(o):
    if isinstance(o, datetime):
        return o.__str__()

def daterange(date1, date2):
    for n in range(int ((date2 - date1).days)+1):
        yield date1 + timedelta(n)

        
fileName = 'newbium.json'
totalSupply = 49000000
ctrAddress = '0x814964b1bceaf24e26296d031eadf134a2ca4105'
ctrTxHash = '0x03047f8e573880f822964804846bf866f7a2c33c80328da285f9d2a984edab1d'
Balances = []
TimeStamps = []

print "Abriendo JSON..."
with open(fileName) as f:
    txs = json.load(f)
print "Buscando accionistas, eventos y transferencias..."

holders = []
cnames = []

oldestEvent = datetime(2070, 1, 1, 00, 00)
newestEvent = datetime(1970, 1, 1, 00, 00)

cnameList = ['WithdrawToken', 'DepositToken', 'TransferFrom', 'Transfer']

for tx in txs:
    if tx['cname'] == 'Transaction' and tx['hash'] == ctrTxHash:
        creatorBlock = tx['blockNumber']
        contract = tx['receipt']['contractAddress']
        creatorHash = tx['from']
        creatorTimeStamp = tx['timestamp']
        oldestEvent = timestamp2human(tx['timestamp'])
        break

bar = progressbar.ProgressBar(maxval=len(txs), \
    widgets=[progressbar.Bar('=', '[', ']'), ' ', progressbar.Percentage()])
bar.start()

for i,tx in enumerate(txs):
    if tx['cname'] in cnameList:
        for log in tx['receipt']['events']:
            if log['address'] == ctrAddress:
                # check if holder is already in holders array
                holderFrom = re.sub('(0x)(000000000000000000000000)(.*)', r'\1\3', log['topics'][1])
                holderTo = re.sub('(0x)(000000000000000000000000)(.*)', r'\1\3', log['topics'][2])
                if holderFrom not in holders:
                    holders.append(holderFrom)
                    Balances.append([])
                    TimeStamps.append([])
                    TimeStamps[-1].append(timestamp2human(creatorTimeStamp))
                    if holderFrom == re.sub('(0x)(000000000000000000000000)(.*)', r'\1\3', creatorHash):
                        Balances[-1].append(totalSupply)
                    else:
                        Balances[-1].append(0)
                if holderTo not in holders:
                    holders.append(holderTo)
                    Balances.append([])
                    TimeStamps.append([])
                    TimeStamps[-1].append(timestamp2human(creatorTimeStamp))
                    if holderTo == re.sub('(0x)(000000000000000000000000)(.*)', r'\1\3', creatorHash):
                        Balances[-1].append(totalSupply)
                    else:
                        Balances[-1].append(0)
                
                if holderFrom in holders:
                    index = holders.index(holderFrom)
                    Balances[index].append(Balances[index][-1])
                    Balances[index].append(Balances[index][-1]-int(log['data'],0))
                    TimeStamps[index].append(timestamp2human(tx['timestamp']))
                    TimeStamps[index].append(timestamp2human(tx['timestamp']))
                if holderTo in holders:
                    index = holders.index(holderTo)
                    Balances[index].append(Balances[index][-1])
                    Balances[index].append(Balances[index][-1]+int(log['data'],0))
                    TimeStamps[index].append(timestamp2human(tx['timestamp']))
                    TimeStamps[index].append(timestamp2human(tx['timestamp']))

                if timestamp2human(tx['timestamp']) > newestEvent:
                    newestEvent = timestamp2human(tx['timestamp'])

    elif tx['cname'] == 'Transaction':
        for log in tx['receipt']['events']:
            if log['address'] == ctrAddress:
                # check if holder is already in holders array
                try:
                    holderFrom = re.sub('(0x)(000000000000000000000000)(.*)', r'\1\3', log['topics'][1])
                    holderTo = re.sub('(0x)(000000000000000000000000)(.*)', r'\1\3', log['topics'][2])
                    continuing = True
                except:
                    continuing = False
                if continuing == True:
                    if holderFrom not in holders:
                        holders.append(holderFrom)
                        Balances.append([])
                        TimeStamps.append([])
                        TimeStamps[-1].append(timestamp2human(creatorTimeStamp))
                        if holderFrom == re.sub('(0x)(000000000000000000000000)(.*)', r'\1\3', creatorHash):
                            Balances[-1].append(1)
                        else:
                            Balances[-1].append(0)
                    if holderTo not in holders:
                        holders.append(holderTo)
                        Balances.append([])
                        TimeStamps.append([])
                        TimeStamps[-1].append(timestamp2human(creatorTimeStamp))
                        if holderTo == re.sub('(0x)(000000000000000000000000)(.*)', r'\1\3', creatorHash):
                            Balances[-1].append(1)
                        else:
                            Balances[-1].append(0)
                    
                    if holderFrom in holders:
                        index = holders.index(holderFrom)
                        Balances[index].append(Balances[index][-1])
                        Balances[index].append(Balances[index][-1]-int(log['data'],0))
                        TimeStamps[index].append(timestamp2human(tx['timestamp']))
                        TimeStamps[index].append(timestamp2human(tx['timestamp']))
                    if holderTo in holders:
                        index = holders.index(holderTo)
                        Balances[index].append(Balances[index][-1])
                        Balances[index].append(Balances[index][-1]+int(log['data'],0))
                        TimeStamps[index].append(timestamp2human(tx['timestamp']))
                        TimeStamps[index].append(timestamp2human(tx['timestamp']))
    elif tx['cname'] not in cnames:
        cnames.append(tx['cname'])

    bar.update(i+1)

bar.finish()

timeLine = []
fundingPath = []
fundingPathPrint = []

oldestEvent = oldestEvent.replace(hour=0, minute=0, second=0, microsecond=0)
newestEvent = newestEvent.replace(hour=0, minute=0, second=0, microsecond=0)

for dt in daterange(oldestEvent, newestEvent):
    timeLine.append(dt)
    fundingPath.append(0)

print "\n"
print cnames
print "Preparando gráfico..."
total = 0
colours=['r','g','b','k']
m=0

percentages = []
address = []

for i, blc in enumerate(Balances):
    if holders[i] != "0x0000000000000000000000000000000000000000":
        total += blc[-1]
        # don't show small fish
        if blc[-1] > 0.01*totalSupply:
            percentages.append(blc[-1])
            address.append(holders[i])
        printing = False
        for j, item in enumerate(blc): 
            index = timeLine.index(TimeStamps[i][j].replace(hour=0, minute=0, second=0, microsecond=0))
            if int(item) < 0.15*totalSupply:
                if j > 0:
                    appendItem = blc[j-1]
                else:
                    appendItem = item
                fundingPath[index] += int(appendItem)
            if item > 0.01*totalSupply:
                printing = True
            if item < 0:
                print holders[i], item
                print len(blc)
        # this would add another entry to have a line from the last tx to today
        #Balances[i].append(blc[-1])
        #TimeStamps[i].append(datetime.now())
        if blc[-1] > 0.05*totalSupply:
            print holders[i], blc[-1]
        if blc[-1] < 0:
            print holders[i], blc[-1]
        # this would plot the balance over time
        if printing == True:
            # ax = plt.plot(TimeStamps[i], Balances[i], colours[m])
            m+=1
        if m>3:
            m = 0

# funding path graph
for day in fundingPath:
    if fundingPathPrint != []:
        fundingPathPrint.append(fundingPathPrint[-1]+float(day)/totalSupply)
    else:
        fundingPathPrint.append(float(day)/totalSupply)

"""
jsonoutH = json.dumps(holders, default = myconverter)
jsonoutB = json.dumps(Balances, default = myconverter)
jsonoutT = json.dumps(TimeStamps, default = myconverter)


with open('newbiumoutH.json', 'w') as outfile:
    json.dump(jsonoutH, outfile)
with open('newbiumoutB.json', 'w') as outfile:
    json.dump(jsonoutB, outfile)
with open('newbiumoutT.json', 'w') as outfile:
    json.dump(jsonoutT, outfile)
"""

for i, pct in enumerate(percentages):
    print pct/totalSupply, address[i]
print "\n"
print "Suma de balances: " + str(total) 
print "Total supply:          " + str(totalSupply)

axe = plt.plot(timeLine, fundingPathPrint)
plt.show()

