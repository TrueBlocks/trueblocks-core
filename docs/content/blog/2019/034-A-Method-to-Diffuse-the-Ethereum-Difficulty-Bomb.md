---
title: A Method to Diffuse the Ethereum Difficulty Bomb
lead: Diffusing the bomb predictably with less angst
date: '2019-11-26T14:05:33.691Z'
draft: false
categories:
- Ethereum
tags: ['ethereum']
keywords: []
weight: 966
---

There is no more angst-ridden profession than being a member of a [bomb squad](https://en.wikipedia.org/wiki/Bomb_disposal). You’ll know what I’m talking about if you’ve ever seen the movie [The Hurt Locker](https://en.wikipedia.org/wiki/The_Hurt_Locker). In a recent Gitter post in the All Core Devs channel, Alexey Akhunov says of the difficulty bomb that it, “…forces people to make rushed decisions and be reckless, without real emergency…” This is true currently, but we can do better.

In a [previous article](https://medium.com/@tjayrush/its-not-that-difficult-33a428c3c2c3), I mentioned two questions that arise each time we discuss the Ethereum difficulty bomb: “How soon will the block times become intolerable?” and “For how many blocks should we reset the bomb?” I presented in that article a simple method to decide the second of these two questions. In this article, I extend that argument and run through an example. I then try to show why, if we adopted the method I’m proposing, we could answer the first question more predictably, removing some the ‘angst’ or ‘emergency’. Later this week, I will write an article analyzing the current situation and making a prediction of when the block times will become intolerable.

#### A Method to Decide How Far Back to Reset the Bomb

Currently the core devs pick the number of blocks to reset the bomb before picking the _fork\_block_. In the method I’m suggesting, we should pick _fork\_block_ first. Assuming we do that, this is how and why we should decide how far back to set the bomb:

_1._ Pick _fork\_block_

2\. Set the bomb back by _fork\_block — 50,000_ blocks

3\. Analysis at the time of the fork (i.e. _fork\_block_ \== _real\_block)_:

_fake\_block_ \= _real\_block_ — (_fork\_block_ — _50,000_)  
           = _fork\_block_ — (_fork\_block_ — _50,000_)  
           = _fork\_block_ — _fork\_block_ + _50,000_ \= _50,000  
    period_ \= floor(_fake\_block_ / _100,000_)  
           = floor(_50,000 / 100,000)_ \= 0

4\. Analysis of ongoing situation:

_fake\_block_ \= _real\_block_ — (_fork\_block_ — _50,000_)  
           = _real\_block_ — _fork\_block_ + _50,000_ \= _blocks\_since\_fork_ \+ _50,000  
    period_ \= floor((_blocks\_since\_fork_ \+ _50,000_) / _100,000_)

Convince yourself that the above calculations have nothing to do with hash rate.

5\. Until the bomb appears, block times hover at 14 seconds due to the adjustment calculation.

6\. The adjustment calculation:

a. works as designed by maintaining 14 second blocks

b. works in the face of any hash rate (this is its purpose)

c. never adjusts the block time _below_ 14 seconds

d. eventually cannot keep up with the bomb allowing the bomb to ‘rear its head.’

7\. Block times increase only after the bomb ‘rears its head.’

8\. The bomb ‘rears its head’ around period 40. (This is from experience and is somewhat dependent on hash rate, but it eventually gets overwhelmed). We can measure this empirically.)

9\. After resetting the period (i.e. _period_ = zero), there will be 40 periods of approximately 14s blocks before the bomb ‘rears its head.’

10\. After resetting the period, there will be 4,000,000 blocks (40 periods of 100,000 blocks) of approximately 14 second blocks before the bomb rears its head.

11\. Extending this:

 _= 14 second blocks \* 4,000,000 blocks  
     = 56,000,000.0 seconds  
     =    933,333.3 minutes  
     =     15,555.5 hours  
     =        648.1 days  
     =          1.7 years_

 _= approximately 21 months_

12\. The bomb first ‘rears its head’ _no earlier than_ 21 months after _fork\_block._

13\. The bomb first ‘rears its head’ _no earlier_ than _fork\_block + 4,000,000_ blocks.

14\. Due to higher hash rate, the bomb _may_ ‘rear its head’ later than this, but not before.

16\. We should schedule the next ‘bomb diffusion’ at _fork\_block + 4,000,000_ blocks.

16\. Notice there is ‘play’ or a ‘safety factor’ in this method. The block times will become intolerable not earlier than _fork\_block + 4,000,000._ Most likely, due to increasing hash rate, this will be later. This allows flexibility in choosing the next _fork\_block_ and removes the angst or emergency nature related to the difficulty bomb.

**Short Caveat**

This article does not deal with the current situation relative to the difficulty bomb. I will write about that issue later this week. This article only proposes a new method for selecting how far back to reset the bomb with the next hard fork and all diffusion-related hard forks in the future — including the pending hard fork to diffuse the bomb being proposed currently.

#### Summary

In this article, I tried to show a simple methodology to select how many blocks to offset when diffusing the Ethereum difficulty bomb.

The method I propose suggests that we first pick the _fork block_ number. Only then decide how many blocks to reset the bomb. In particular, given a _fork\_block_, select the offset to _fork\_block_ — _50,000_.

This will have the following effects: (a) it resets the bomb as far back as is possible while staying away from zero which may (but doesn’t) cause an edge case; (b) it makes the earliest block at which the bomb ‘rears its head’ more predictable (assuming period 40 is where the bomb first appears — this can be measured); and (c) it eliminates the angst from the decision-making process, making it more likely that the core developers will not remove the bomb entirely out of discomfort, as has been suggested.

(Not to go down another rabbit hole, but I believe the bomb serves a very important, larger societal purpose and it should be preserved vehemently.)

I’d love to hear discussion. Please comment below and clap to share the love.

#### About the Author

Thomas Jay Rush owns the software company TrueBlocks, LLC whose primary project is also called TrueBlocks, a collection of software libraries and applications enabling real-time, per-block smart contract monitoring and analytics to the Ethereum blockchain. Contact Rush through the website. Send donations to 0xf503017D7baF7FBC0fff7492b751025c6A78179b.
