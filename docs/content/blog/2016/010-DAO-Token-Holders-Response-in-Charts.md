---
title: DAO Token Holders’ Response (in Charts)
lead: Created with TrueBlocks
date: '2016-12-06T17:54:45.139Z'
draft: false
categories:
- Ethereum
tags: ['ethereum']
keywords: []
weight: 990
---

_I am posting this previously unpublished post now because I’m clearing out my drafts folder, and I didn’t want to simply delete the post. I wrote it soon after the DAO hack, but before it became apparent that the White Hats rescue the remaining ether in The DAO (or as some have called it “safely steal the ether”). The post is probably only interesting for historical reasons. References to days of the week in this post are to be read as meaning the week of Friday, June 17th._

I’m sure everyone is aware of Friday, June 17th when DarkDAO reared its ugly head. I woke up that morning, as I normally do, and sat down at the breakfast table with my morning newspaper (i.e. **slack**). The first message I saw said something like, “If you initiated CongoSplit…please contact us! Now!” The next message I read was, “The DAO is being drained — ongoing!”

I don’t know about you, but that many exclamation points first thing in the morning upsets me. I spent the rest of that day, trying to figure out what was going on. Soft fork, hard fork, counterattack, recursive call bug, splitDAO, transferWithoutReward…head explode. Exclamation point!

#### How Did I Respond?

I responded as I do to most other things in my life: I dove down into the data and tried to collect information. All day Friday, I spent combing through the blockchain data, trying to figure out what other people were doing. This is what I found.

#### How Did DAO Token Holders Repond?

As you might expect, the activity of the DAO token holders increased significantly on Friday. Each of the charts on this page shows the number of times one or another of the DAO functions was called.

The first chart shows the number of **_transfer_** and **_transferFrom_** function calls. It’s obvious that people reacted to the news by trying to sell their tokens.

Notice the large number of transfers on May 28. That was the first day after the creation period. This was most likely exchanges shifting tokens to individual account holders. You can see that the transfer activity droped significantly during most of the rest of June, but it picked up again on Friday.

Notice also that the **_transferFrom_** functions (pink) were higher than the **_transfer_** functions in May. This again points to exchanges. Since Friday \[the 17th of June\], the direct calls to **_transfer_** are more numerous than to **_transferFrom_**. **_Transfer_** must be called directly by the account holder, so these are people acting on their own behalf.

![](/blog/img/010-DAO-Token-Holders-Response-in-Charts-001.png)

#### Exiting the DAO

A DAO token holder may exit the DAO in three ways: (1) by selling (and thereby transferring) their tokens on an open market; (2) by creating a split proposal, moving their ether into a baby DAO, and then making a proposal to retrieve their ether; and (3) by voting on a split proposal and exiting into someone else’s baby DAO.

Calls to the **_newProposal_** function (method 2) are shown in the next chart. (Please note that the data I use does not distinguish between regular investment proposals and split proposals.) As you can clearly see, the number of proposals has increased significantly since the attack.

![](/blog/img/010-DAO-Token-Holders-Response-in-Charts-002.png)

The first question that came to my mind when I looked at this chart was _How many of these new proposals point to a smart contract with the same recursive-call bug? How many of these proposals will drain the DAO further?_

The second thing I thought about was the twenty-seven days (plus a seven debating days) that people will have to wait to get their ether. _What happens in the case of a soft fork?_ My next worrisome thought: _What happens in the case of a soft fork if they can then not agree on a hard fork?_ The people making proposals now are running away from the DAO. The soft fork will freeze activity in not only The DAO, but any child DAO. Without a follow-on hard fork, these baby DAOs will be frozen forever.

#### Rats Leaving the Ship

Selling on the market (**_transfer_**, **_transferFrom_**) and leaving via the split mechanism (**_newProposal_**) are two methods to leave the DAO. A third method is to vote on someone else’s split proposal and ride with them into their new baby DAO.

It appears that there are at least some people taking this path. This is revealed in the third and fourth charts below. The spike in voting on Friday and then again on Sunday, and secondly, the large increase in **_splitDAO_** function calls reveal people trying to jump ship on someone else’s split.

![](/blog/img/010-DAO-Token-Holders-Response-in-Charts-003.png)

Two different things are going on in this first chart. During the first few operational weeks of the DAO, there were a relatively high number of people voting — possibly out of a sense of excitement. This type of voting is different, I think, from the voting we see since Friday. The voting on Friday and Sunday were, I think, people trying to leave The DAO. The low number of votes on Saturday, I think, is because of the messaging from the White Hats instructing people to refrain from interacting with the software while they figured out what to tell us. There was an explicit post reddit.

Most of the voting on Friday, I think, was by people scurrying to get off a sinking ship. The sad thing about rats leaving a ship is that they end up in the water where there is a not-insignificant chance of drowning anyway. This view is reinforced by the following chart.

![](/blog/img/010-DAO-Token-Holders-Response-in-Charts-004.png)

The increase in the number of calls to splitDAO as people started figuring out this third method of exiting The DAO is obvious. In a baby DAO, if you are not the initiator of the original split proposal, you will not be able to make new proposals in the child DAO. This is because you will be on the whitelist. This leaves you at a disavantage. Perhaps.

You will be at a disadvantage because you will not be able to get your money out of the child DAO unless the originator is magnanomous or enlightened. If the originator owns more tokens than all of the other people in that child DAO, they can simply take the ether. More likely though, they will own less tokens than the rest of the members. In this case, the only solution seems to be a truce. If the originator does not whitelist anyone, and the majority does not allow the originator to take the ether, perhaps the only solution is a proposal that shares the ether. Or perhaps the originator of the baby DAO agrees to release the ether for some reward. The trouble, of course, as is true with this entire smart contract universe, is that communicating with other members of the contract is nearly impossible.

The point I’m making is that trying to split off with someone else’s baby DAO is as bad as being a rat and jumping off a sinking ship. Probably not a great idea.

#### No Conclusions

I would never make a recommendation as to what anyone else should do at this point. I know what I did. I know what I’m advising my son to do. We’re selling our tokens — even at a loss. You will have to figure it out for yourself.

Here’s what other people are doing however. Over 23,000 different accounts purchased tokens during the creation period. Look at the **_y_** axis of the above charts. The great majority of people are doing nothing. My guess is that this is because they don’t know what to do. I hope this post can at least give you some ideas or some thoughts. Good luck. Keep swimming!
