### Steps to Migrate
---
- <div style="text-decoration:line-through">Rename all path routines from `getXXXPath` to `getPathToXXX` so we can find them more easily</div>
- <div style="text-decoration:line-through">Put all the PathAccessor functions in a single file to better control them (one for .go, one for .cpp)</div>
- Add `[settings]chain=` option to trueBlocks.toml
- We need to add global command line option `--chain` availble to all tools and overriding any configuration settings.
- Use XDG spec for environment variables
- Expand performance testing to include chain (set to `mainnet` for all records)
- Fix both the `cmake` files and the `make install` if necassary.
- The files and scripts in `build_assets` may have to be re-worked
- We need to document this in the README (search for XDG to find out where)
- Add `chain` to the `status` endpoints
- Add testing to utillib tests to set XDG folders, and try to create non-existant folders
- Finish the migrations-0.24.0.md file
- Make sure all routines that return paths return paths ending with '/'
- When block special blocks are mainnet centric
- Re-write the `trueBlocks.toml` file with version `0.24.0`

### What We Need to Control
---
There are three values we need to control:
- Where is the `trueBlocks.toml` file?
  - `$XDG_DATA_HOME/trueblocks/`, if not empty
  - `$HOME/.local/share/trueblocks/`, if on Linux
  - `$HOME/Library/Application Support/TrueBlocks/`, if on Mac
  - Call this folder `TB_HOME`

- Get the value of `$TB_HOME/trueBlocks.toml:[settings]chain`
  - if not found, create it with default values
  - if missing, add value of `mainnet` and re-write
  - update version when writing out new values
  - call this value `CHAIN`

- Where is configuration
  - `$XDG_CONFIG_HOME/trueblocks/$CHAIN/`, if not empty
  - else the value stored at `trueBlocks.toml:[settings]configPath/$CHAIN/`
  - else `$TB_HOME/config/$CHAIN/`

- Where is cache
  - `$XDG_CACHE_HOME/trueblocks/$CHAIN/`, if not empty
  - else the value stored at `trueBlocks.toml:[settings]cachePath/$CHAIN/`
  - else `TB_HOME/cache/$CHAIN/`

- For all three folders, we create the folder if it doesn't exist
- For the configuration and cache
  - we append the value of trueBlocks.toml [settings]`chain` or `mainnet` if empty

### Rules for checking proper migration in GoLang code
---
- If the version in `trueBlocks.toml` is `0.24.0` or later, no migration needed
- If the `configPath` contains `cache`, `mocked`, `manifest`, `unchained`, `ts.bin.gz`, `poloniex.tar.gz`, tell the user to move it
- If the `configPath` contains any `toml` file other than `trueBlocks.toml`, tell them to move it.
- Auto create if it doesn't exist `configPath/mainnet/config`, folder
- If there is no mainnet folder, fail
- If there is no Abis folder, fail
- If there is no mainnet/config/names.tab, fail
- If there is no mainnet/config/allocs.tab, fail

### Format of new config file
---

```
$BASE_PATH = `~/local/share/trueblocks` or `~/Library/Application Support/TrueBlocks/`

[settings]
chain = ""              # defaults to mainnet (must be one of mainnet, ropstem, rinkeby, goerli, kovan, xdai)
basePath = ""           # defaults to ${BASE_PATH}

[mainnet]
rpcProvider = ""
remoteExplorer = ""

[rinkeby]
rpcProvider = ""
remoteExplorer = ""
```

### Calculated Paths
---
We want the only configuration to be ${BASE_PATH}. All other paths are calculated based on that value and ${CHAIN}

| Name      | Calculated Value                |
| --------- | ------------------------------- |
| cachePath | {BASE_CACHE}/cache/{CHAIN}/     |
| indexPath | {BASE_CACHE}/unchained/{CHAIN}/ |

### Per-Chain Data

