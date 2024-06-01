# Future Examples

- Getting started / Installation
  - This will just show the meta data and help the user learn how to build their own examples.

- Getting Status from the Node
  - Query the node to make sure the system is working.

- Eth Balances for Three Accounts
  - This will query for balance of three accounts (using name to find Vitalik for example, a .eth, and a hard coded address).

- Eth Balances over Time
  - Using When, find some blocks per date. Show balance histories per date.

- Token Balances over Time
  - Find a token, find some dates, get token balances over time.

- Monthly Bank Statements
  - Use chifra export --accounting --balances over time.

- For Every Chain
  - Do it for more than one chain.

- 1000 times more data in the same amount of time
  - Start by showing time chifra blocks 1000000-2000000:10000. This takes about X seconds. No cache -- querying the node.
  - Next add --hashes to the command. Takes about 1/4 the time. Still queries the node. No cache. But faster. Points out that TrueBlocks is flexibile.
  - Next add --cache. Same amount of time (perhaps even slower because still querying node but now writing the to cache.
  - Next rerun previous command. Out of cache. Takes X/500 seconds.
  - Next change :10000 to :100 (100 times more records). Still 5 times faster than original.
  - Next change to :10 (1000 more records than original). Only twice as slow.
  - 500 times faster data.
  - Allows for much deeper analysis.
  - Allows for iteration.
  - Can go back and forth between 10000 (for exploration) and 10 (for detail work).
  - Better Data Faster

- Remote RPC vs. Local RPC vs. Cached Local Data
  - Follow on to 7. Starts with cached data - super fast.
  - Shows --decache (allows for cleanup)
  - Show slowdown of reading from local node instead of cache.
  - Switch to remote node.
  - Expensive. Slow as shit. Not private.
  - Better Data Faster.
