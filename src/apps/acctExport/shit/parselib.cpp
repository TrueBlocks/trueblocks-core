/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */
#include "tokenlib.h"
#include "walletlib.h"

//-----------------------------------------------------------------------
const string_q func_changeMembership = "0x824dbc9a";
const string_q func_changeUnicorn = "0x522103fa";
const string_q func_changeVotingRules = "0xbcca1fd3";
const string_q func_checkProposalCode = "0xeceb2945";
const string_q func_debatingPeriodInMinutes = "0x69bd3436";
const string_q func_executeProposal = "0x237e9492";
const string_q func_freezeAccount = "0xe724529c";
const string_q func_frozenAccount = "0xb414d4b6";
const string_q func_majorityMargin = "0xaa02a90f";
const string_q func_memberId = "0x39106821";
const string_q func_members = "0x5daf08ca";
const string_q func_minimumQuorum = "0x8160f0b5";
const string_q func_mintToken = "0x79c65068";
const string_q func_newProposalInEther = "0xb9f256cd";
const string_q func_newProposalInWei = "0x83876bc9";
const string_q func_numProposals = "0x400e3949";
const string_q func_owner = "0x8da5cb5b";
const string_q func_priceOfAUnicornInFinney = "0x50c34878";
const string_q func_proposals = "0x013cf08b";
const string_q func_spentAllowance = "0xdc3080f2";
const string_q func_transferOwnership = "0xf2fde38b";
const string_q func_unicornAddress = "0xb7782455";
const string_q func_vote = "0xd3c0715b";
const string_q func_acceptRegistrarTransfer = "0xea9e107a";
const string_q func_cancelBid = "0x2525f5c1";
const string_q func_ens = "0x3f15457f";
const string_q func_entries = "0x267b6922";
const string_q func_eraseNode = "0xde10f04b";
const string_q func_finalizeAuction = "0x983b94fb";
const string_q func_getAllowedTime = "0x13c89a8f";
const string_q func_invalidateName = "0x15f73331";
const string_q func_isAllowed = "0x93503337";
const string_q func_launchLength = "0xae1a0b0c";
const string_q func_newBid = "0xce92dced";
const string_q func_registryStarted = "0x9c67f06f";
const string_q func_releaseDeed = "0x0230a07c";
const string_q func_rootNode = "0xfaff50a8";
const string_q func_sealedBids = "0x5e431709";
const string_q func_shaBid = "0x22ec1244";
const string_q func_startAuction = "0xede8acdb";
const string_q func_startAuctions = "0xe27fe50f";
const string_q func_startAuctionsAndBid = "0xfebefd61";
const string_q func_state = "0x61d585da";
const string_q func_transfer0122 = "0x79ce9fac";
const string_q func_transferRegistrars = "0x5ddae283";
const string_q func_unsealBid = "0xaefc8c72";
const string_q func_unsealBid6090 = "0x47872b42";

