## tools/scripts

This folder contains various tools and python scripts that provide simple, helpful utilities that aid in debugging and understanding Ethereum blocks, transactions and accounts.

---

`Usage:` ethscan.py [ block | hash | accounts ]

`Purpose:` Opens EtherScan (http://etherscan.com) website to either a given block number, a given block or transaction hash, or a given Ethereum address. You may specify any number of the above in any combination.

---

`Usage:` ethgoog.py address(es)

`Purpose:` Searches Google for the given address(es) while eliminating results from blockchain scrapers. This makes it easier to find references to a given Ethereum address if one is searching for information about that address.

---

`Usage:` pylint.py <source_code_file>

`Purpose:` We use this source code linter to insure consistent code style. If you add code to the repository, please use this tool before submitting your changes.

**Powered by Qblocks<sup>&trade;</sup>**
