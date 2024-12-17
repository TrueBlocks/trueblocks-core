---
title: "Install Docker"
description: "Install the Docker Version of TrueBlocks "
lead: "Explorer is the GUI frontend for TrueBlocks"
date: 2021-07-02T08:01:07-03:00
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: true
images: []
menu: 
  docs:
    parent: install
weight: 0400
toc: true
---

First, make sure that you have the `chifra-core` backend up and running,
then run the application from [a clone of the explorer repo](https://github.com/TrueBlocks/trueblocks-explorer).

## Before you begin

&#9745; Set up the trueblocks backend using the [chifra installation instructions](/docs/install/install-core)

&#9745; In a terminal window, run the command `chifra serve`

## Install

Keep the `chifra serve` process running.

Then, in a new terminal, run these commands:

1. git clone git@github.com:TrueBlocks/trueblocks-explorer.git
2. cd trueblocks-explorer
3. cp .env.example .env
4. yarn
5. yarn develop

Open your browser, and access the app from `localhost:1234`.
