/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "grabABI.h"

    //-----------------------------------------------------------------------
    voidgrabABI _init(QUITHANDLER qh){
    etherlib_init(qh);    CConstructor::registerClass();
    CApprove::registerClass();
    CChangeAllowedRecipients::registerClass();
    CChangeProposalDeposit::registerClass();
    CCreateTokenProxy::registerClass();
    CExecuteProposal::registerClass();
    CGetMyReward::registerClass();
    CHalveMinQuorum::registerClass();
    CNewContract::registerClass();
    CNewProposal::registerClass();
    CReceiveEther::registerClass();
    CRefund::registerClass();
    CRetrieveDAOReward::registerClass();
    CSplitDAO::registerClass();
    CTransfer::registerClass();
    CTransferFrom::registerClass();
    CTransferFromWithoutReward::registerClass();
    CTransferWithoutReward::registerClass();
    CUnblockMe::registerClass();
    CVote::registerClass();
    CAllowedRecipientChangedEvent::registerClass();
    CApprovalEvent::registerClass();
    CCreatedTokenEvent::registerClass();
    CFuelingToDateEvent::registerClass();
    CNewCuratorEvent::registerClass();
    CProposalAddedEvent::registerClass();
    CProposalTalliedEvent::registerClass();
    CRefundEvent::registerClass();
    CTransferEvent::registerClass();
    CVotedEvent::registerClass();
    tokenlib_init(qh);
}

        //-----------------------------------------------------------------------
        const string_q func_ = "0xd9fdb088";
