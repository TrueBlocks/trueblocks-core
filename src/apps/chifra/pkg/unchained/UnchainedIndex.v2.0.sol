/*
NOTE -- ADD THE ABILITY TO TELL WHERE THE NEW CONTACT IS AFTER AN UPGRADE
*/

// SPDX-License-Identifier: MIT
pragma solidity >=0.6.0 <0.7.0;

contract UnchainedIndex {
    constructor() public {
        owner = msg.sender;
        manifestHash = "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm"; // empty file
        timestampsHash = "";
        namesHash = "";
        emit HashPublished("init", manifestHash);
        emit OwnerChanged(address(0), owner);
    }

    function publishTimestamps(string memory hash) public {
        require(msg.sender == owner, "msg.sender must be owner");
        timestampsHash = hash;
        emit HashPublished("timestamps", hash);
    }

    function publishNames(string memory hash) public {
        require(msg.sender == owner, "msg.sender must be owner");
        namesHash = hash;
        emit HashPublished("names", hash);
    }

    function publishHash(string memory hash) public {
        require(msg.sender == owner, "msg.sender must be owner");
        manifestHash = hash;
        emit HashPublished("manifest", hash);
    }

    function changeOwner(address newOwner) public returns (address oldOwner) {
        require(msg.sender == owner, "msg.sender must be owner");
        oldOwner = owner;
        owner = newOwner;
        emit OwnerChanged(oldOwner, newOwner);
        return oldOwner;
    }

    function() payable {
        require(owner != 0x0, "msg.sender is not set");
        emit DonationSent(owner, value, timestamp);
        send(owner, balance);
    }

    event HashPublished(string type, string hash);
    event OwnerChanged(address oldOwner, address newOwner);
    event DonationSent(address from, uint256 amount, uint256 ts);

    string public manifestHash;
    string public timestampsHash;
    string public namesHash;
    address public owner;
}
