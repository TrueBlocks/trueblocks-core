---
title: "Better Token Accounting"
description: "Information about how we account for tokens"
lead: "Information about how we account for tokens"
date: 2021-12-23T18:10:28-04:00
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
images: []
draft: false
contributor: ["rush"]
tags: ["trueblocks"]
weight: 943
---

Eth accounting is identical
Token accounting is improved in these ways:

1. Intra block token accounting
2. Account for every Transfer event
3. If a balance is incorrect, do one of three things:
   1. If the transaction contains a "large" input, and the event says something transferred, but the balances don't change, lable probably-spam and add a field called removal of phony transfer in
   2. If the function is one of a very small number of common mint functions, assign to knownMint field
   3. If the function is one of a very small number of common burn methods, assign to a knownBurn field
   4. If the balance goes up - label the transation as implied-mint and add to a feild called impliedMint
   5. If the balance goes down - label the reconciliation as impied-burn and add to a field called impliedBurn

Bugs fixed:
   In previous code, if a token transacted multple times in a single block the entire net transfer was assigned to the first transfer. In new code, each individual inter-block transfer is accounting for. Show a picture

   In the previous code, if an approval and a tranfer happened in the same block, the approval transaction was mistakenly identified as the transfer even though the accounting worked out, the wrong transaction was being identified.

   We added a bunch of new fields to the data structure and organized the data better (show image).

What we need to do yet

Example of Fake Fishing
   0xecd28a36f7811cf4bfa965f00ccbdf68dc6eccf70410525a5c2696ab30b05035 on account 0x153b95d138bb0c7a217dbc337e7de68f575ee8fb.txt
