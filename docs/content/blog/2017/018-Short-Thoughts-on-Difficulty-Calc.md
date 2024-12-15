---
title: Short Thoughts on Difficulty Calc
lead: A short take
date: '2017-10-01T17:29:45.828Z'
draft: false
categories:
- Ethereum
tags: ['ethereum']
keywords: []
weight: 982
---

I found this text in [EIP 2](https://github.com/ethereum/EIPs/blob/c83a4aa8585d568d13e36e471e2f6b445896e6d6/EIPS/eip-2.md) of the Ethereum github repo. It’s from the justification section of the EIP. It explains some of the choices in the difficulty calculation code found here:

> The difficulty adjustment change conclusively solves a problem that the Ethereum protocol saw two months ago where an excessive number of miners were mining blocks that contain a timestamp equal to `parent_timestamp + 1`; this skewed the block time distribution, and so the current block time algorithm, which targets a _median_ of 13 seconds, continued to target the same median but the mean started increasing. If 51% of miners had started mining blocks in this way, the mean would have increased to infinity. The proposed new formula is roughly based on targeting the mean; one can prove that with the formula in use an average block time longer than 24 seconds is mathematically impossible in the long term.

> The use of `(block_timestamp - parent_timestamp) // 10` as the main input variable rather than the time difference directly serves to maintain the coarse-grained nature of the algorithm, preventing an excessive incentive to set the timestamp difference to exactly 1 in order to create a block that has slightly higher difficulty and that will thus be guaranteed to beat out any possible forks. The cap of -99 simply serves to ensure that the difficulty does not fall extremely far if two blocks happen to be very far apart in time due to a client security bug or other black-swan issue.

I put is here only so I can reference it from other articles in my Byzantium Thoughts series.
