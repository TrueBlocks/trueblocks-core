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
 *
 * This monitor code is part of the 'etherTip' monitor.
 */
#include "tokenlib.h"
#include "walletlib.h"
#include "articulate.h"

//-----------------------------------------------------------------------
static const string_q func_changeMembership = "0x824dbc9a";
static const string_q func_changeUnicorn = "0x522103fa";
static const string_q func_changeVotingRules = "0xbcca1fd3";
static const string_q func_checkProposalCode = "0xeceb2945";
static const string_q func_debatingPeriodInMinutes = "0x69bd3436";
static const string_q func_executeProposal = "0x237e9492";
static const string_q func_freezeAccount = "0xe724529c";
static const string_q func_frozenAccount = "0xb414d4b6";
static const string_q func_majorityMargin = "0xaa02a90f";
static const string_q func_memberId = "0x39106821";
static const string_q func_members = "0x5daf08ca";
static const string_q func_minimumQuorum = "0x8160f0b5";
static const string_q func_mintToken = "0x79c65068";
static const string_q func_newProposalInEther = "0xb9f256cd";
static const string_q func_newProposalInWei = "0x83876bc9";
static const string_q func_numProposals = "0x400e3949";
static const string_q func_owner = "0x8da5cb5b";
static const string_q func_priceOfAUnicornInFinney = "0x50c34878";
static const string_q func_proposals = "0x013cf08b";
static const string_q func_spentAllowance = "0xdc3080f2";
static const string_q func_transferOwnership = "0xf2fde38b";
static const string_q func_unicornAddress = "0xb7782455";
static const string_q func_vote = "0xd3c0715b";