//-----------------------------------------------------------------------
string_q promoteToFunc(const CTransaction *p) {

    if (p && (p->input.length()>=10 || p->input == "0x")) {

        string_q items[256];
        uint32_t nItems=0;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_changeMembership)
        {
            // function changeMembership(address targetMember, uint256 voteWeight, bool canAddProposals, string memberName)
            // 0x824dbc9a
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "bool";
            items[nItems++] = "string";
            return decodeRLP("changeMembership", params, nItems, items);

        } else if (encoding == func_changeUnicorn)
        {
            // function changeUnicorn(uint256 newUnicornPriceInFinney, address newUnicornAddress)
            // 0x522103fa
            items[nItems++] = "uint256";
            items[nItems++] = "address";
            return decodeRLP("changeUnicorn", params, nItems, items);

        } else if (encoding == func_changeVotingRules)
        {
            // function changeVotingRules(uint256 minimumQuorumForProposals, uint256 minutesForDebate, int256 marginOfVotesForMajority)
            // 0xbcca1fd3
            items[nItems++] = "uint256";
            items[nItems++] = "uint256";
            items[nItems++] = "int256";
            return decodeRLP("changeVotingRules", params, nItems, items);

        } else if (encoding == func_executeProposal)
        {
            // function executeProposal(uint256 proposalNumber, bytes transactionBytecode)
            // 0x237e9492
            items[nItems++] = "uint256";
            items[nItems++] = "bytes";
            return decodeRLP("executeProposal", params, nItems, items);

        } else if (encoding == func_freezeAccount)
        {
            // function freezeAccount(address target, bool freeze)
            // 0xe724529c
            items[nItems++] = "address";
            items[nItems++] = "bool";
            return decodeRLP("freezeAccount", params, nItems, items);

        } else if (encoding == func_mintToken)
        {
            // function mintToken(address target, uint256 mintedAmount)
            // 0x79c65068
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            return decodeRLP("mintToken", params, nItems, items);

        } else if (encoding == func_newProposalInEther)
        {
            // function newProposalInEther(address beneficiary, uint256 etherAmount, string JobDescription, bytes transactionBytecode)
            // 0xb9f256cd
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "string";
            items[nItems++] = "bytes";
            return decodeRLP("newProposalInEther", params, nItems, items);

        } else if (encoding == func_newProposalInWei)
        {
            // function newProposalInWei(address beneficiary, uint256 weiAmount, string JobDescription, bytes transactionBytecode)
            // 0x83876bc9
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "string";
            items[nItems++] = "bytes";
            return decodeRLP("newProposalInWei", params, nItems, items);

        } else if (encoding == func_transferOwnership)
        {
            // function transferOwnership(address newOwner)
            // 0xf2fde38b
            items[nItems++] = "address";
            return decodeRLP("transferOwnership", params, nItems, items);

        } else if (encoding == func_vote)
        {
            // function vote(uint256 proposalNumber, bool supportsProposal, string justificationText)
            // 0xd3c0715b
            items[nItems++] = "uint256";
            items[nItems++] = "bool";
            items[nItems++] = "string";
            return decodeRLP("vote", params, nItems, items);

        } else if (encoding == func_acceptRegistrarTransfer)
        {
            // function acceptRegistrarTransfer(bytes32 hash, address deed, uint256 registrationDate)
            // 0xea9e107a
            items[nItems++] = "bytes32";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            return decodeRLP("acceptRegistrarTransfer", params, nItems, items);

        } else if (encoding == func_cancelBid)
        {
            // function cancelBid(address bidder, bytes32 seal)
            // 0x2525f5c1
            items[nItems++] = "address";
            items[nItems++] = "bytes32";
            return decodeRLP("cancelBid", params, nItems, items);

        } else if (encoding == func_eraseNode)
        {
            // function eraseNode(bytes32[] labels)
            // 0xde10f04b
            items[nItems++] = "bytes32[]";
            return decodeRLP("eraseNode", params, nItems, items);

        } else if (encoding == func_finalizeAuction)
        {
            // function finalizeAuction(bytes32 _hash)
            // 0x983b94fb
            items[nItems++] = "bytes32";
            return decodeRLP("finalizeAuction", params, nItems, items);

        } else if (encoding == func_invalidateName)
        {
            // function invalidateName(string unhashedName)
            // 0x15f73331
            items[nItems++] = "string";
            return decodeRLP("invalidateName", params, nItems, items);

        } else if (encoding == func_newBid)
        {
            // function newBid(bytes32 sealedBid)
            // 0xce92dced
            items[nItems++] = "bytes32";
            return decodeRLP("newBid", params, nItems, items);

        } else if (encoding == func_releaseDeed)
        {
            // function releaseDeed(bytes32 _hash)
            // 0x0230a07c
            items[nItems++] = "bytes32";
            return decodeRLP("releaseDeed", params, nItems, items);

        } else if (encoding == func_startAuction)
        {
            items[nItems++] = "bytes32";
            return decodeRLP("startAuction", params, nItems, items);

        } else if (encoding == func_startAuctions)
        {
            // function startAuctions(bytes32[] _hashes)
            // 0xe27fe50f
            items[nItems++] = "bytes32[]";
            return decodeRLP("startAuctions", params, nItems, items);

        } else if (encoding == func_startAuctionsAndBid)
        {
            // function startAuctionsAndBid(bytes32[] hashes, bytes32 sealedBid)
            // 0xfebefd61
            items[nItems++] = "bytes32[]";
            items[nItems++] = "bytes32";
            return decodeRLP("startAuctionsAndBid", params, nItems, items);

        } else if (encoding == func_transfer0122)
        {
            // function transfer(bytes32 _hash, address newOwner)
            // 0x79ce9fac
            items[nItems++] = "bytes32";
            items[nItems++] = "address";
            return decodeRLP("transfer0122", params, nItems, items);

        } else if (encoding == func_transferRegistrars)
        {
            // function transferRegistrars(bytes32 _hash)
            // 0x5ddae283
            items[nItems++] = "bytes32";
            return decodeRLP("transferRegistrars", params, nItems, items);

        } else if (encoding == func_unsealBid)
        {
            // function unsealBid(bytes32 _hash, address _owner, uint256 _value, bytes32 _salt)
            // 0xaefc8c72
            items[nItems++] = "bytes32";
            items[nItems++] = "address";
            items[nItems++] = "uint256";
            items[nItems++] = "bytes32";
            return decodeRLP("unsealBid", params, nItems, items);

        } else if (encoding == func_unsealBid6090)
        {
            // function unsealBid(bytes32 _hash, uint256 _value, bytes32 _salt)
            // 0x47872b42
            items[nItems++] = "bytes32";
            items[nItems++] = "uint256";
            items[nItems++] = "bytes32";
            return decodeRLP("unsealBid6090", params, nItems, items);

        }
        // falls through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // returns NULL if not promoted
    string_q ret = p->function;
    const CTransaction *t = promoteToToken(p);
    if (t) {
        ret = t->function;
        if (t != p)
            delete t;
    }
    return ret;
}
