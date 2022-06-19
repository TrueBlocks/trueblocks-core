# Unchained Index

Go code to interact with the Unchained Index smart contract, which is purposefully a very simple attestation to an IPFS hash of TrueBlocks' pin manifest.

### Links

[Unchained Index Smart Contract](https://etherscan.io/address/0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd)  
[Unchained Index Owner](https://etherscan.io/address/0xAaCfb3880147061cCA2a9305eb076Bf889731870)  
[Video explaining the project](https://www.youtube.com/watch?v=NvSbMueU7wc)

### Solidity for Unchained Index
```
// SPDX-License-Identifier: MIT
pragma solidity >=0.6.0 <0.7.0;

contract UnchainedIndex {
    constructor() public {
        owner = msg.sender;
        manifestHash = "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm"; // empty file
        emit HashPublished(manifestHash);
        emit OwnerChanged(address(0), owner);
    }

    function publishHash(string memory hash) public {
        require(msg.sender == owner, "msg.sender must be owner");
        manifestHash = hash;
        emit HashPublished(hash);
    }

    function changeOwner(address newOwner) public returns (address oldOwner) {
        require(msg.sender == owner, "msg.sender must be owner");
        oldOwner = owner;
        owner = newOwner;
        emit OwnerChanged(oldOwner, newOwner);
        return oldOwner;
    }

    event HashPublished(string hash);
    event OwnerChanged(address oldOwner, address newOwner);

    string public manifestHash;
    address public owner;
}
```