| Chain Id | Chain     | Remote Explorer              | Erigon      | Genesis Hash (from Erigon)                                         |
| -------- | --------- | ---------------------------- | ----------- | ------------------------------------------------------------------ |
| 1        | mainnet   | https://etherscan.io         | mainnet     | 0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3 |
| 3        | ropsten   | https://ropsten.etherscan.io | reposten    | 0x41941023680923e0fe4d74a34bdac8141f2540e3ae90623718e47d66d1ca4a2d |
| 4        | rinkeby   | https://rinkeby.etherscan.io | rinkeby     | 0x6341fd3daf94b748c72ced5a5b26028f2474f5f00d824504e4fa37a75767e177 |
| 5        | goerli    | https://goerli.etherscan.io  | goerli      | 0xbf7e331f7f7c1dd2e05159666b3bf8bc7a8a3a9eb1d518969eab529dd9b88c1a |
| 42       | kovan     | https://kovan.etherscan.io   | kovan       | 0xa3c565fc15c7478862d50ccd6561e3c06b24cc509bf388941c25ea985ce32cb9 |
|          |           | https://polygonscan.com      |             |                                                                    |
| 56       |           | https://bscscan.com          | bsc-mainnet | 0x0d21840abff46b96c84b2ac9e10e4f5cdaeb5693cb665db62a2f3b02d2d57b5b |
|          |           | https://hecoinfo.com         |             |                                                                    |
|          |           |                              | erigonmine  | 0xfecd5c85712e36f30f09ba3a42386b42c46b5ba5395a4246b952e655f9aa0f58 |
| 1212120  |           |                              | fermion     | 0x0658360d8680ead416900a552b67b84e6d575c7f0ecab3dbe42406f9f8c34c35 |
| 77       |           |                              | sokol       | 0x5b28c1bfd3a15230c9a46b399cd0f9a6920d432e85381cc6a140b06e8410112f |
|          |           |                              | yolov3      |                                                                    |
| 97       |           |                              | chapel?     | 0x6d3c66c5357ec91d5c43af47e234a939b22557cbb552dc45bebbceeed90fbe34 |
| 1417     |           |                              | rialto?     | 0x005dc005bddd1967de6187c1c23be801eb7abdd80cebcc24f341b727b70311d6 |
|          | xdai      |                              |             | 0x4f1dd23188aab3a76b463e4af801b52b1248ef073c648cbdc4c9333d3da79756 |
|          | polkadot? |                              |             | ???                                                                |
|          | filecoin? |                              |             | ???                                                                |

### Open Questions
----
- What about bytzantium and the 'status' value for non-mainnet chains?
- What about xDai?

### Existing Paths

<br>

#### Go Lang Paths
----
| Name              | Description                                                                                                      |
| ----------------- | ---------------------------------------------------------------------------------------------------------------- |
| GetPathToConfig   | Hard coded in utils.go<br>`~/local/share/trueblocks` (Linux)<br>`~/Library/Application Support/TrueBlocks` (Mac) |
| GetPathToCache    | Found in config file<br>defaults to `GetPathToConfig() + "cache"`                                                |
| GetPathToIndex    | Found in config file<br>defaults to `GetPathToConfig() + "unchained"`                                            |
| GetPathToCommands | Hard coded to $HOME/.local/bin/chifra on both platforms in GetPathToCommands                                     |

<br>

#### C++ Paths
----
| Name              | Description                                                                                              |
| ----------------- | -------------------------------------------------------------------------------------------------------- |
| getPathToConfig   |                                                                                                          |
|                   | getPathToIndex, getPathToCache                                                                           |
| getPathToCache    | `cachePath` from trueBlocks.toml                                                                         |
|                   | getPathToBinaryCache, getPathToPriceDb, getPathToMonitor,<br/>getPathToMonitorDels, getPathToMonitorLast |
| getPathToIndex    | `indexPath` from trueBlocks.toml                                                                         |
| getPathToCommands |                                                                                                          |



<br>

