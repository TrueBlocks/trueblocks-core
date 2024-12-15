---
title: Simple Undeniable Facts
lead: The truth will not be denied
date: '2020-01-10T04:23:30.173Z'
draft: false
categories:
- Trueblocks
tags: ['trueblocks']
keywords: []
weight: 965
---

One of the reasons I fell in love with blockchain tech way back in 2013 was because I felt that for the first time in history, engineers had created a system that could produced undeniable data. Undeniable access — yes, but more importantly, undeniably high-quality data. That was a paradigm shift.

While, technically, the data on a blockchain may be untrue, what is mathematically undeniable is that at least 51% of the participants have agreed that the data is consistent — maybe that’s as close as we can get to data being ‘true’.

Bitcoin (and other blockchains) produces the best data the world has ever seen, and I think that this near-perfect data can usher in a better world. At least, that’s what I used to think…

![](/blog/img/035-Simple-Undeniable-Facts-001.jpg)

In this post, I will present a list of what I hope are obviously factually true statements. I want to explain a looming danger I see coming down the pike. To me, this looming danger looks a lot like an out-of-control, 18-wheeled tractor-trailer coming right for us.

Each item in the lists below says something simple that is obviously true. After a small group of statements, I try to draw equally true conclusions. I’m not sure this will convince anyone of my views, but I’d love to hear what you’all think.

So off we go…

#### The Blockchain Data is Too Big

