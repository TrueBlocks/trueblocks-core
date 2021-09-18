A Monitor is a list of [Appearances](/data-model/accounts/#appearance) associated with a given address along with various details about those appearances. A monitor is created when a user expresses interest in an address by calling either [chifra list](/docs/chifra/accounts/#chifra-list) or [chifra export](/docs/chifra/accounts/#chifra-export) tool (or querying thier associated APIs).

Once created, a monitor may be periodically *freshened* by calling either `chifra list` or `chifra export`, however, it is also possible to freshen a monitor continually with [chifra scrape --monitors](/docs/chifra/admin/#chifra-scrape). This tool watches the front of the chain and repeatedly calls `chifra list`.

[{FIELDS}]

| **Tools**                                                   |                                                              |
| ----------------------------------------------------------- | ------------------------------------------------------------ |
| [chifra status monitors](/docs/chifra/admin/#chifra-status) | report on the status of the TrueBlocks system                |
| [chifra list](/docs/chifra/accounts/#chifra-list)           | list appearances for one or more addresses                   |
| [chifra export](/docs/chifra/accounts/#chifra-export)       | export full detail of transactions for one or more addresses |
