---
title: Downloading the DAO
lead: Everything, all the time
date: '2016-06-10T18:43:27.918Z'
draft: false
categories:
- Ethereum
tags: ['ethereum']
keywords: []
weight: 996
---

I am obsessed with the DAO. I spend a ridiculously large amount of time thinking about it, reading about it, and trying to figure it out. The most interesting thing to me is the data. The fact that the smallest transactions —both into and out of the DAO — are recorded forever is very interesting to me.

Ever since I got into Ethereum, I’ve been trying to figure out how to get at all that data. I’ve been hacking for weeks. I’ve diven (doven? dove?) into the depths of the core code. I’ve studied ‘geth,’ ‘eth,’ ‘mist,’ and ‘web3.’ I’ve learned how to use RESTful APIs and JSON. I had no idea what I was doing, but I knew what I wanted — I wanted that data.

I finally have it. In the end, I cobbled together Ethslurp (a block chain scraper) which is now available and open sourced on Git Hub. (Watch the video explaining how it works: [http://ethslurp.com/videos.html](http://ethslurp.com/videos.html).)

In Ethslurp’s most basic use, one simply tells it to “slurp” an Ethereum address. The program returns JSON data:

> $> ethslurp 0xbb9bc244d798123fde783fcc1c72d3bb8c189413

When I run this command from a Linux or Mac command line, Ethslurp downloads, imports, and then exports every transaction to or from that address. (You may notice this address as “The DAO.” There are more than 100,000 transactions to the DAO, so it takes a long time to download — If you try it, don’t say I didn’t warn you.)

If, instead, I run this command:

> $> ethslurp -l -f:csv -a:transactions.csv

and then open the file transactions.csv, MS Excel pops up, and I can see all 100,000 transactions in a spreadsheet. (**\-l** means slurp last address, **\-f** means format, **:csv** specifies the format, and **\-a** means archive to the given file)

It took me weeks, but I finally had the data. I’m so pleased, that I’ve decided to share it with the world: [http://daodeepdive.com/cgi-bin/site.cgi?cmd=Data](http://daodeepdive.com/cgi-bin/site.cgi?cmd=Data).

#### Now What?

Given this data, the first thing I wanted to do was to separate out the ‘Token Creation’ transactions from the transactions that have occured since the creation period.

The Mist browser tells me that the token creation period ended at timeStamp = 146442600.

![](/blog/img/004-Downloading-the-DAO-001.png)

Every transaction on the Ethereum blockchain has a timeStamp. So I simply went into the spreadsheet and found the last transaction that happened before “closing time” and deleted the rest. I’ve since added a feature to EthSlurp to specify the earliest and/or latest timeStamp, block number, or date. I can now run this command:

> $> ./ethslurp -f:csv -timeStamps::1464426000 -l -a:creation.csv

Which says ‘using the last (**\-l**) address slurped and storing the data in comma separated values format (**\-f:cvs**), read from the earliest timeStamp to latest **1464426000** inclusive and store the results in **creation.csv**. In other words — slurp The DAO’s creation period.

Next, I wanted to verify that the total ether that was being reported by EthSlurp was accurate. I used three different sources to check my data: (1) the daohub.org website, (2) etherscan.io (which, if you look at the internals of Ethslurp is what the program is slurping), and (3) the Mist browser, which I consider the authoritative source of information on the DAO.

What I did was:

1. I opened up the file creation.csv in Excel and summed the column labeled ‘value.’ My expectation was that this total would agree with the Mist browser, <http://etherscan.io>, and the <http://daohub.org> website.
2. Before I did that, because the ‘value’ column is in ‘wei,’ I converted it to ether. I did this by adding a new column and dividing ‘value’ by 10¹⁸ (see [What the F is a Finney](https://medium.com/@tjayrush/what-the-f-is-a-finney-8e727f29e77f#.fcas67m6m)). I then summed the ‘ether’ column as well.
3. According to my downloaded list of transactions the total ether taken in during the creation was 11,980287.5826, which was higher than the number being reported by the Mist browser by 2.17%. Interestingly, though, this value was closer than the value reported by the DAOHub.org website. (I leave figuring that out as an exercise — \[Jun 11: this has since been explained, see comments below\]). The Etherscan.io website agrees pretty well with the Mist browser.

![](/blog/img/004-Downloading-the-DAO-002.png)

#### Where to next?

I’m currently interested in the data field called ‘input’ which contains what I believe to be the actual function calls and parameters for each interaction with the DAO.

I am also interested two fields called ‘gasUsed’ and ‘gas,’ which, in some cases, are equal to each other. My first thought about these fields is that being equal may indicate an “out of gas” condition, but it turns out this is not quite correct.

I’ve hand-checked a number of these transactions, and I’m finding three different types of ‘gas=gasUsed’ transactions: (1) transactions that do not have enough gas to even get started and, therefore, they fail on their first opcode ([example](https://etherscan.io/vmtrace?txhash=0x0c86ed91f75f5b44d7e8e97facbebba9cf1a14982d9006da480049c34d7cc12f)), (2) transactions that have exactly the right amout of gas, and finish their execution with zero gas remaining ([example](https://etherscan.io/vmtrace?txhash=0x685a466ac4ffa7a12172652726341fe7c18f124b8f94cef3ff30134ed81115b4)), and (3) a very odd type of transaction that fails when it encounters an opcode that charges about ten god-gillion gas. 76 digits worth of gas ([example](https://etherscan.io/vmtrace?txhash=0x8f3717c30250ccb23f5261319671c2a1b33ee3e9bf7c46a69c372c0217e177fa)). That is way more atoms than there are in the ether, let alone ether in anyone’s wallet. I’m not sure what’s going on there. \[Jun 11: Thanks to [Yoichi Hirai](https://thedao.slack.com/messages/@yhirai) on thedao slack, I’ve since learned that this large number is just a negative amount of gas remaining. Thanks Yoichi.\]

#### Who Cares?

If you’re like me you care simply because its cool as hell, and all information is good information. If you’re not like me, you got bored a long time ago, and you are no longer reading, so you can go pound sand.

Hope you found this interesting. I’ll be posting a lot more about what I’m finding in the transaction logs soon. See you next time.
