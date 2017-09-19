### Possible Use Cases for QuickBlocks
_Taglines:	Accounting for the Revolution_
		_It’s all about the data_
[Important note on everything on this page: it all works with Enterprise Ethereum as well.]

**Better RPC:**
- End users: Smart contract developers, auditors/regulators, accountants
- Competition: Infura (maybe as an addition to or in competition to)
- Runs as either AWS API or on local computer (Blockchain on a Stick™)
- Benefits
    - 100 times faster
    - Much better data
    - As an SDK it is fully decentralized
    - Automatically created code
    - Code never needs to be maintained
    - ERC20 Token and Wallet contracts built-in

**Smart Contract Monitoring / Reporting:**
- End Users: Smart contract developers, smart contract participants (i.e. token holders)
- Competition: None
- Send email or text notifications when “weird” things happen
    - Recursive attacks
    - Mismatches between token balances and amount of ether (invariants)
    - Customized monitoring
    - Largest purchase or other “events” monitoring
- Reporting
    - Total token holders
    - Amount of ether
    - Outflows vs. inflows
- Reporting and monitoring can be per-contract, per-contract-group, industry, or Blockchain-wide.
- This is a new type of “smart contract security” that no-one else has mentioned
- Could sell “insurance”
- Marketing efforts create an expectation in the community that every smart contract should be monitored

**Automated Tax Returns:**
- Users: Any individual users, any smart contract owner, accountants
- Given a list of accounts, create credits and debits to each account
- Attach spot prices (by averaging five exchanges for example). (This data, being historical, can be cached.)
- Calculate gains/losses for tax purposes
- With addition of API data from exchange websites (Kraken, CoinBase) could also report on exchange held accounts

**Testing / Debugging Aids:**
- End Users: Smart contract testing engineers, developers
- Competition: None
- Record and playback live interactions with a already deployed smart contracts
- Programmable, so test engineers can build test cases ad-hoc or modify live playbacks
- Use against proposed new versions of the smart contract (with programmatic modification of the data to meet the new contract)
- Analyze gas consumption to lower costs for both end users and contract owners

**Auditing support:**
- Users: Regulators / Auditors
- Fully parsed data makes for much easier auditing of smart contracts
- Can provide 3rd party (outside generated) data to support investors and others
- Could search for non-delivery of promises (i.e. provably true gambling sites—are they really paying out what they say they are? Gambling Watch™).

**Account ‘clustering’:**
- Uses: Reporting function at global scale
- Using computational geometry code from VRaniML, I can visually show ‘relatedness’ of accounts.
- Strength of link is related to either number of interactions or value of interactions
- Might be using in sharding the chain (under the assumption that one would want closely related addresses to belong to the same shard and that in almost all cases accounts are tightly coupled)

**Consulting: Gas Optimization**
- I can figure out the cost of deploying a contract (or series of contracts). This would be an analysis of the cost of running the contract if the contract starts paying for transactions.

**Consulting: Smart contract auditing**
- There is very little discussion of Logging as a method to increase smart contract security.
- Example: SingularDTV contracts have no events! Impossible to audit.
- Protect every ‘send’ and ‘call’ with events has two purposes
- Makes auditing much easier
- Captures recursive attacks
- See my comment on this smart contract security audit: 

**Ongoing Conversations:**
	See other document

**People doing similar work:**
- https://medium.com/everex/ethplorer-2-0-tokens-for-investors-widgets-and-api-for-developers-d985d751cea2#.c1wdhkdmo (centralized)
- http://etherscan.io (centralized)
- http://infura.com (centralized)
