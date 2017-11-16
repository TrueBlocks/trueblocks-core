*************
Use Cases
*************

Access to faster, more informative data ("articulated data") from the Ethereum blockchain opens up the possibility of a richer collection of applications than has been previously possible. Here we provide a list of potential applications that may be built on top of the QuickBlocks libraries.

Accounting / Auditing Functions
--------------------------------

**A Better RPC Interface (Web3.2)**

- Improves on the experience of smart contract developers and aids regulators, auditors and accountants by providing improved access to the Ethereum data. May run either remotely (AWS, Digital Ocean) or locally against your own local-running Ethereum node.

:End users: Smart contract developers, auditors/regulators, accountants
:Notes: When running locally, 100s times faster than existing methods; significantly improved (articulated) data fed directly into traditional databases; programmable SDK; example source code; ability to automatically generate code; code does not need to be maintained; built in support for ERC20 tokens and wallet contracts

**Smart Contract Monitoring:**

- Actively monitor one (or more) Ethereum smart contracts and user accounts (or any combination) watching for odd or 'known-dangerous' transactional patterns. Report to anomalies to a list of email, SMS, web site, or individuals whenever something of interest happens.

:End Users: Smart contract developers, smart contract participants (i.e. token holders)
:Notes: 'Weird' things include recursive attacks,  violations of invariants (token balances to ether balance), largest purchases, most active trader accounts, etc.; Could potentially spawn an "insured" smart contract industry expectation.

**Smart Contract Reporting:**

- Instantaneous "Quarterly" reports available every second. On demand reports generated for cap tables (report on token holders), individual ether holdings and transaction histories (i.e. bank statements) on a per-account, per-contract-group, by industry, or system wide.

:End Users: Smart contract developers, smart contract participants (i.e. token holders), economists, regulators
:Notes: Allows for self-reporting on business processes, expenditures, and revenue from outside an organization--no need to wait for company reports; marketing efforts might engender an expectation that every smart contract's accounting is fully transparent.

**Automated Tax Returns:**

- Automated tax reporting for any jurisdiction showing dates and cost of acquisitions, cost basis, holding period, dates and revenues on sales, and any tax liabilities.

:End Users: Individual users, purveyors of smart contract systems, accountants, auditors
:Notes: Historical spot prices from agreed-upon source for each currency and/or token can be shared across the planet. With the addition of APIs from popular crypto exchanges (i.e. Kraken, CoinBase) could also report on exchange-held accounts

**Auditing Support:**

Provide data and transactional information to third parties not associated with the development team of a smart contract system. Interesting to potential investors, industry analysts, auditors and/or regulators.

:End Users: regulators, auditors, potential investors
:Note: Fully parsed data makes for much easier auditing of smart contracts, could expose non-delivery of promised behavior (i.e. are "provably true" gambling sites actually paying out at the rate they claim? Gambler Watch™).

Developer Ecosystem
--------------------

**Testing / Debugging Aids:**

- Record and play back "real world" interactions with a already deployed smart contracts. Being programmable, test engineers may use QuickBlocks to build test cases ad-hoc (fuzzing) or modify previously recorded live playbacks.

:End Users: Smart contract testing engineers, smart contract developers
:Notes: Could be used against proposed new versions of the smart contract (with programmatic modification of the data to meet the new contract's interface); could be used to aid in gas optimization

**Account ‘Clustering’:**

Using computational geometry code already written, visualize 'relatedness' of accounts, relationship between transactions, usage trends, upward or downward movement of token transfer activity, and myriad other things.

:End Users: Economists, data scientists interest in the system-wide behavior
:Notes: The strength of link could be related to the number of interactions between two account or the total value transferred; computational geometry (graph-based data structures) might be useful in sharding the chain (under the assumption that one would want interrelated accounts to belong to the same shard and that in most cases accounts are tightly coupled).

Consulting Opportunities
-------------------------

**Gas Optimization**

- As Ethereum progresses and it becomes possible for smart contracts to pay for their end users' gas usage, it will become increasingly important to optimize for gas usage in a smart contract. Existing tools report only on expected gas usage based on estimated behavior. QuickBlocks can report on actually "live" data captured in the wild. A consulting service or product ideas could be built on top of this capability.

**Smart Contract Auditing**

- Some smart contracts (SingularDTV as an example) do not have adequate logging / event generation built into them. This makes it nearly impossible to properly account for their behavior. Certain activity in a smart contract can be "protected" with semaphore events that can surround value transfers, for example. These 'semaphores' if not properly closed would indicate a recursive attack. There is an opportunity for consulting related to full instrumenting smart contracts with "active events" that aid in the monitoring function.
