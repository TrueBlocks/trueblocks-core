---
title: "Introduction"
description: "Some Thoughts on the Broken Ethereum Node"
lead: "Some Thoughts on the Broken Ethereum Node"
date: 2020-10-06T08:48:57+00:00
lastmod: 2020-10-06T08:48:57+00:00
draft: true
images: []
weight: 0100
toc: true
---


1. The Ethereum node is broken. It is broken because it does not have an index of addresses. This makes it impossible to get the transactional history of an address directly from the node.

2. Indexes and content-addressable data stores such as IPFS do not play well together. This leads to directly to centralized (and old-fashioned) Web 2.0 data access through blockchain explorers which have already become the only way most users can get transactional histories of their own accounts. That's not decentralized.

3. If a single entity (or a small number of entities) becomes the only way for users to get their transactional histories, that entity will capture those users. This will happen either by the entity withholding the data and charging for access or (more likely) by giving the data away for free and return for much more lucrative payment through privacy invasion.

4. To solve this problem, the Ethereum node needs to index the blockchain. TrueBlocks builds this missing index. However, we don't want to build it and be able to later control it or limit access to it. That's where IPFS comes in. We build a chunked index (that is, periodically we stop adding data to the growing chunk). This allows us to store the chunk immutably on IPFS.

5. Before storing the data, we add a Bloom filter that covers the index chunk. We publish this immutable Bloom filter to IPFS as well. The Bloom filter is very small and speeds up the search of the chunked index by orders of magnitude.

6. Because end-user machines have small hard drives and we wish to permissionlessly and irrevocably distribute the index, TrueBlocks only downloads the much smaller Bloom filters to the end users machine on first install. Subsequent queries for an address first check the Bloom filter and only download the associated full index chunk of there's a hit on the Bloom filter. In this way, each user downloads only the portion of the index that is of interest to them.

7. Those who say it is impossible to index the Ethereum blockchain on small, desktop machines are wrong.

8. Indexing the Ethereum blockchain by address turns the broken Ethereum node into a very performant server able to be run under a fully local environment. This is because once one knows which transactions to query, the node itself delivers that data quite quickly. Furthermore, TrueBlocks has a built-in client side cache which further speeds up access to the data.

9. TrueBlocks turns a local Ethereum node into a performant data server for end user applications. TrueBlocks dApps are unique, *very fast*, fully-decentralized and distributed applications (that is, they are TRUE dApps). This is how we all thought things should work -- software completely local to the end user's machine with zero web servers snooping on the user's activity.
