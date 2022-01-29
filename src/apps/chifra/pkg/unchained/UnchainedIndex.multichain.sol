/*

See the other file also -- UnchainedIndexv2.sol

Version 2
    (Required):
    -- needs to accept chain (string) for any publication
    -- needs to store the hashes per chain
    -- our manifest needs to change to reference two additional peices of
       data. The timestamp file and the names database (in csv format).

    (Would be nice)
    -- A value called 'upgradedTo' that accepts an address and stays empty
        until the owner sets it. If non-empty, all functions are disabled
        (through a require) and the value points to the upgraded contract

    -- Marketing material surrounding the idea that this hash is the cheapest
        possible way to publish access to the entire chain's index

    -- Allow others to submit names files for possible inclusion in the names database
*/

// SPDX-License-Identifier: MIT
pragma solidity ^0.8.11;

contract UnchainedIndex {
    constructor() {
        owner = msg.sender;
        chainNameToHash["mainnet"] = "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm"; // empty file
        emit HashPublished("mainnet", chainNameToHash["mainnet"]);
        emit OwnerChanged(address(0), owner);
    }

    function publishHash(string memory chainName, string memory hash) public {
        require(msg.sender == owner, "msg.sender must be owner");
        chainNameToHash[chainName] = hash;
        emit HashPublished(chainName, hash);
    }

    function changeOwner(address newOwner) public returns (address oldOwner) {
        require(msg.sender == owner, "msg.sender must be owner");
        oldOwner = owner;
        owner = newOwner;
        emit OwnerChanged(oldOwner, newOwner);
        return oldOwner;
    }

    function donate() public payable {
        require(owner != address(0), "owner is not set");
        emit DonationSent(owner, msg.value, block.timestamp);
        payable(owner).transfer(address(this).balance);
    }

    event HashPublished(string chainName, string hash);
    event OwnerChanged(address oldOwner, address newOwner);
    event DonationSent(address from, uint256 amount, uint256 ts);

    mapping (string=>string) public chainNameToHash;
    address public owner;
}
