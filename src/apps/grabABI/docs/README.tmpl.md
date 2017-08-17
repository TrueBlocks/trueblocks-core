## [{NAME}]

`grabABI` retreives an ABI file either from a local cache, from the Ethereum Name Service (ENS), or from [Etherscan](http://etherscan.io). Once retreived, `grabABI` generates the classDefinition files needed by [makeClass](../makeClass) to generate a parselib for each [Ethereum address monitor](../../monitors).

** Notes: **

There is also an option`--silent` that is useful for scripting. It causes the app to display fewer messages.
[{COMMAND_HELP}]
