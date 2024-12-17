---
title: Mother May I?
lead: A new way to think about data APIs
date: '2019-06-24T19:40:19.363Z'
draft: false
categories:
- Trueblocks
tags: ['trueblocks']
keywords: []
weight: 970
---

A New Way to Think About Data APIs

When I was a kid, we used to play a game called “Mother May I”. In the game, the person who was _it_, called “Mother”, would stand on one end of the room and the players would stand on the other. The players would shout out things such as, “Mother, may I take one giant step forward?” or “Mother, may I take two scissors steps?” Mother would say either “Yes” or “No.” Players pleaded with Mother in random order. Mother had to say “Yes” more often than “No.” The first player to reach Mother became Mother, and a new game would start. Not exactly the most exciting game in the world, but we were kids so give us a break.

In this article, we will play an imaginary game of Mother May I between a player (you, the developer) and `chifra` (the tentative name for our TrueBlocks’ API). Hopefully, this will help you understand our thinking related to APIs and what we think may have to happen as we all learn how to interact with shared global _immutable_ data.

One thing we think is clear: old-fashioned web 2.0 APIs such as Alethio and AmberData should not be the way we build out this new infrastructure. If we do, we will not end up with what we want. We will end up exactly where we already are: captured by huge, mega-data-gobbling, mega-corporations who watch everything we do and use that information to manipulate us.

![](/blog/img/030-Mother-May-I-001.png)

After this first interaction, the user does not have the actual data heshe wants, but heshe has the location of that data.

![](/blog/img/030-Mother-May-I-002.png)

The API does not deliver the actual data. The API delivers the location of the data on an immutable, content-addressable data store. The end user retrieves the data they want from that location thereby distributing it across the world. The user can respond to this in two ways:

![](/blog/img/030-Mother-May-I-003.png)

Even though we’ve educated the user that they should stop using data APIs arbitrarily since they will capture them, the user insists heshe wants to use one. Who are we to argue after that?

![](/blog/img/030-Mother-May-I-004.png)

Yes. We’re starting to think people have to be this radical when it comes to data APIs. The costs we pay are, we think, too high for the benefit. The last thing we want to do — now that we have a new paradigm brewing—is end up where we already are. Concern yourself with capturability, censorability, data validity, and availability.
