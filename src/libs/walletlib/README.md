## walletlib

Created automatically entirely by the makeClass and grabABI applications, the tokenlib and walletlib libraries implement functionality to provide support for common Ethereum smart 
contract functionality.

We could have allowed each smart-contract that supports the ERC20 token standard to implement these functions on their own, but because of the prevalence of these functions, and because 
for many use cases the ABI file may not be available, we chose to implement this common functionality in libraries. This allows us to support these common types of smart contracts 
without requiring an ABI file from the end user (which is some cases may not be available).

By virtue of being created automatically by makeClass, each of the classes in these libraries may be customized using the //EXISTING_CODE functionality. However, at present, these class 
are not customized.

As a further consequence of being derived from the base classes of the etherlib library, each of these classes may be easily “promoted” to more capable and informative classes. 
The two built-in functions, promote_to_transaction(CTransaction *t), and promote_to_event(CLogEntry *e) are provided to promote any given etherlib class.

In each of these two functions, the parsed data is retrieved and then split to reveal the function or event name and its parameters. Given this information the appropriate class is 
created dynamically and the particular fields of the class are assigned from the parsed parameters.

As difficult as this is to explain, this is one of the fundamental capabilities provided by QuickBlocks™. By being able to promote generic CTransaction instances to richer, more 
informative classes particular to a smart contract, QuickBlocks™ is able to regurgitate or re-deliver richer data than it was given.

Instead of simply delivering the from, to, amount, and timestamp from a transaction along with an unparsed input data field (as is available, at best, from the RPC), QuickBlocks™ 
delivers fully articulated, parsed JSON, tab-delimited, comma-separated or any other format data to a smart contract’s front end or desktop application.

Furthermore, by virtue of being derived ultimately from the CBaseNode, each of these “promoted” classes inherit the ability to create itself dynamically and serialize itself. It 
is therefore possible to build a list of all transactions for a particular account (or series of accounts) or smart contract and store this data in a serialized, binary format for very 
fast, later retrieval.

The tokenlib and walletlib are provided so as to support parsing of a small collection of functions and events related to the Ethereum token standard and multi-sig wallets even in the 
absence of a smart contract’s ABI definition.

In other words, any smart contract that implements the ERC 20 token interface may be successfully promoted even without its ABI. In the presence of an ABI, an even more useful and 
valuable promotion of transactions and events may be accomplished. This feature is described in the next session.
