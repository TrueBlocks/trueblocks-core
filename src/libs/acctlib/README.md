## acctlib

The acctlib provides functionality to store per-account data, such as a list of transactions of interest for an account, in a quickly accessible 16-way tree data structure [5] keyed by account address. This data structure lends itself very well to storing per-account data particularly given the nature of the indexing key which is a 20-byte account address.