//-----------------------------------------------------------------------
bool articulateTransaction(CTransaction *p) {

    if (p->func)
        return false;

    // articulate the events, so we can return with a fully articulated object
    for (size_t i = 0 ; i < p->receipt.logs.size() ; i++)
        articulateEvent(&p->receipt.logs[i]);

    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
        // EXISTING_CODE
        // EXISTING_CODE

        if (encoding == func_changeMembership) {
            // function changeMembership(address targetMember, uint256 voteWeight, bool canAddProposals, string memberName)
            // 0x824dbc9a
            p->func = new CFunction("changeMembership");
            p->func->inputs.push_back(CParameter("targetMember", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("voteWeight", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("canAddProposals", "bool", str_2_Int(extract(params, 2*64, 64))));
            p->func->inputs.push_back(CParameter("memberName", "string", extract(params, 3*64, 64)));
            return true;

        } else if (encoding == func_changeUnicorn) {
            // function changeUnicorn(uint256 newUnicornPriceInFinney, address newUnicornAddress)
            // 0x522103fa
            p->func = new CFunction("changeUnicorn");
            p->func->inputs.push_back(CParameter("newUnicornPriceInFinney", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("newUnicornAddress", "address", str_2_Addr(extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_changeVotingRules) {
            // function changeVotingRules(uint256 minimumQuorumForProposals, uint256 minutesForDebate, int256 marginOfVotesForMajority)
            // 0xbcca1fd3
            p->func = new CFunction("changeVotingRules");
            p->func->inputs.push_back(CParameter("minimumQuorumForProposals", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("minutesForDebate", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("marginOfVotesForMajority", "int256", str_2_Int(extract(params, 2*64, 64))));
            return true;

        } else if (encoding == func_executeProposal) {
            // function executeProposal(uint256 proposalNumber, bytes transactionBytecode)
            // 0x237e9492
            p->func = new CFunction("executeProposal");
            p->func->inputs.push_back(CParameter("proposalNumber", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("transactionBytecode", "bytes", extract(params, 1*64)));
            return true;

        } else if (encoding == func_freezeAccount) {
            // function freezeAccount(address target, bool freeze)
            // 0xe724529c
            p->func = new CFunction("freezeAccount");
            p->func->inputs.push_back(CParameter("target", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("freeze", "bool", str_2_Int(extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_mintToken) {
            // function mintToken(address target, uint256 mintedAmount)
            // 0x79c65068
            p->func = new CFunction("mintToken");
            p->func->inputs.push_back(CParameter("target", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("mintedAmount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            return true;

        } else if (encoding == func_newProposalInEther) {
            // function newProposalInEther(address beneficiary, uint256 etherAmount, string JobDescription, bytes transactionBytecode)
            // 0xb9f256cd
            p->func = new CFunction("newProposalInEther");
            p->func->inputs.push_back(CParameter("beneficiary", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("etherAmount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("JobDescription", "string", extract(params, 2*64, 64)));
            p->func->inputs.push_back(CParameter("transactionBytecode", "bytes", extract(params, 3*64)));
            return true;

        } else if (encoding == func_newProposalInWei) {
            // function newProposalInWei(address beneficiary, uint256 weiAmount, string JobDescription, bytes transactionBytecode)
            // 0x83876bc9
            p->func = new CFunction("newProposalInWei");
            p->func->inputs.push_back(CParameter("beneficiary", "address", str_2_Addr(extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("weiAmount", "uint256", str_2_Wei("0x" + extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("JobDescription", "string", extract(params, 2*64, 64)));
            p->func->inputs.push_back(CParameter("transactionBytecode", "bytes", extract(params, 3*64)));
            return true;

        } else if (encoding == func_transferOwnership) {
            // function transferOwnership(address newOwner)
            // 0xf2fde38b
            p->func = new CFunction("transferOwnership");
            p->func->inputs.push_back(CParameter("newOwner", "address", str_2_Addr(extract(params, 0*64, 64))));
            return true;

        } else if (encoding == func_vote) {
            // function vote(uint256 proposalNumber, bool supportsProposal, string justificationText)
            // 0xd3c0715b
            p->func = new CFunction("vote");
            p->func->inputs.push_back(CParameter("proposalNumber", "uint256", str_2_Wei("0x" + extract(params, 0*64, 64))));
            p->func->inputs.push_back(CParameter("supportsProposal", "bool", str_2_Int(extract(params, 1*64, 64))));
            p->func->inputs.push_back(CParameter("justificationText", "string", extract(params, 2*64, 64)));
            return true;

        }
        // falls through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // never returns NULL
    return articulateToken(p);
}

//-----------------------------------------------------------------------
static const string_q evt_ChangeOfRules = "0xa439d3fa452be5e0e1e24a8145e715f4fd8b9c08c96a42fd82a855a85e5d57de";
static const string_q evt_FrozenFunds = "0x48335238b4855f35377ed80f164e8c6f3c366e54ac00b96a6402d4a9814a03a5";
static const string_q evt_MembershipChanged = "0x4cea7af04e7093442a4d8d039e7b517557c4787a8cf68f900473c9bf3abfae4c";
static const string_q evt_ProposalAdded = "0x646fec02522b41e7125cfc859a64fd4f4cefd5dc3b6237ca0abe251ded1fa881";
static const string_q evt_ProposalTallied = "0xd220b7272a8b6d0d7d6bcdace67b936a8f175e6d5c1b3ee438b72256b32ab3af";
static const string_q evt_Voted = "0xc34f869b7ff431b034b7b9aea9822dac189a685e0b015c7d1be3add3f89128e8";

//-----------------------------------------------------------------------
bool articulateEvent(CLogEntry *p) {

    if (p->func)
        return false;

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        string_q data = extract(p->data, 2);
        // EXISTING_CODE
        // EXISTING_CODE

        if (topic_2_Str(p->topics[0]) % evt_ChangeOfRules) {
            // event ChangeOfRules(uint256 minimumQuorum, uint256 debatingPeriodInMinutes, int256 majorityMargin)
            // 0xa439d3fa452be5e0e1e24a8145e715f4fd8b9c08c96a42fd82a855a85e5d57de
            p->func = new CFunction("ChangeOfRulesEvent");
            p->func->inputs.push_back(CParameter("minimumQuorum", "", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("debatingPeriodInMinutes", "", str_2_Wei("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("majorityMargin", "", str_2_Int("0x" + extract(data, 2*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_FrozenFunds) {
            // event FrozenFunds(address target, bool frozen)
            // 0x48335238b4855f35377ed80f164e8c6f3c366e54ac00b96a6402d4a9814a03a5
            p->func = new CFunction("FrozenFundsEvent");
            p->func->inputs.push_back(CParameter("target", "", str_2_Addr(extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("frozen", "", str_2_Int("0x" + extract(data, 1*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_MembershipChanged) {
            // event MembershipChanged(address member)
            // 0x4cea7af04e7093442a4d8d039e7b517557c4787a8cf68f900473c9bf3abfae4c
            p->func = new CFunction("MembershipChangedEvent");
            p->func->inputs.push_back(CParameter("member", "", str_2_Addr(extract(data, 0*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_ProposalAdded) {
            // event ProposalAdded(uint256 proposalID, address recipient, uint256 amount, string description)
            // 0x646fec02522b41e7125cfc859a64fd4f4cefd5dc3b6237ca0abe251ded1fa881
            p->func = new CFunction("ProposalAddedEvent");
            p->func->inputs.push_back(CParameter("proposalID", "", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("recipient", "", str_2_Addr(extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("amount", "", str_2_Wei("0x" + extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("description", "", "0x" + extract(data, 3*64, 64)));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_ProposalTallied) {
            // event ProposalTallied(uint256 proposalID, int256 result, uint256 quorum, bool active)
            // 0xd220b7272a8b6d0d7d6bcdace67b936a8f175e6d5c1b3ee438b72256b32ab3af
            p->func = new CFunction("ProposalTalliedEvent");
            p->func->inputs.push_back(CParameter("proposalID", "", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("result", "", str_2_Int("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("quorum", "", str_2_Wei("0x" + extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("active", "", str_2_Int("0x" + extract(data, 3*64, 64))));
            return true;

        } else if (topic_2_Str(p->topics[0]) % evt_Voted) {
            // event Voted(uint256 proposalID, bool position, address voter, string justification)
            // 0xc34f869b7ff431b034b7b9aea9822dac189a685e0b015c7d1be3add3f89128e8
            p->func = new CFunction("VotedEvent");
            p->func->inputs.push_back(CParameter("proposalID", "", str_2_Wei("0x" + extract(data, 0*64, 64))));
            p->func->inputs.push_back(CParameter("position", "", str_2_Int("0x" + extract(data, 1*64, 64))));
            p->func->inputs.push_back(CParameter("voter", "", str_2_Addr(extract(data, 2*64, 64))));
            p->func->inputs.push_back(CParameter("justification", "", "0x" + extract(data, 3*64, 64)));
            return true;

        }
        // fall through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // never returns NULL
    return articulateTokenEvent(p);
}

//-----------------------------------------------------------------------------
string_q sigs[] = {
    // Token support
    func_approve_qb,
    func_transferFrom_qb,
    func_transfer_qb,
    // Wallet support
    func_addOwner_qb,
    func_changeOwner_qb,
    func_changeRequirement_qb,
    func_confirm_qb,
    func_execute_qb,
    func_isOwner_qb,
    func_kill_qb,
    func_removeOwner_qb,
    func_resetSpentToday_qb,
    func_revoke_qb,
    func_setDailyLimit_qb,
    // Contract support
    func_changeMembership,
    func_changeUnicorn,
    func_changeVotingRules,
    func_checkProposalCode,
    func_debatingPeriodInMinutes,
    func_executeProposal,
    func_freezeAccount,
    func_frozenAccount,
    func_majorityMargin,
    func_memberId,
    func_members,
    func_minimumQuorum,
    func_mintToken,
    func_newProposalInEther,
    func_newProposalInWei,
    func_numProposals,
    func_owner,
    func_priceOfAUnicornInFinney,
    func_proposals,
    func_spentAllowance,
    func_transferOwnership,
    func_unicornAddress,
    func_vote,
};
size_t nSigs = sizeof(sigs) / sizeof(string_q);

//-----------------------------------------------------------------------------
string_q topics[] = {
    // Token support
    evt_Transfer_qb,
    evt_Approval_qb,
    // Wallet support
    evt_Confirmation_qb,
    evt_ConfirmationNeeded_qb,
    evt_Deposit_qb,
    evt_MultiTransact_qb,
    evt_OwnerAdded_qb,
    evt_OwnerChanged_qb,
    evt_OwnerRemoved_qb,
    evt_RequirementChanged_qb,
    evt_Revoke_qb,
    evt_SingleTransact_qb,
    // Contract support
    evt_ChangeOfRules,
    evt_FrozenFunds,
    evt_MembershipChanged,
    evt_ProposalAdded,
    evt_ProposalTallied,
    evt_Voted,
};
size_t nTopics = sizeof(topics) / sizeof(string_q);

/*
 ABI for addr : 0xfb6916095ca1df60bb79ce92ce3ea74c37c5d359
[{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"proposals","outputs":[{"name":"recipient","type":"address"},{"name":"amount","type":"uint256"},{"name":"description","type":"string"},{"name":"votingDeadline","type":"uint256"},{"name":"executed","type":"bool"},{"name":"proposalPassed","type":"bool"},{"name":"numberOfVotes","type":"uint256"},{"name":"currentResult","type":"int256"},{"name":"proposalHash","type":"bytes32"}],"type":"function"},{"constant":false,"inputs":[{"name":"proposalNumber","type":"uint256"},{"name":"transactionBytecode","type":"bytes"}],"name":"executeProposal","outputs":[{"name":"result","type":"int256"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"memberId","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"numProposals","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"priceOfAUnicornInFinney","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"newUnicornPriceInFinney","type":"uint256"},{"name":"newUnicornAddress","type":"address"}],"name":"changeUnicorn","outputs":[],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"uint256"}],"name":"members","outputs":[{"name":"member","type":"address"},{"name":"voteWeight","type":"uint256"},{"name":"canAddProposals","type":"bool"},{"name":"name","type":"string"},{"name":"memberSince","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"debatingPeriodInMinutes","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"minimumQuorum","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"targetMember","type":"address"},{"name":"voteWeight","type":"uint256"},{"name":"canAddProposals","type":"bool"},{"name":"memberName","type":"string"}],"name":"changeMembership","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"beneficiary","type":"address"},{"name":"weiAmount","type":"uint256"},{"name":"JobDescription","type":"string"},{"name":"transactionBytecode","type":"bytes"}],"name":"newProposalInWei","outputs":[{"name":"proposalID","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"majorityMargin","outputs":[{"name":"","type":"int256"}],"type":"function"},{"constant":true,"inputs":[],"name":"unicornAddress","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":false,"inputs":[{"name":"beneficiary","type":"address"},{"name":"etherAmount","type":"uint256"},{"name":"JobDescription","type":"string"},{"name":"transactionBytecode","type":"bytes"}],"name":"newProposalInEther","outputs":[{"name":"proposalID","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"minimumQuorumForProposals","type":"uint256"},{"name":"minutesForDebate","type":"uint256"},{"name":"marginOfVotesForMajority","type":"int256"}],"name":"changeVotingRules","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"proposalNumber","type":"uint256"},{"name":"supportsProposal","type":"bool"},{"name":"justificationText","type":"string"}],"name":"vote","outputs":[{"name":"voteID","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[{"name":"proposalNumber","type":"uint256"},{"name":"beneficiary","type":"address"},{"name":"amount","type":"uint256"},{"name":"transactionBytecode","type":"bytes"}],"name":"checkProposalCode","outputs":[{"name":"codeChecksOut","type":"bool"}],"type":"function"},{"constant":false,"inputs":[{"name":"newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"type":"function"},{"inputs":[{"name":"minimumQuorumForProposals","type":"uint256"},{"name":"minutesForDebate","type":"uint256"},{"name":"marginOfVotesForMajority","type":"int256"},{"name":"congressLeader","type":"address"}],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":false,"name":"proposalID","type":"uint256"},{"indexed":false,"name":"recipient","type":"address"},{"indexed":false,"name":"amount","type":"uint256"},{"indexed":false,"name":"description","type":"string"}],"name":"ProposalAdded","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"proposalID","type":"uint256"},{"indexed":false,"name":"position","type":"bool"},{"indexed":false,"name":"voter","type":"address"},{"indexed":false,"name":"justification","type":"string"}],"name":"Voted","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"proposalID","type":"uint256"},{"indexed":false,"name":"result","type":"int256"},{"indexed":false,"name":"quorum","type":"uint256"},{"indexed":false,"name":"active","type":"bool"}],"name":"ProposalTallied","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"member","type":"address"}],"name":"MembershipChanged","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"minimumQuorum","type":"uint256"},{"indexed":false,"name":"debatingPeriodInMinutes","type":"uint256"},{"indexed":false,"name":"majorityMargin","type":"int256"}],"name":"ChangeOfRules","type":"event"}]

ABI for addr : 0x89205a3a3b2a69de6dbf7f01ed13b2108b2c43e7
[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_spender","type":"address"},{"name":"_value","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"_from","type":"address"},{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"balanceOf","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"target","type":"address"},{"name":"mintedAmount","type":"uint256"}],"name":"mintToken","outputs":[],"type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"type":"function"},{"constant":false,"inputs":[{"name":"_to","type":"address"},{"name":"_value","type":"uint256"}],"name":"transfer","outputs":[],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"}],"name":"frozenAccount","outputs":[{"name":"","type":"bool"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"}],"name":"spentAllowance","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":true,"inputs":[{"name":"","type":"address"},{"name":"","type":"address"}],"name":"allowance","outputs":[{"name":"","type":"uint256"}],"type":"function"},{"constant":false,"inputs":[{"name":"target","type":"address"},{"name":"freeze","type":"bool"}],"name":"freezeAccount","outputs":[],"type":"function"},{"constant":false,"inputs":[{"name":"newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"type":"function"},{"inputs":[{"name":"initialSupply","type":"uint256"},{"name":"tokenName","type":"string"},{"name":"decimalUnits","type":"uint8"},{"name":"tokenSymbol","type":"string"},{"name":"centralMinter","type":"address"}],"type":"constructor"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"value","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"target","type":"address"},{"indexed":false,"name":"frozen","type":"bool"}],"name":"FrozenFunds","type":"event"}]

*/
