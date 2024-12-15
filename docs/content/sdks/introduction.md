---
title: "SDKs"
description: "We've generated a few SDKs for your use"
lead: ""
draft: false
url: "/sdks/introduction"
menu: 
  sdks:
    parent: introduction
weight: 1000
toc: true
---

[Chifra serve](/chifra/admin/#chifra-daemon) provides an API interface to the `chifra` command line. As part of this
functionality, we've written two SDKs for Python and TypeScript to make using the API easier. We welcome
contributions for other SDKs.

The GoLang SDK is different. Unlike the other SDKs, the GoLang SDK does not require you to run the API server.
This has very significant performance implications because there is no serialization of the data. In the
server case, the code must read the data from the RPC (or the local binary cache), turn it into a JSON string,
and send it to the TypeScript or Python SDK. 

In the GoLang SDK, the data is in memory ready for use directly from disc. No strinification. This makes the
GoLang SDK as fast as it can possbily be.

The two API-releated SDKs are the [TypeScript SDK](/sdks/typescript-sdk/) and the [Python SDK](/sdks/python-sdk/).

The GoLang SDKs is here [GoLang SDK](/sdks/go-sdk/).

Here's the [SDKs repo](https://github.com/TrueBlocks/trueblocks-core/blob/master/sdk/README.md).