1. Unless data is small or it’s already sorted the way you want it to be sorted, it‘s impossible to query the data quickly without an index.
2. Indexing a database makes the database larger.
3. Commercial-grade computers and laptops — the kind we all own — have limited hard drive space.
4. Extracting and indexing all the data on a blockchain, especially the Ethereum blockchain because of traces, creates a database much larger than any commercial-grade hardware can accommodate.
5. While the Ethereum node does index some of its data (such as topics), it does not index `appearances`.
6. (In our parlance, `appearances` refers to every location of an address anywhere on the chain — this includes the obvious places such as a transaction’s `to` or `from` address, the addresses created when a smart contract is deployed, the address destroyed and the receiving address when a smart contract self-destructs, addresses initiating events, addresses receiving tokens, addresses being named on the ENS, holders, borrowers, and users of DAI, etc., etc., etc. Everything.
7. Importantly, many `appearances` happen deep inside of smart contracts when the contract uses an address as data. Many times, the smart contract remains silent about this use (it doesn’t generate events). Sometimes there are financial impacts on an address that aren’t made obvious.
8. Many `appearances` are buried ‘deep inside the traces’ and are not located by naive, shallow methods of extraction. (We’ve explained this issue in an article [here](https://medium.com/coinmonks/how-many-tokens-do-you-have-eae7233676f1).)
9. In order to produce a 100% accurate accounting for or an audit of a given address, one must — unfortunately — dig into every `appearance`. This means you need to descend into every trace. This means you either must search an index or you must scan the chain from start to finish.
10. **Conjecture:** Without an index of appearances, it is impossible to do an accurate, full, and complete accounting or auditing of a given address on commercial-grade hardware.
11. **Conjecture:** If you can’t do a full accounting on your own machine, you must either forgo deep, complete insight into your addresses or you must rely on a third party to give you the data you need.
12. **Conjecture:** If, in order for your system to account for itself, you must rely on third parties, your system is not decentralized.
13. **Conclusion:** Ethereum is not now and will never be decentralized unless something changes.

#### Everyone Wants Privacy — Some People Want Transparency

1. Most people care deeply about their own financial data but very little about other people’s financial data.
2. No-one — not one single person on this planet — wants their financial information exposed without their permission.
3. Some people (obversely) purposefully expose parts of their financial activity in a wish to be transparent. Generally, these people are community oriented and not looking to invade other people’s privacy.
4. Some people peek into other people’s medicine cabinets.
5. There’s an astronomical amount of money to be made from understanding, analyzing, predicting, and exploiting other people’s private (and public) financial activity; that is, looking in their medicine cabinet.
6. A small percentage of people are unethical — and the pressure to be unethical becomes more apparent when there are an astronomical number of dollars to be made.
7. **Conclusion:** As blockchains become more prevalent, and people realize they can’t get the data themselves, they will rely on businesses to provide that data. As time goes on, the businesses will see opportunity in unscrupulously invading people’s privacy.

#### (S)he Who Owns the Hard Drive, Owns the Data

1. Every piece of useful data is eventually stored on a hard drive.
2. If you use data that is on your own local hard drive — barring viruses — no-one can see what you’re doing.
3. If you’re using data on someone else’s hard drive, it’s impossible to stop them from peeking at what you’re doing.
4. Individuals do not like to pay for services — especially digital services.
5. Digital service providers cannot provide free services for free. At some point, they must monetize the service they provide.
6. Most people are easily convinced to accept free services without asking deeply about the real cost of the service. _Hint:_ the real cost is your privacy.
7. **Conclusion:** Over time data service providers appear to deliver free access to people’s personal financial data. These companies, delivering data _from their own hard drives,_ will eventually conclude that snooping on their users can be very lucrative. They won’t be able to resist.

#### Blockchain Data is the Best Data Ever the World has Ever Seen

1. Blockchains organize data in a near-perfect, immutable way, and while the data is genuinely difficult, making it impossible for normal people to use it, businesses will be able to extract deep insight into anyone’s personal financial history.
2. The quality of the data, and therefore its value, skyrocketed when blockchains became prevalent. The quality of that data will continue to increase exponentially as adoption increases, tools improve, and network effects take over.
3. **Conjecture:** Unscrupulous businesses with lots of resources, lots of technical skill, and _very large hard drives_ will be invading everyone’s privacy in new and novel ways in twenty years.
4. **Conjecture:** Almost every team in the ecosystem is working on systems that rely in some way on third parties.
5. **Conclusion:** We should stop building something we don’t want.

#### Is There Any Hope…Only If We Work Together

1. Because people care about their own financial information, they are incentivized them to hold that data closely (if they’re smart).
2. Blockchains require some data sharing between participants in order to work. They wouldn’t work if users only stored their own data. (If they did work that way, how would other computers come to consensus on the data? (Please don’t say ZK-Snarks or stateless clients— these are nice ideas, but I’ve not seen any of them anywhere near production.)
3. While sharing must happen, not every participant needs to carry every piece of the data on their hard drives.
4. It _is_ necessary for every participant to have undeniable access to every piece of data. (Otherwise, people couldn’t trust the system. If one knows one has undeniable access to the data, one doesn’t need to store it locally.)
5. Blockchains are immutable.
6. Once blockchain data ages past a certain number of minutes (say six for Ethereum, sixty for Bitcoin), the data never changes for the rest of human history.
7. There’s something about immutable data that loves a cache.
8. There’s something about immutable data that loves a hash.
9. Content-addressable file systems, such as IPFS, store data on the user’s local hard drive.
10. The location of that data is identical to the file’s file name (that is, the location _is_ the hash — now and forever).
11. Once content is written to IPFS, the hash for that data never changes. More accurately, if the contents change, the file’s location changes.
12. Hashes are very short and very easy to share. With an internet connect, the IPFS software, and an IPFS hash, the user has undeniable access to the contents stored at the hash.
13. Blockchains continually produce new data, and therefore continually produce new `appearances` that must be added to the index. On Ethereum, new data comes every 14 seconds.
14. Constantly changing data and content-addressable file systems do not work and play well together.
15. One way to alleviate this problem is to periodically make a snapshot of the index and move on to another file. This creates chunks of data in equal-sized files. Snapshots are immutable. This is what TrueBlocks does with its `appearance` index.
16. If one is continually producing new data, one needs to publish the location (in our case this means the hashes) of that data to one’s users.
17. Blockchains, in a certain sense, “publish” block hashes.
18. **Conjecture:** If we wished to, we could change the blockchain software to produce content-addressable hashes instead of “regular” block hashes.
19. **Conjecture:** The physical storage of the blockchain data is optimized for the blockchain’s use — not for content-addressable storage.
20. **Conjecture:** Having the blockchain store its data in a content-addressable store would ruin its performance.
21. **Conjecture:** It would be possible for blockchains to produce a content-addressable hash without actually storing its data in a content-addressable store. This would maintain the performance of the chain and keep it working.
22. **Conjecture:** The POW and difficulty calculations would not need to change — the previous block’s IPFS hash could be inserted into the current block, thereby maintaining the chain. A random nonce could be inserted as well, maintaining the POW — everything would work exactly as it works now.
23. **Conjecture:** The fact that the block’s hash _is_ an `IPFS` has would be informational only — a by-product so to say.
24. **Conjecture:** The `meaning` of the block hash would go from being utterly meaningless to very, very meaningful.
25. **Conclusion (most important):** Block hashes should _be_ IPFS hashes.

#### Blockchain’s Killer App is Community

1. The community — eventually and unrelated to how the software actually stores its data — would eventually understand that the block data is available _at the location of the hash_. As a result of this understanding, users would come to “expect” in that location. As a result of _that_, the data would simply start to appear in that location. If everyone knew where the data was, everyone would go there to look for it. As more and more people find the data in that location (put it there if they don’t find it), the likelihood of finding it in the future would grow. There would be no other logical place to look for the data other than its block hash.
2. Because people value their own data, they would would have no problem pinning the data. Software written to use the data could pin it by default. This would have no detrimental effect on the user machine as he/she would need to store local application-specific data anyway. There would be no downside — only upside.
3. **Conjecture:** Over time, regular users could run extremely light nodes — the only thing on those chains would store would be the hashes. At block 10,000,000, which is where the chain is now, this would take up 32 bytes \* 10,000,000 blocks — or about 2.5 GB. Anyone can cary that much data.
4. If we did this, every piece of on-chain data would be available — undeniably — to anyone in the world on their local machines.
5. Each user could hold the full block detail of only those blocks that they’re interested in (the blocks containing their transactions). The software would pin these blocks, in effect, making them available to others.
6. The system could be designed redundantly. When a user asks for a single block, the system could give her (and pin) more than one block. In this way, participants would hold (and pin) their own data plus an additional portion on behalf of the community.
7. **Conclusion:** It seems likely that some number of ‘super-nodes’ would have to run on behalf of the entire community. This would be needed to ensure that every single block is available somewhere. The Ethereum Foundation, Consensys, Infura, EtherScan and other small and large players, could run these super-nodes. It would be their donation to the community. (Note that traffic to these providers would be small since each individual user would be caching their own data).

#### But…How Would People Know Which Parts of the Data They Need?

1. Above, I mentioned an index of `appearances`. This index is needed so users know which blocks to look for.
2. With TrueBlocks, we’ve already built the software that builds, hashes, pins, and queries the appearance index.
3. After nearly 10,000,000 blocks, TrueBlocks has produced more than 3,500 snapshots of the appearance index. This index is immutable, compact, stored as a binary file both locally and in IPFS, and takes up about 35GB _on our hard drive_. TrueBlocks search it with 100% privacy.
4. To lessen the size of the index, we went a step further and produced Bloom filters of each snapshot. Just like the snapshots, the Bloom filters are immutable, very compact, and pinned on IPFS. The Bloom filters allow TrueBlocks to identify which chunks of index are needed. The Bloom filters are only 1.5 GB (also in 3,500 files), making them even more accessible to regular users.
5. We go even further than that. For both the Bloom filters and the index chunks, we produce text files listing the hashes which we distribute as part of the software. The first time one runs TrueBlocks, it downloads 1.5 GB of Bloom filters. It then starts a daemon to catches up to (and keep up to) the front of the chain.
6. Using the Bloom filters, the software queries (quickly) for a list of snapshots. The list of snapshots are then downloaded. For almost every address, this list is very, very small.
7. For popular addresses (such as CryptoKitties, ENS, or DAI) the list of snapshots is very large. Nearly every chunk is of interest.
8. **Conclusion:** This naturally requires heavy users of the chain to download and pin much more data than regular users — this is a very desirable quality for a distributed system. It’s naturally fair.
9. Once one has the list of hashes, one has undeniable, fully-local, 100% private access to the Bloom filters.
10. This gives one undeniable access to the minimal list of index chunks. This give undeniable and fully-local access to every transaction for an address.
11. The list of Bloom filter hashes is [here](https://github.com/TrueBlocks/trueblocks-core/blob/master/src/other/install/ipfs-hashes/blooms.txt).
12. The list of IPFS hashes of the chunked appearance index is [here](https://github.com/TrueBlocks/trueblocks-core/blob/master/src/other/install/ipfs-hashes/finalized.txt).
13. **Conclusion:** The above two links and [the TrueBlocks software](https://github.com/TrueBlocks/trueblocks-core) gives everyone access to a full index of appearances of every addresses anywhere on the chain. This access comes _on your own local machine. From_ there, you can operate 100% privately and fully decentrally.

#### Conclusion

We intend to write further about how TrueBlocks accomplishes the creation of the appearance index and how it post-processes those appearances into snapshots and how it stores the data on the hard drive. Visit us again in a few weeks.

We’re please to announce we now have a GitCoin grant page here: [https://gitcoin.co/grants/184/trueblocks](https://gitcoin.co/grants/184/trueblocks). Donate today, during the CLR matching period, and we will get the added benefit of a larger matching grant. Even small amounts have a big impact.

If you’d rather not expose yourself and you’d still like to donate in support of our work, send ETH to 0xf503017d7baf7fbc0fff7492b751025c6a78179b. Thanks for reading.
