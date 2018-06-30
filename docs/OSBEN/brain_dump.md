Some ideas from Emiel in OSBEN telegram group

- Selfdestruct/suicide of a (token) contract should burn account token balances.
      No log/event will likely ever fire.
- Selfdestruct/suicide of a parent contract containing the transfer function should burn
    or at least Mark token balances as unavailable (Parity2 wallet hack).
    
Other brain dumps

- An air drop that does fires mint events but not token transfer events
- An air drop that fires both mint events and transfer events
- An air drop that only fires transfer events
- A foriegn contract call that fails and then the caller reverts
- A foriegn contract call that fails but the caller doesn't revert
- Failed (thrown) Pre-byzantium transactions (note, because the receipt did not have an error code, one would have to --trace to determine error status). Tests run on non-tracing nodes will fail this test.
