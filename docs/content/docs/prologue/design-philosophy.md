---
title: "Design philosophy"
description: "The philosophy behind the technical design of TrueBlocks"
lead: "A core set of values guides every aspect of TrueBlocks' technical design."
date: 2021-10-04T10:03:01-03:00
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu: 
  docs:
    parent: prologue
weight: 0301
toc: true
---

## The three commandments

1. **Let users query this data without asking third-parties for permission.**

    If you can access blockchain data only through a third party (e.g. a cloud provider),
    is the data really decentralized?

2. **Provide accurate access, to underived, consented-to, pure immutable data.**

    If the blockchain data is not accurate, what good is the blockchain?

3. **Perform well on small machines**

    If an application requires web-scale computer infrastructure,
    does it really support decentralization?

## How TrueBlocks prioritizes design features

No application can do all things. All design decisions involve tradeoffs.
TrueBlocks tries to make trade off _consciously_, so that we always
make the application best serve our core values.

### Core design values

These core design features are _imperative_.
They underlie every single commit made to the TrueBlocks code base.

| Core design values          | What they mean in practice matter                                                                                                           |
| --------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------- |
| Decentralization            | No one should have to rely on anyone other than themselves for data. This including users who want data at the very end of the data journey |
| Performance                 | TrueBlocks should be fast on small machines, and provide options for minimal disc footprints                                                |
| Preserves integrity of data | Blockchain data is mutually-agreed upon and consented-to. Don't tamper with it.                                                             |

### Very important values

These values are particularly important. They are naturally necessary to achieving
the core design features.

| Very important values | What they mean in practice                                                                                                              |
| --------------------- | --------------------------------------------------------------------------------------------------------------------------------------- |
| Transparency          | Every person gets permissionless access to all the data they want; the application should be open source, letting users verify the data |
| Reproducibility       | Required for transparency and decentralization                                                                                          |
| Stability             | It should run forever with zero downtime                                                                                                |

### Important values

These values are important, and TrueBlocks tries to maximize these values wherever possible.
However, if we have to make a tradeoff, these values come after the core values.

| Important values | What they mean in practice                                             |
| ---------------- | ---------------------------------------------------------------------- |
| Robustness       | App should work forever without needing to be futzed with              |
| Approachability  | Easy to understand how it works; easy to understand how to get started |
| Simplicity       | Related to Approachability                                             |
| Extensibility    | Others should be able to add to it                                     |
| Maintainability  | Ideally requires no maintenance                                        |
| Debuggability    | An internal concern                                                    |

### Not priorities

While TrueBlocks does not _try_ to sacrifice these values, they are not core concerns.

| Not a priority   | Why these values aren't emphasized                                                                                                                                                                                                                                                                                  |
| ---------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Compatibility    | Depends on programming languages that we use, so not a concern                                                                                                                                                                                                                                                      |
| Composability    | Not really a concern                                                                                                                                                                                                                                                                                                |
| Expressiveness   | Not a concern, readable commands more important                                                                                                                                                                                                                                                                     |
| Interoperability | Only care about data being platform-independent. Production of the index needs to create something that is useful outside of TrueBlocks                                                                                                                                                                             |
| Mensurability    | For internal use only, so less of a concern                                                                                                                                                                                                                                                                         |
| Portability      | Depends on programming language, so not a concern                                                                                                                                                                                                                                                                   |
| Safety           | Not really. Consented to data is safe.                                                                                                                                                                                                                                                                              |
| Security         | Not really. Out of our code's purview other than basic software security. We hold no special data: our data is consented-to data. Reproducible, consented-to data is reproducibly safe. The app runs on local machines. If the local machine is compromised, TrueBlocks is not the user's biggest security concern. |
| Thoroughness     | The application should do one thing perfectly -- reconciliations                                                                                                                                                                                                                                                    |

_This ranking of values was inspired by [a talk given by Bryan Cantrill in 2018](https://www.youtube.com/watch?v=2wZ1pCpJUIM)._

## Mantras for decentralized data

Because "commandments" sounds authoritarian and Abrahamic, we also keep an ongoing list of mantras.

If repeated endlessly, they will eventually sink in:

* All the data, for all the people, all the time, but no data if the user doesn’t want it
* Use only consented-to data…
    * If you can’t do that, use only data derived directly from consented-to data
    * If you can’t do that, find a way to force the data provider to prove their data
* The paradigm has shifted, embrace it - the server is now called localhost
* Break yourself out of the browser. There’s a big wide desktop out there
* It’s easier to scale up than to scale down - decentralize first
* Most users care only about themselves and maybe a few smart contracts
* Some users, but not all, care about everything
* Everyone needs to do accounting and pay taxes
* The node can, and should be, be improved