const string_q func_DAOpaidOut = "0x643f7cdd";
const string_q func_DAOrewardAccount = "0x82bf6464";
const string_q func_actualBalance = "0x39d1f908";
const string_q func_allowance = "0xdd62ed3e";
const string_q func_allowedRecipients = "0x4df6d6cc";
const string_q func_approve = "0x095ea7b3";
const string_q func_balanceOf = "0x70a08231";
const string_q func_blocked = "0xe5962195";
const string_q func_changeAllowedRecipients = "0x749f9889";
const string_q func_changeProposalDeposit = "0xe33734fd";
const string_q func_checkProposalCode = "0xeceb2945";
const string_q func_closingTime = "0x4b6753bc";
const string_q func_createTokenProxy = "0xbaac5300";
const string_q func_curator = "0xe66f53b7";
const string_q func_daoCreator = "0x149acf9a";
const string_q func_divisor = "0x1f2dc5ef";
const string_q func_executeProposal = "0x237e9492";
const string_q func_extraBalance = "0x21b5b8dd";
const string_q func_getMyReward = "0xcc9ae3f6";
const string_q func_getNewDAOAddress = "0xbe7c29c1";
const string_q func_halveMinQuorum = "0x78524b2e";
const string_q func_isFueled = "0xb7bc2c84";
const string_q func_lastTimeMinQuorumMet = "0x96d7f3f5";
const string_q func_minQuorumDivisor = "0x674ed066";
const string_q func_minTokensToCreate = "0x0c3b7b96";
const string_q func_newContract = "0x6837ff1e";
const string_q func_newProposal = "0x612e45a3";
const string_q func_numberOfProposals = "0x8d7af473";
const string_q func_paidOut = "0x81f03fcb";
const string_q func_privateCreation = "0xf8c80d26";
const string_q func_proposalDeposit = "0x8b15a605";
const string_q func_proposals = "0x013cf08b";
const string_q func_receiveEther = "0xa3912ec8";
const string_q func_refund = "0x590e1ae3";
const string_q func_retrieveDAOReward = "0xa1da2fb9";
const string_q func_rewardAccount = "0x0e708203";
const string_q func_rewardToken = "0xcdef91d0";
const string_q func_splitDAO = "0x82661dc4";
const string_q func_totalRewardToken = "0x34145808";
const string_q func_totalSupply = "0x18160ddd";
const string_q func_transfer = "0xa9059cbb";
const string_q func_transferFrom = "0x23b872dd";
const string_q func_transferFromWithoutReward = "0xdbde1988";
const string_q func_transferWithoutReward = "0x4e10c3ee";
const string_q func_unblockMe = "0x2632bf20";
const string_q func_vote = "0xc9d27afe";

    //-----------------------------------------------------------------------
    const CTransaction* promoteToFunc(const CTransaction* p) {
    if (p && (p->input.length() >= 10 || p->input == "0x")) {
                CStringArray types;

        string_q encoding = extract(p->input, 0, 10);
        string_q params   = extract(p->input, 10);
  // EXISTING_CODE
                       // EXISTING_CODE

                if (encoding == func_) {
            // constructor(address _curator, address _daoCreator, uint256 _proposalDeposit, uint256 _minTokensToCreate, uint256 _closingTime, address _privateCreation)
            // 0xd9fdb088
            CConstructor *a = new CConstructor;
            a->CTransaction::operator=(*p);
            a->_curator = str_2_Addr(extract(params, 0*64, 64));
            a->_daoCreator = str_2_Addr(extract(params, 1*64, 64));
            a->_proposalDeposit = str_2_Wei("0x" + extract(params, 2*64, 64));
            a->_minTokensToCreate = str_2_Wei("0x" + extract(params, 3*64, 64));
            a->_closingTime = str_2_Wei("0x" + extract(params, 4*64, 64));
            a->_privateCreation = str_2_Addr(extract(params, 5*64, 64));
            types.push_back("address");
            types.push_back("address");
            types.push_back("uint256");
            types.push_back("uint256");
            types.push_back("uint256");
            types.push_back("address");
            return a;

        } else if (encoding == func_approve) {
            // function approve(address _spender, uint256 _amount)
            // 0x095ea7b3
            CApprove *a = new CApprove;
            a->CTransaction::operator=(*p);
            a->_spender = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (encoding == func_changeAllowedRecipients) {
            // function changeAllowedRecipients(address _recipient, bool _allowed)
            // 0x749f9889
            CChangeAllowedRecipients *a = new CChangeAllowedRecipients;
            a->CTransaction::operator=(*p);
            a->_recipient = str_2_Addr(extract(params, 0*64, 64));
            a->_allowed = str_2_Int(extract(params, 1*64, 64));
            types.push_back("address");
            types.push_back("bool");
            return a;

        } else if (encoding == func_changeProposalDeposit) {
            // function changeProposalDeposit(uint256 _proposalDeposit)
            // 0xe33734fd
            CChangeProposalDeposit *a = new CChangeProposalDeposit;
            a->CTransaction::operator=(*p);
            a->_proposalDeposit = str_2_Wei("0x" + extract(params, 0*64, 64));
            types.push_back("uint256");
            return a;

        } else if (encoding == func_createTokenProxy) {
            // function createTokenProxy(address _tokenHolder)
            // 0xbaac5300
            CCreateTokenProxy *a = new CCreateTokenProxy;
            a->CTransaction::operator=(*p);
            a->_tokenHolder = str_2_Addr(extract(params, 0*64, 64));
            types.push_back("address");
            return a;

        } else if (encoding == func_executeProposal) {
            // function executeProposal(uint256 _proposalID, bytes _transactionData)
            // 0x237e9492
            CExecuteProposal *a = new CExecuteProposal;
            a->CTransaction::operator=(*p);
            a->_proposalID = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_transactionData = extract(params, 1*64);
            types.push_back("uint256");
            types.push_back("bytes");
            return a;

        } else if (encoding == func_getMyReward) {
            // function getMyReward()
            // 0xcc9ae3f6
            CGetMyReward *a = new CGetMyReward;
            a->CTransaction::operator=(*p);
            return a;

        } else if (encoding == func_halveMinQuorum) {
            // function halveMinQuorum()
            // 0x78524b2e
            CHalveMinQuorum *a = new CHalveMinQuorum;
            a->CTransaction::operator=(*p);
            return a;

        } else if (encoding == func_newContract) {
            // function newContract(address _newContract)
            // 0x6837ff1e
            CNewContract *a = new CNewContract;
            a->CTransaction::operator=(*p);
            a->_newContract = str_2_Addr(extract(params, 0*64, 64));
            types.push_back("address");
            return a;

        } else if (encoding == func_newProposal) {
            // function newProposal(address _recipient, uint256 _amount, string _description, bytes _transactionData, uint256 _debatingPeriod, bool _newCurator)
            // 0x612e45a3
            CNewProposal *a = new CNewProposal;
            a->CTransaction::operator=(*p);
            a->_recipient = str_2_Addr(extract(params, 0*64, 64));
            a->_amount = str_2_Wei("0x" + extract(params, 1*64, 64));
            a->_description = extract(params, 2*64, 64);
            a->_transactionData = extract(params, 3*64);
            a->_debatingPeriod = str_2_Wei("0x" + extract(params, 4*64, 64));
            a->_newCurator = str_2_Int(extract(params, 5*64, 64));
            types.push_back("address");
            types.push_back("uint256");
            types.push_back("string");
            types.push_back("bytes");
            types.push_back("uint256");
            types.push_back("bool");
            return a;

        } else if (encoding == func_receiveEther) {
            // function receiveEther()
            // 0xa3912ec8
            CReceiveEther *a = new CReceiveEther;
            a->CTransaction::operator=(*p);
            return a;

        } else if (encoding == func_refund) {
            // function refund()
            // 0x590e1ae3
            CRefund *a = new CRefund;
            a->CTransaction::operator=(*p);
            return a;

        } else if (encoding == func_retrieveDAOReward) {
            // function retrieveDAOReward(bool _toMembers)
            // 0xa1da2fb9
            CRetrieveDAOReward *a = new CRetrieveDAOReward;
            a->CTransaction::operator=(*p);
            a->_toMembers = str_2_Int(extract(params, 0*64, 64));
            types.push_back("bool");
            return a;

        } else if (encoding == func_splitDAO) {
            // function splitDAO(uint256 _proposalID, address _newCurator)
            // 0x82661dc4
            CSplitDAO *a = new CSplitDAO;
            a->CTransaction::operator=(*p);
            a->_proposalID = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_newCurator = str_2_Addr(extract(params, 1*64, 64));
            types.push_back("uint256");
            types.push_back("address");
            return a;

        } else if (encoding == func_transfer) {
            // function transfer(address _to, uint256 _value)
            // 0xa9059cbb
            CTransfer *a = new CTransfer;
            a->CTransaction::operator=(*p);
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 1*64, 64));
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (encoding == func_transferFrom) {
            // function transferFrom(address _from, address _to, uint256 _value)
            // 0x23b872dd
            CTransferFrom *a = new CTransferFrom;
            a->CTransaction::operator=(*p);
            a->_from = str_2_Addr(extract(params, 0*64, 64));
            a->_to = str_2_Addr(extract(params, 1*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 2*64, 64));
            types.push_back("address");
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (encoding == func_transferFromWithoutReward) {
            // function transferFromWithoutReward(address _from, address _to, uint256 _value)
            // 0xdbde1988
            CTransferFromWithoutReward *a = new CTransferFromWithoutReward;
            a->CTransaction::operator=(*p);
            a->_from = str_2_Addr(extract(params, 0*64, 64));
            a->_to = str_2_Addr(extract(params, 1*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 2*64, 64));
            types.push_back("address");
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (encoding == func_transferWithoutReward) {
            // function transferWithoutReward(address _to, uint256 _value)
            // 0x4e10c3ee
            CTransferWithoutReward *a = new CTransferWithoutReward;
            a->CTransaction::operator=(*p);
            a->_to = str_2_Addr(extract(params, 0*64, 64));
            a->_value = str_2_Wei("0x" + extract(params, 1*64, 64));
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (encoding == func_unblockMe) {
            // function unblockMe()
            // 0x2632bf20
            CUnblockMe *a = new CUnblockMe;
            a->CTransaction::operator=(*p);
            return a;

        } else if (encoding == func_vote) {
            // function vote(uint256 _proposalID, bool _supportsProposal)
            // 0xc9d27afe
            CVote *a = new CVote;
            a->CTransaction::operator=(*p);
            a->_proposalID = str_2_Wei("0x" + extract(params, 0*64, 64));
            a->_supportsProposal = str_2_Int(extract(params, 1*64, 64));
            types.push_back("uint256");
            types.push_back("bool");
            return a;

    }
    // falls through
}

// EXISTING_CODE
                       
// EXISTING_CODE

// never returns NULL
return promoteToToken(p);
}

//-----------------------------------------------------------------------
const string_q evt_AllowedRecipientChanged = "0x73ad2a153c8b67991df9459024950b318a609782cee8c7eeda47b905f9baa91f";
const string_q evt_Approval = "0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925";
const string_q evt_CreatedToken = "0xdbccb92686efceafb9bb7e0394df7f58f71b954061b81afb57109bf247d3d75a";
const string_q evt_FuelingToDate = "0xf381a3e2428fdda36615919e8d9c35878d9eb0cf85ac6edf575088e80e4c147e";
const string_q evt_NewCurator = "0x9046fefd66f538ab35263248a44217dcb70e2eb2cd136629e141b8b8f9f03b60";
const string_q evt_ProposalAdded = "0x5790de2c279e58269b93b12828f56fd5f2bc8ad15e61ce08572585c81a38756f";
const string_q evt_ProposalTallied = "0xdfc78bdca8e3e0b18c16c5c99323c6cb9eb5e00afde190b4e7273f5158702b07";
const string_q evt_Refund = "0xbb28353e4598c3b9199101a66e0989549b659a59a54d2c27fbb183f1932c8e6d";
const string_q evt_Transfer = "0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef";
const string_q evt_Voted = "0x86abfce99b7dd908bec0169288797f85049ec73cbe046ed9de818fab3a497ae0";

    //-----------------------------------------------------------------------
    const CLogEntry* promoteToFunc Event(const CLogEntry* p) {
    size_t nTops = p->topics.size();
    if (nTops > 0) {    // the '0'th topic is the event signature
                CStringArray types;
        string_q data = extract(p->data, 2);
        string_q params;
        bool first = true;
        for (auto t : p->topics) {
            if (!first)
                params += extract(topic_2_Str(t),2);
            first = false;
        }
        params += data;
  // EXISTING_CODE
                        // EXISTING_CODE
                if (topic_2_Str(p->topics[0]) % evt_AllowedRecipientChanged) {
            // event AllowedRecipientChanged(address indexed _recipient, bool _allowed)
            // 0x73ad2a153c8b67991df9459024950b318a609782cee8c7eeda47b905f9baa91f
            CAllowedRecipientChangedEvent *a = new CAllowedRecipientChangedEvent;
            a->CLogEntry::operator=(*p);
            a->_recipient = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_allowed = str_2_Int("0x" + extract(data, 0*64, 64));
            types.push_back("address");
            types.push_back("bool");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Approval) {
            // event Approval(address indexed _owner, address indexed _spender, uint256 _amount)
            // 0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925
            CApprovalEvent *a = new CApprovalEvent;
            a->CLogEntry::operator=(*p);
            a->_owner = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_spender = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            types.push_back("address");
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_CreatedToken) {
            // event CreatedToken(address indexed to, uint256 amount)
            // 0xdbccb92686efceafb9bb7e0394df7f58f71b954061b81afb57109bf247d3d75a
            CCreatedTokenEvent *a = new CCreatedTokenEvent;
            a->CLogEntry::operator=(*p);
            a->to = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_FuelingToDate) {
            // event FuelingToDate(uint256 value)
            // 0xf381a3e2428fdda36615919e8d9c35878d9eb0cf85ac6edf575088e80e4c147e
            CFuelingToDateEvent *a = new CFuelingToDateEvent;
            a->CLogEntry::operator=(*p);
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_NewCurator) {
            // event NewCurator(address indexed _newCurator)
            // 0x9046fefd66f538ab35263248a44217dcb70e2eb2cd136629e141b8b8f9f03b60
            CNewCuratorEvent *a = new CNewCuratorEvent;
            a->CLogEntry::operator=(*p);
            a->_newCurator = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            types.push_back("address");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ProposalAdded) {
            // event ProposalAdded(uint256 indexed proposalID, address recipient, uint256 amount, bool newCurator, string description)
            // 0x5790de2c279e58269b93b12828f56fd5f2bc8ad15e61ce08572585c81a38756f
            CProposalAddedEvent *a = new CProposalAddedEvent;
            a->CLogEntry::operator=(*p);
            a->proposalID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->recipient = str_2_Addr(extract(data, 0*64, 64));
            a->amount = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->newCurator = str_2_Int("0x" + extract(data, 2*64, 64));
            a->description = "0x" + extract(data, 3*64, 64);
            types.push_back("uint256");
            types.push_back("address");
            types.push_back("uint256");
            types.push_back("bool");
            types.push_back("string");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ProposalTallied) {
            // event ProposalTallied(uint256 indexed proposalID, bool result, uint256 quorum)
            // 0xdfc78bdca8e3e0b18c16c5c99323c6cb9eb5e00afde190b4e7273f5158702b07
            CProposalTalliedEvent *a = new CProposalTalliedEvent;
            a->CLogEntry::operator=(*p);
            a->proposalID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->result = str_2_Int("0x" + extract(data, 0*64, 64));
            a->quorum = str_2_Wei("0x" + extract(data, 1*64, 64));
            types.push_back("uint256");
            types.push_back("bool");
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Refund) {
            // event Refund(address indexed to, uint256 value)
            // 0xbb28353e4598c3b9199101a66e0989549b659a59a54d2c27fbb183f1932c8e6d
            CRefundEvent *a = new CRefundEvent;
            a->CLogEntry::operator=(*p);
            a->to = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Transfer) {
            // event Transfer(address indexed _from, address indexed _to, uint256 _amount)
            // 0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef
            CTransferEvent *a = new CTransferEvent;
            a->CLogEntry::operator=(*p);
            a->_from = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_to = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            types.push_back("address");
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Voted) {
            // event Voted(uint256 indexed proposalID, bool position, address indexed voter)
            // 0x86abfce99b7dd908bec0169288797f85049ec73cbe046ed9de818fab3a497ae0
            CVotedEvent *a = new CVotedEvent;
            a->CLogEntry::operator=(*p);
            a->proposalID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->position = str_2_Int("0x" + extract(data, 0*64, 64));
            a->voter = str_2_Addr(nTops > 3 ? topic_2_Str(p->topics[3]) : "");
            types.push_back("uint256");
            types.push_back("bool");
            types.push_back("address");
            return a;

    }
    // fall through
}

// EXISTING_CODE
                       
// EXISTING_CODE

// never returns NULL
return promoteToTokenEvent(p);
}
// EXISTING_CODE
<code>
                if (topic_2_Str(p->topics[0]) % evt_AllowedRecipientChanged) {
            // event AllowedRecipientChanged(address indexed _recipient, bool _allowed)
            // 0x73ad2a153c8b67991df9459024950b318a609782cee8c7eeda47b905f9baa91f
            CAllowedRecipientChangedEvent *a = new CAllowedRecipientChangedEvent;
            a->CLogEntry::operator=(*p);
            a->_recipient = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_allowed = str_2_Int("0x" + extract(data, 0*64, 64));
            types.push_back("address");
            types.push_back("bool");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Approval) {
            // event Approval(address indexed _owner, address indexed _spender, uint256 _amount)
            // 0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925
            CApprovalEvent *a = new CApprovalEvent;
            a->CLogEntry::operator=(*p);
            a->_owner = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_spender = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            types.push_back("address");
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_CreatedToken) {
            // event CreatedToken(address indexed to, uint256 amount)
            // 0xdbccb92686efceafb9bb7e0394df7f58f71b954061b81afb57109bf247d3d75a
            CCreatedTokenEvent *a = new CCreatedTokenEvent;
            a->CLogEntry::operator=(*p);
            a->to = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_FuelingToDate) {
            // event FuelingToDate(uint256 value)
            // 0xf381a3e2428fdda36615919e8d9c35878d9eb0cf85ac6edf575088e80e4c147e
            CFuelingToDateEvent *a = new CFuelingToDateEvent;
            a->CLogEntry::operator=(*p);
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_NewCurator) {
            // event NewCurator(address indexed _newCurator)
            // 0x9046fefd66f538ab35263248a44217dcb70e2eb2cd136629e141b8b8f9f03b60
            CNewCuratorEvent *a = new CNewCuratorEvent;
            a->CLogEntry::operator=(*p);
            a->_newCurator = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            types.push_back("address");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ProposalAdded) {
            // event ProposalAdded(uint256 indexed proposalID, address recipient, uint256 amount, bool newCurator, string description)
            // 0x5790de2c279e58269b93b12828f56fd5f2bc8ad15e61ce08572585c81a38756f
            CProposalAddedEvent *a = new CProposalAddedEvent;
            a->CLogEntry::operator=(*p);
            a->proposalID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->recipient = str_2_Addr(extract(data, 0*64, 64));
            a->amount = str_2_Wei("0x" + extract(data, 1*64, 64));
            a->newCurator = str_2_Int("0x" + extract(data, 2*64, 64));
            a->description = "0x" + extract(data, 3*64, 64);
            types.push_back("uint256");
            types.push_back("address");
            types.push_back("uint256");
            types.push_back("bool");
            types.push_back("string");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_ProposalTallied) {
            // event ProposalTallied(uint256 indexed proposalID, bool result, uint256 quorum)
            // 0xdfc78bdca8e3e0b18c16c5c99323c6cb9eb5e00afde190b4e7273f5158702b07
            CProposalTalliedEvent *a = new CProposalTalliedEvent;
            a->CLogEntry::operator=(*p);
            a->proposalID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->result = str_2_Int("0x" + extract(data, 0*64, 64));
            a->quorum = str_2_Wei("0x" + extract(data, 1*64, 64));
            types.push_back("uint256");
            types.push_back("bool");
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Refund) {
            // event Refund(address indexed to, uint256 value)
            // 0xbb28353e4598c3b9199101a66e0989549b659a59a54d2c27fbb183f1932c8e6d
            CRefundEvent *a = new CRefundEvent;
            a->CLogEntry::operator=(*p);
            a->to = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->value = str_2_Wei("0x" + extract(data, 0*64, 64));
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Transfer) {
            // event Transfer(address indexed _from, address indexed _to, uint256 _amount)
            // 0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef
            CTransferEvent *a = new CTransferEvent;
            a->CLogEntry::operator=(*p);
            a->_from = str_2_Addr(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->_to = str_2_Addr(nTops > 2 ? topic_2_Str(p->topics[2]) : "");
            a->_amount = str_2_Wei("0x" + extract(data, 0*64, 64));
            types.push_back("address");
            types.push_back("address");
            types.push_back("uint256");
            return a;

        } else if (topic_2_Str(p->topics[0]) % evt_Voted) {
            // event Voted(uint256 indexed proposalID, bool position, address indexed voter)
            // 0x86abfce99b7dd908bec0169288797f85049ec73cbe046ed9de818fab3a497ae0
            CVotedEvent *a = new CVotedEvent;
            a->CLogEntry::operator=(*p);
            a->proposalID = str_2_Wei(nTops > 1 ? topic_2_Str(p->topics[1]) : "");
            a->position = str_2_Int("0x" + extract(data, 0*64, 64));
            a->voter = str_2_Addr(nTops > 3 ? topic_2_Str(p->topics[3]) : "");
            types.push_back("uint256");
            types.push_back("bool");
            types.push_back("address");
            return a;

    }
    // fall through
}
// EXISTING_CODE

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
    func_,
    func_DAOpaidOut,
    func_DAOrewardAccount,
    func_actualBalance,
    func_allowance,
    func_allowedRecipients,
    func_approve,
    func_balanceOf,
    func_blocked,
    func_changeAllowedRecipients,
    func_changeProposalDeposit,
    func_checkProposalCode,
    func_closingTime,
    func_createTokenProxy,
    func_curator,
    func_daoCreator,
    func_divisor,
    func_executeProposal,
    func_extraBalance,
    func_getMyReward,
    func_getNewDAOAddress,
    func_halveMinQuorum,
    func_isFueled,
    func_lastTimeMinQuorumMet,
    func_minQuorumDivisor,
    func_minTokensToCreate,
    func_newContract,
    func_newProposal,
    func_numberOfProposals,
    func_paidOut,
    func_privateCreation,
    func_proposalDeposit,
    func_proposals,
    func_receiveEther,
    func_refund,
    func_retrieveDAOReward,
    func_rewardAccount,
    func_rewardToken,
    func_splitDAO,
    func_totalRewardToken,
    func_totalSupply,
    func_transfer,
    func_transferFrom,
    func_transferFromWithoutReward,
    func_transferWithoutReward,
    func_unblockMe,
    func_vote,
};
size_t nSigs = sizeof(sigs) / sizeof(string_q);

//-----------------------------------------------------------------------------
const string_q topics[] = {
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
    evt_AllowedRecipientChanged,
    evt_Approval,
    evt_CreatedToken,
    evt_FuelingToDate,
    evt_NewCurator,
    evt_ProposalAdded,
    evt_ProposalTallied,
    evt_Refund,
    evt_Transfer,
    evt_Voted,
};
const size_t nTopics = sizeof(topics) / sizeof(string_q);

    /*
     */