#### Local Tooling Only
----
| Name               | Description                                             |
| ------------------ | ------------------------------------------------------- |
| getPathToDocs      | For internal tooling only - works from `./build` folder |
| getPathToTemplates | For internal tooling only - works from `./build` folder |
| getPathToSource    | For internal tooling only - works from `./build` folder |

Search for IndexPath, CachePath in golang code
Blaze has command line options to explicitly take the paths
We used to preclude the user from customizing an indexPath if it doesn't exist
Should be fail if the XDG paths do not exist?





-------
tjayrush commented on Apr 18, 2021 • 
Until we find a place to put this:

If the user tries to run TrueBlocks against an Ethereum node that is not Parity (OpenEthereum) and/or does not have --tracing option enabled when the node started, TrueBlocks will report that and refuse to run.

The user may disable this by adding the following section and values to the file $CONFIG/blockScrape.toml (where $CONFIG is dependent on the user's operating system):

[requires]
tracing=false
parity=false
balances=false
If the [requires] section does not exist, add it.

If $CONFIG/blockScrape.toml does not exist, create it.

Please note, that while this will allow TrueBlocks to work, it will break many of the test cases. Also, if you are running against a truly non-tracing node (as opposed say to an Erigon node that is reporting incorrectly), the data produced will be significantly less useful (especially for reconciling token and ETH accounting).

If you make this change, we would be interested to learn what you learn, as we do not test against nodes without --tracing enabled nor against non-archive nodes.

Please add any thoughts or experiences below.

Allowing TrueBlocks to Run Against Non-Archive, Non-Tracing, Non-OpenEthereum nodes hasno dependencies
@tjayrush tjayrush added the bug label on Apr 18, 2021
@tjayrush tjayrush self-assigned this on Apr 18, 2021
@tjayrush tjayrush mentioned this issue on Apr 18, 2021
User generated query: Will TrueBlocks work without an archive node TrueBlocks/trueblocks-docs#13
 Closed
@tjayrush tjayrush added the TC-apps label on Apr 27, 2021
@tjayrush tjayrush changed the pipeline from New Issues to IceBox - Bugs for TrueBlocks workspace on Apr 27, 2021
@tjayrush
 
Member
Author
tjayrush commented on May 1, 2021
From issue #1561:

By default, the TrueBlocks scraper will only run against a tracing, OpenEthereum (or parity) node. You can turn this test off, but if you do you will get a 'lesser' index.

The 'tracing' requirement is needed to get 'every appearance of every address on the chain.' There's nothing that can be done about that as about 40-50% of all appearances on the chain only appear in traces.

The 'parity' (or OpenEthereum) requirement is needed for speed especially when querying traces.

The end user may turn both of these requirements off (or any combination) by editing a file called $HOME/.quickBlocks/blockScrape.toml and adding the following section with the following values:

[requires]
tracing=false
parity=false
Note that the index created in this method will be significantly 'short' on entries. Also, while the OpenEthereum node is significantly faster when querying the RPC, that speed mostly manifests itself when querying traces. For example, we think it may be five to ten times faster than Geth (if Geth supports the right routines).

All this to say, that you're welcome to use non-tracing, non-parity nodes, but your performance may vary.

@tjayrush tjayrush mentioned this issue on May 1, 2021
blockScrape: Allow running scraper against non-tracing, non-parity nodes #1561
 Closed
@tjayrush tjayrush added this to the 0.9.1.0 - Beaver milestone on May 6, 2021
@tjayrush tjayrush removed this from the 0.9.1.0 - Beaver milestone on May 18, 2021
@tjayrush tjayrush added this to the 0.9.2.0 - Bedford milestone on May 18, 2021
@tjayrush tjayrush mentioned this issue on Jun 17, 2021
Non-tracing node reports 'node produced an empty response' on chifra status because trace_call is not implemented #1640
 Closed
@tjayrush tjayrush removed this from the 0.9.6.0 - Bedford milestone on Jul 31, 2021
@tjayrush tjayrush added this to the 0.9.4.0 - Beaver milestone on Jul 31, 2021
@tjayrush tjayrush removed this from the 0.9.12.0 - Beaver milestone on Aug 5, 2021
@tjayrush tjayrush added this to  TrueBlocks/trueblocks-core#1781 Non-Mainnet Behaviour on Dec 4, 2021
@tjayrush tjayrush changed the pipeline from IceBox - Bugs to Epic: Non-Mainnet Behaviour for TrueBlocks workspace on Dec 4, 2021
@tjayrush


==============================
If we're running on a non-mainnet chain, this function:

https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/libs/etherlib/node.cpp#L770

Will always fail because only on mainnet does the test transaction have 23 traces. We should come up with a better way than telling people to add an entry in blockScrape.toml of

[requires]
tracing = false
Tracing check on non-mainnet chains will never work hasno dependencies
@perama-v
 
perama-v commented on Jul 9, 2021
For further context, on Goerli, with .toml modified to include:

[requires]
testing = false
The call chifra blocks 5062611 returns data normally, but the call with traces chifra blocks 5062611 -t returns:

{ "data": [] }
As do all calls to chifra traces.

@tjayrush tjayrush added bug TC-libraries labels on Jul 31, 2021
@tjayrush tjayrush changed the pipeline from New Issues to IceBox - Bugs for TrueBlocks workspace on Jul 31, 2021
@tjayrush tjayrush added the duplicate label on Aug 25, 2021
@tjayrush tjayrush changed the pipeline from IceBox - Bugs to 0.23.0 - Bucks - Progress for TrueBlocks workspace on Aug 25, 2021
@tjayrush
 
Member
Author
tjayrush commented on Sep 29, 2021 • 
For further context, on Goerli, with .toml modified to include:

[requires]
testing = false
The call chifra blocks 5062611 returns data normally, but the call with traces chifra blocks 5062611 -t returns:

{ "data": [] }
As do all calls to chifra traces.

I just noticed there used to be a mistake above. It said to set testing to false. It should have said set tracing to false. I corrected the above.

Are you sure you're running against a tracing node? Can you run a curl command against the same node using trace_block for example?

@tjayrush
 
Member
Author
tjayrush commented on Sep 29, 2021
There's another option that you can turn off if trying to run against a non-mainnet node. See below.

@tjayrush
 
Member
Author
tjayrush commented on Sep 29, 2021
There are two options you can set to disable checks for tracing and archive node on other chains. In a file $CONFIG/blockScrape.toml make these settings:

[requires]
tracing = false
archive = false
@tjayrush tjayrush mentioned this issue on Oct 5, 2021
ChifraResponse.json.Unmarshal failure when bad data sent from Erigon #1776
 Closed
@tjayrush tjayrush added this to the 0.16.0.0 - Berks milestone on Oct 16, 2021
@tjayrush tjayrush changed the pipeline from 0.23.0 - Bucks - Progress to IceBox - Bugs for TrueBlocks workspace on Oct 29, 2021
@tjayrush tjayrush added this to  TrueBlocks/trueblocks-core#1781 Non-Mainnet Behaviour on Dec 4, 2021
@tjayrush tjayrush changed the pipeline from IceBox - Bugs to Epic: Non-Mainnet Behaviour for TrueBlocks workspace on Dec 4, 2021
@tjayrush

 
 


==============================
As we saw when one of our users tried to run against the Rinkeby testnet, two files (probably more) are mainnet-centric.

In order to run against two different networks, the user can adjust the indexPath and cachePath values to separate the two networks. This already works.

But, the way the code is laid out now, two files (names.tab and ts.bin.gz) which are installed and read on initial run by the code are mainnet centric. Because of this both names per address and timestamp per block will be wrong on any other network.

The long term solution is to put those databases into IPFS, freshen them just like we do the index, and publish the hashes to the UnchainedIndex smart contract (with appropriate network labels so that they work for any network). (This would include allow others to push hashes to the contract.)

In the meantime, one of the troubles is, that the files are 'installed if not present' when the user does make. This makes it difficult for the user to 'delete the offending files' and have them be "built on first run" because if the files don't exist they are replaced with 'make.'

Possible short term fix: try to figure out from the chain which network we're on (should be simple). Read the pre-canned names.tab and ts.bin on if we're on mainnet. Warn the user otherwise that the files can't be used on non-mainnet chains.


=========================

(1) we need to provide a place in our cache and unchained folders to handle other chains. This is easy as all access to both of those folders are in a single function and we can easily add the chainid to those paths. (We can query the chain ids -- similar to what's described here: https://besu.hyperledger.org/en/stable/Concepts/NetworkID-And-ChainID/). In this way, the user won't have to do anything.

(2) In addition to where we store data (in which cache), there's the issue of where we get data (as we learned with the timestamps file and the manifest file for chifra init). These also can be store per chain using the chain id.

We need to support multiple chains including testnets hasno dependencies
@tjayrush tjayrush mentioned this issue on Sep 3, 2021
Building the index from the command line is not working as designed. #1729
 Closed
@tjayrush tjayrush added enhancement TC-apps labels on Sep 10, 2021
@tjayrush tjayrush changed the pipeline from New Issues to IceBox - Enhancements for TrueBlocks workspace on Sep 10, 2021
@tjayrush tjayrush changed the pipeline from IceBox - Enhancements to Epic: Indexing / Scraper for TrueBlocks workspace on Oct 16, 2021
@tjayrush tjayrush added TX-indexing and removed TC-apps labels on Oct 16, 2021
@tjayrush tjayrush added this to  TrueBlocks/trueblocks-core#1872 Scraper - Epic Issue on Nov 3, 2021
@talespa
 
talespa commented on Nov 18, 2021
I'm testing on Ropsten (id: 3) but when i try to run '$ chifra init' i have a FATAL like this:

~/trueblocks-core$ chifra init
INFO[18-11|15:11:17.902] hashToIndexFormatFile: Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2
INFO[18-11|15:11:17.902] hashToBloomFormatFile: QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD
INFO[18-11|15:11:17.902] unchainedIndexAddr: 0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd
INFO[18-11|15:11:17.902] manifestHashEncoding: 0x337f3f32
INFO[18-11|15:11:17.902] Calling unchained index smart contract...
FATAL[18-11|15:11:18.617] while unpacking value: abi: attempting to unmarshall an empty string while arguments are expected
@tjayrush
 
Member
Author
tjayrush commented on Nov 18, 2021
So sorry for your trouble. This is definitely a bug in our code.

One of the problems we have is that we don't normally test against Ropsten but only because of a lack of resources. I'll mark this as a priority, and try to fix it for you over the next weekend.

@tjayrush tjayrush added the priority label on Nov 18, 2021
@zimbabao
 
zimbabao commented on Nov 23, 2021
ubuntu@:/web3-index/code/trueblocks-core (master)$ ./bin/chifra init
INFO[23-11|23:19:40.310] hashToIndexFormatFile: Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2
INFO[23-11|23:19:40.310] hashToBloomFormatFile: QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD
INFO[23-11|23:19:40.310] unchainedIndexAddr: 0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd
INFO[23-11|23:19:40.311] manifestHashEncoding: 0x337f3f32
INFO[23-11|23:19:40.311] Calling unchained index smart contract...
FATAL[23-11|23:19:40.318] while unpacking value: abi: attempting to unmarshall an empty string while arguments are expected
I see this issue on ubuntu 18.04.1

@somecryptodude
 
somecryptodude commented on Dec 5, 2021
I see this same issue on BSC, and I updated all references from Etherscan's API to bscscan's API. Thanks

@TheGreatAxios
 
TheGreatAxios commented 12 days ago
Is there any update on this? When using locally with a custom side chain this error is duplicated.

Thanks!

@tjayrush tjayrush changed the pipeline from Epic: Indexing / Scraper to Epic: Non-Mainnet Behaviour for TrueBlocks workspace 11 days ago
@tjayrush
 
Member
Author
tjayrush commented 11 days ago • 
We just released v0.22.0-alpha which was a lot of work related to the performance of the names database. I'm debating what's next...

The major problem for us is one of resources. We're severely limited. Without explicit financial support, I have a feeling this issue will lag quite substantially other than support for the merge and Eth 2.0 which we will be forced to support.

It would be a massive help to us if those interested could help us categorize the issues. We know of a few already (names, indexes, determining tracing). But, the biggest issue, is setting up and gaining access to the RPC of these other chains. We run our own Ethereum mainnet clients, and that, in and of itself, is quite a lot of work.

We would hire-out other endpoints, but the way TrueBlocks hits the node, we get almost instantaneously rate-limited and we can't afford to hire-out dedicated servers.

We're open to solutions...


=======================
Uniswap pricing of reconciliations obviously doesn't work on non-mainnet.


=====================
(1) we need to provide a place in our cache and unchained folders to handle other chains. This is easy as all access to both of those folders are in a single function and we can easily add the chainid to those paths. (We can query the chain ids -- similar to what's described here: https://besu.hyperledger.org/en/stable/Concepts/NetworkID-And-ChainID/). In this way, the user won't have to do anything.

(2) In addition to where we store data (in which cache), there's the issue of where we get data (as we learned with the timestamps file and the manifest file for chifra init). These also can be store per chain using the chain id.

We need to support multiple chains including testnets hasno dependencies
@tjayrush tjayrush mentioned this issue on Sep 3, 2021
Building the index from the command line is not working as designed. #1729
 Closed
@tjayrush tjayrush added enhancement TC-apps labels on Sep 10, 2021
@tjayrush tjayrush changed the pipeline from New Issues to IceBox - Enhancements for TrueBlocks workspace on Sep 10, 2021
@tjayrush tjayrush changed the pipeline from IceBox - Enhancements to Epic: Indexing / Scraper for TrueBlocks workspace on Oct 16, 2021
@tjayrush tjayrush added TX-indexing and removed TC-apps labels on Oct 16, 2021
@tjayrush tjayrush added this to  TrueBlocks/trueblocks-core#1872 Scraper - Epic Issue on Nov 3, 2021
@talespa
 
talespa commented on Nov 18, 2021
I'm testing on Ropsten (id: 3) but when i try to run '$ chifra init' i have a FATAL like this:

~/trueblocks-core$ chifra init
INFO[18-11|15:11:17.902] hashToIndexFormatFile: Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2
INFO[18-11|15:11:17.902] hashToBloomFormatFile: QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD
INFO[18-11|15:11:17.902] unchainedIndexAddr: 0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd
INFO[18-11|15:11:17.902] manifestHashEncoding: 0x337f3f32
INFO[18-11|15:11:17.902] Calling unchained index smart contract...
FATAL[18-11|15:11:18.617] while unpacking value: abi: attempting to unmarshall an empty string while arguments are expected
@tjayrush
 
Member
Author
tjayrush commented on Nov 18, 2021
So sorry for your trouble. This is definitely a bug in our code.

One of the problems we have is that we don't normally test against Ropsten but only because of a lack of resources. I'll mark this as a priority, and try to fix it for you over the next weekend.

@tjayrush tjayrush added the priority label on Nov 18, 2021
@zimbabao
 
zimbabao commented on Nov 23, 2021
ubuntu@:/web3-index/code/trueblocks-core (master)$ ./bin/chifra init
INFO[23-11|23:19:40.310] hashToIndexFormatFile: Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2
INFO[23-11|23:19:40.310] hashToBloomFormatFile: QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD
INFO[23-11|23:19:40.310] unchainedIndexAddr: 0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd
INFO[23-11|23:19:40.311] manifestHashEncoding: 0x337f3f32
INFO[23-11|23:19:40.311] Calling unchained index smart contract...
FATAL[23-11|23:19:40.318] while unpacking value: abi: attempting to unmarshall an empty string while arguments are expected
I see this issue on ubuntu 18.04.1

@somecryptodude
 
somecryptodude commented on Dec 5, 2021
I see this same issue on BSC, and I updated all references from Etherscan's API to bscscan's API. Thanks

@TheGreatAxios
 
TheGreatAxios commented 12 days ago
Is there any update on this? When using locally with a custom side chain this error is duplicated.

Thanks!

@tjayrush tjayrush changed the pipeline from Epic: Indexing / Scraper to Epic: Non-Mainnet Behaviour for TrueBlocks workspace 11 days ago
@tjayrush
 
Member
Author
tjayrush commented 11 days ago • 
We just released v0.22.0-alpha which was a lot of work related to the performance of the names database. I'm debating what's next...

The major problem for us is one of resources. We're severely limited. Without explicit financial support, I have a feeling this issue will lag quite substantially other than support for the merge and Eth 2.0 which we will be forced to support.

It would be a massive help to us if those interested could help us categorize the issues. We know of a few already (names, indexes, determining tracing). But, the biggest issue, is setting up and gaining access to the RPC of these other chains. We run our own Ethereum mainnet clients, and that, in and of itself, is quite a lot of work.

We would hire-out other endpoints, but the way TrueBlocks hits the node, we get almost instantaneously rate-limited and we can't afford to hire-out dedicated servers.

We're open to solutions...

===================================
On adding custom abis to TrueBlocks, what worked for me on Rinkeby is the following:

Generate an abi for a smartcontract and export it somewhere as json. My dev toolchain is based on Hardhat. So I used https://github.com/symblox/hardhat-abi-gen (I picked is from the top of a google search--"hardhat generate abi". @symblox forked it from another project. I did zero due diligence on the project. So, use it at your own risk.). The resulting json is an undecorated array of signature elements with a .json file naming convention. My gen tool, hardhat-abi-gen, sticks this file in [projectRoot]/abi.
Copy .json to [cachePath]\abis, and change the file's name to .json.
Run chifra export --articulate --fmt json . The resulting output showed proper articulation of prior transactions from calls to custom methods in the contract.
Run chifra abis . Dumped output that looked right based on cursory eyeballing.
GitHub
GitHub - symblox/hardhat-abi-gen: 🧰 Export Solidity contract ABIs o...
🧰 Export Solidity contract ABIs on compilation ⚙️. Contribute to symblox/hardhat-abi-gen development by creating an account on GitHub.
Comment from user about ABI generation hasno dependencies
@davidfsol5
 
davidfsol5 commented on Dec 2, 2021 • 
Other observations:

Running Step 4 before running Step 2 generates an empty [contractAddress].json file.
My contract references another contract. Chifra autogenerated an empty [otherContractAddress].json.
I tried an ABI file that added a top-level "address" element and that named the signature element array "abi". Chifra was not able to articulate from this file.
@davidfsol5
 
davidfsol5 commented on Dec 2, 2021
Normal hardhat compilation leaves contract abis in the build/artifacts/contracts directory. So, no need for hardhat-abi-gen as long as TrueBlocks can use the enriched content--more elements than just the method signature array.

I've attached an example abi from the openZeppelin tutorial contract, Box, for formatting.
Box.zip

@tjayrush tjayrush added bug TC-tools labels on Dec 4, 2021
@tjayrush tjayrush changed the pipeline from New Issues to IceBox - Bugs for TrueBlocks workspace on Dec 4, 2021
@tjayrush tjayrush changed the pipeline from IceBox - Bugs to Epic: Non-Mainnet Behaviour for TrueBlocks workspace 8 days ago
@tjayrush

 
 

