pragma solidity 0.4.25;

/* ----------------------------------------------------------------------------
// SAFE MATH
// ----------------------------------------------------------------------------*/
contract SafeMath {
	function add(uint a, uint b) internal pure returns (uint c) {
		c = a + b;
		require(c >= a);
	}
	function sub(uint a, uint b) internal pure returns (uint c) {
		require(b <= a);
		c = a - b;
	}
	function mul(uint a, uint b) internal pure returns (uint c) {
		c = a * b;
		require(a == 0 || c / a == b);
	}
	function div(uint a, uint b) internal pure returns (uint c) {
		require(b > 0);
		c = a / b;
	}
}


contract FiatContract {
	function ETH(uint _id) public constant returns (uint);
	function USD(uint _id) public constant returns (uint);
	function EUR(uint _id) public constant returns (uint);
	function GBP(uint _id) public constant returns (uint);
	function updatedAt(uint _id) public constant returns (uint);
}


// ----------------------------------------------------------------------------
// ERC20 TOKEN, with the addition of symbol, name and decimals and an
// initial fixed supply
// ----------------------------------------------------------------------------
contract Whatever is ERC20Interface, SafeMath {
	string public symbol;
	string public name;
	uint8 public decimals;
	bool public pause;
/**/
	// Investor database
	struct Holder {
		uint balance;
		uint affirmDate;
		uint verifyDate;
		uint commitments;
		uint etherPaymentDue;
		string verifyMetaData;
		bool isTrustedVerifier;
	}

	// Request database
	struct Request {
		uint date;
		uint tokenAmount;
		uint dollarPrice;
		uint etherDepositMade;
		address requesterAddress;
		bool hasBeenClosed;
		uint acceptanceDT;
		address acceptorAddress;
		RequestType requestType;
	}

	enum RequestType {
		IsBidRequestToBuy,
		IsAskRequestToSell
	}

	// Contract settings enumeration
	enum Settings {
		SeedAmount,
		DaysStale,
		DaysVerify,
		MinPrice,
		MaxPrice
	}

	// Records for each request
	mapping(uint => Request) requests;
	uint[] requestKeys;
	uint lastRequestKey;

	// Records for each account
	mapping(address => Holder) holders;
	address[] holderAddresses;

	// Admin of account approves the transfer of an amount to another account
	mapping(address => mapping (address => uint)) allowed;

	// ----------------------------------------------------------------------------
	// MODIFIERS
	// ----------------------------------------------------------------------------

	// circuit breaker for the contract
	modifier notPaused() {
		require (!pause);
		_;
	}

	// Functions with this modifier only can be executed by the admin
	modifier onlyAdmin() {
		require (msg.sender == admin);
		_;
	}

	// Basic requirement to participate
	modifier isSeeded(address _account) {
		require (holders[_account].balance > 0);
		_;
	}

	// If not stale, can purchase/receive/redeem
	// If stale, can only redeem
	modifier isNotStale(address _account) {
		require (holders[_account].affirmDate >= sub(now, (_daysStale * 1 days)));
		_;
	}


	// ------------------------------------------------------------------------
	// CONSTRUCTOR
	// ------------------------------------------------------------------------
	constructor() public {
		symbol = "WHT";
		name = "Whatevers";
		decimals = 18;
		pause = false;
		admin = msg.sender;	// initially admin = owner
		owner = msg.sender;	// initially owner = admin
		contractAddress = this;

		totalTokensCommitted	= 0;    // no commitments
		totalEtherPaymentsDue	= 0;    // no payments due
		totalEtherDepositsMade	= 0;    // no payments made

		_totalSupply    = 0;			// variable mint;
		_seedAmount     = 10**14;		// 0.0001 Tokens
		_daysStale      = 370 * 3;		// off-chain use 365 * 3
		_daysVerify     = 370;			// off-chain use 365
		_minPrice		= 0;			// no min price set
		_maxPrice		= 0;			// no max price set

		fiatContract = FiatContract(0x8055d0504666e2B6942BeB8D6014c964658Ca591);     // MainNet
		//fiatContract = FiatContract(0x2CDe56E5c8235D6360CCbb0c57Ce248Ca9C80909);   // Ropsten
	}


	// ------------------------------------------------------------------------
	// PRIMARY FUNCTIONS
	// ------------------------------------------------------------------------

	// New users must be "seeded" before they can do anything
	// Users cannot self-seed.  Only the admin and trustedVerifiers can seed
	// Once seeded, users can re-affirm themselves to update their status
	// To discourage programmatic attack, keep seedAmount < gas price
	function affirmStatusAsAccreditedInvestor(address _account) public notPaused returns (bool success)
	{
		if (msg.sender == _account && holders[_account].balance > 0) {
			holders[_account].affirmDate = now;
			emit AffirmStatus(msg.sender, _account, holders[_account].affirmDate, holders[_account].balance);
			return true;
		}
		require (msg.sender == admin || holders[msg.sender].isTrustedVerifier);
		if (holders[_account].balance == 0) {
			_totalSupply = add(_totalSupply, _seedAmount);
			holders[_account].balance = add(holders[_account].balance, _seedAmount);
			if (holders[_account].affirmDate == 0) {
				holderAddresses.push(_account);
			}
		}
		holders[_account].affirmDate = now;
		emit AffirmStatus(msg.sender, _account, holders[_account].affirmDate, holders[_account].balance);
		return true;
	}

	// For Reg D Rule 506(c).
	// Trusted verifiers can assert that a user is verified (e.g. submits W-2)
	function verifyStatusAsAccreditedInvestor
	(
		address _account, 		string
_verifyMetaData) public
 notPaused
  returns
   (
	   bool
	    success
		)



		 {
		require (holders[msg.sender].isTrustedVerifier);
		if (holders[_account].balance == 0) {
			_totalSupply = add(_totalSupply, _seedAmount);
			holders[_account].balance = add(holders[_account].balance, _seedAmount);
			if (holders[_account].affirmDate == 0) {
				holderAddresses.push(_account);
			}
		}
		holders[_account].affirmDate = now;
		holders[_account].verifyDate = now;
		holders[_account].verifyMetaData = _verifyMetaData;
		emit VerifyStatus(msg.sender, _account, holders[_account].verifyDate, _verifyMetaData);
		return true;
	}

	// Dollar price specified by requestor must be within bounds
	// Or the admin has not specified upper and lower price limits
	function priceOkay(uint _dollarPrice) internal returns (bool) {
		if (_minPrice == 0 && _maxPrice == 0) {
			return true;
		} else if (_maxPrice == 0 && _minPrice <= _dollarPrice) {
			return true;
		} else if (_minPrice == 0 && _maxPrice >= _dollarPrice) {
			return true;
		} else if (_minPrice <= _dollarPrice && _maxPrice >= _dollarPrice) {
			return true;
		} else {
			emit Message("priceOkay", "Price out of bounds");
			return false;
		}
	}

	// Investor (buyer/sender) cannot have stale accreditation (e.g. > 3 years)
	// Receiver (seller/issuer) cannot have stale accreditation (e.g. > 3 years)
	function permitTransfer(address _to)
		isSeeded(_to)
		isNotStale(_to)
		isNotStale(msg.sender)
		internal view returns (bool) {
			return true;
	}

	function sufficientBalance(address _account, uint _amount) internal returns (bool) {
		require (holders[_account].balance >= holders[_account].commitments);
		uint availableTokens = sub(holders[_account].balance, holders[_account].commitments);
		if (availableTokens >= _amount) {
			return true;
		} else {
			emit Message("sufficientBalance", "Amount exceeds availableTokens");
			return false;
		}
	}

	// Transfer with metadata to record notes on the blockchain
	function transferX(address _to, uint _amount, string _transferMetaData) external returns (bool success) {
	    if (transfer(_to, _amount)) {
	        emit TransferX(msg.sender, _to, _amount, _transferMetaData);
	        return true;
	    }
	}

	// Transfer the balance from token sender's account to `to` account
	// - Sender's account must have sufficient balance to transfer
	// - 0 value transfers are allowed
	function transfer(address _to, uint _amount) public notPaused returns (bool success) {
		require (sufficientBalance(msg.sender, _amount));
		if (permitTransfer(_to)) {
			holders[msg.sender].balance = sub(holders[msg.sender].balance, _amount);
			holders[_to].balance = add(holders[_to].balance, _amount);
			emit Transfer(msg.sender, _to, _amount);
			return true;
		} else {
			emit Message("transfer", "Transfer not permitted");
			return false;
		}
	}

	// Token owner can approve for `spender` (like ICO issuer) to transferFrom(...)
	// _amount from the token owner's account
	// https://github.com/ethereum/EIPs/blob/master/EIPS/eip-20-token-standard.md
	// recommends that there are no checks for the approval double-spend attack
	// as this should be implemented in user interfaces
	function approve(address _spender, uint _amount) public notPaused returns (bool success) {
		allowed[msg.sender][_spender] = _amount;
		emit Approval(msg.sender, _spender, _amount);
		return true;
	}

	// Transfer `tokens` from the `from` account to the `to` account
	// The calling account must already have sufficient tokens approve(...)-d
	// for spending from the `from` account and
	// - From account must have sufficient balance to transfer
	// - Spender must have sufficient allowance to transfer
	// - 0 value transfers are allowed
	function transferFrom(address _from, address _to, uint _amount) public notPaused returns (bool success) {
		require (sufficientBalance(_from, _amount));
		if (permitTransfer(_to)) {
			holders[_from].balance = sub(holders[_from].balance, _amount);
			allowed[_from][msg.sender] = sub(allowed[_from][msg.sender], _amount);
			holders[_to].balance = add(holders[_to].balance, _amount);
			emit Transfer(_from, _to, _amount);
			return true;
		} else {
			emit Message("transferFrom", "Transfer not permitted");
			return false;
		}
	}

	// Returns the amount of tokens approved by the owner that can be
	// transferred to the spender's account (like ICO issuer)
	function allowance(address _owner, address _spender) public view returns (uint remaining) {
		return allowed[_owner][_spender];
	}


	// ------------------------------------------------------------------------
	// BID-ASK REQUEST FUNCTIONS
	// ------------------------------------------------------------------------

	function makeNewAskRequestToSELL(uint _tokenAmount, uint _dollarPrice) external notPaused {
		require (priceOkay(_dollarPrice) && sufficientBalance(msg.sender, _tokenAmount));
		holders[msg.sender].commitments = add(holders[msg.sender].commitments, _tokenAmount);
		totalTokensCommitted = add(totalTokensCommitted, _tokenAmount);
		createRequest(_tokenAmount, _dollarPrice, 0, RequestType.IsAskRequestToSell, msg.sender);
	}

	function makeNewBidRequestToBUY(uint _tokenAmount, uint _dollarPrice) external notPaused payable {
		require (priceOkay(_dollarPrice));
		if (!permitTransfer(msg.sender)) {
			emit Message("makeNewBidRequestToBUY", "User not permitted to buy");
			revert();
		}
		uint etherAmount = etherEquivalent(_dollarPrice, _tokenAmount);
		etherAmount = div(mul(etherAmount, 12), 10);
		if (msg.value < etherAmount) {
			emit Message("makeNewBidRequestToBUY", "User sent insufficient ether");
			revert();
		}
		if (!depositPayment()) {
			emit Message("makeNewBidRequestToBUY", "Ether deposit failed");
			revert();
		}
		createRequest(_tokenAmount, _dollarPrice, msg.value, RequestType.IsBidRequestToBuy, msg.sender);
	}

	function createRequest(uint _tokenAmount, uint _dollarPrice, uint _deposit, uint8 _requestType, address _requester) internal {
		lastRequestKey = add(lastRequestKey, 1);
		requestKeys.push(lastRequestKey);
		Request storage newRequest = requests[lastRequestKey];
		newRequest.date = now;
		newRequest.tokenAmount = _tokenAmount;
		newRequest.dollarPrice = _dollarPrice;
		newRequest.etherDepositMade = _deposit;
		totalEtherDepositsMade = add(totalEtherDepositsMade, _deposit);
		newRequest.requestType = _requestType;
		newRequest.requesterAddress = _requester;
		newRequest.hasBeenClosed = false;
		emit MakeRequest(lastRequestKey, newRequest.date, _tokenAmount, _dollarPrice, _requestType, _requester);
	}

	// Payable fallback function to deposit ether into contract
	function depositPayment() public payable returns (bool) {
		return true;
	}

	function etherEquivalent(uint _dollarPrice, uint _tokenAmount) internal view returns (uint) {
		//return div(mul(div(mul(mul(15000000000000, 100), _dollarPrice), 10**18), _tokenAmount), 10**18);
	    return div(mul(div(mul(mul(fiatContract.USD(0), 100), _dollarPrice), 10**18), _tokenAmount), 10**18);
	}

	// Only the requester/originator or admin can cancel a request
	// Any request that already has been accepted cannot be cancelled
	// First mark the request closed
	// Then uncommit any requester's tokens associated with this requester (ASK)
	// Finally return any ether deposit made by the requester to this request (BID)
	function cancelExistingRequest   ( uint  




      _key) external notPaused



 {


		require (isRequestOpen(_key));
		Request storage theRequest = requests[_key];
		address requester = theRequest.requesterAddress;
		if (msg.sender != requester && msg.sender != admin) {
			emit Message("cancelExistingRequest", "Sender is not creator or admin");
			return;
		}
		theRequest.hasBeenClosed = true;
		if (theRequest.requestType == RequestType.IsAskRequestToSell) {
		    releaseCommitment(theRequest.tokenAmount, requester);
		} else {
		    holders[requester].etherPaymentDue = add(holders[requester].etherPaymentDue, theRequest.etherDepositMade);
		    makeDepositDue(theRequest.etherDepositMade);
		}
		emit CancelRequest(_key, msg.sender, theRequest.etherDepositMade, theRequest.tokenAmount);
	}

	// The buyer (msg.sender) must be an accredited investor (seeded and not stale)
	// For each ASK Request in the array:
	// The request must be an open ASK (not already accepted or canceled)
	// Passed parameters also must equal request parameters (amount and price)
	// Ether price is determined by looking up USD exchange rate using FiatContract
	// The buyer (msg.sender) must have sent enough ether for trade value
	// All sent ether (msg.value) is deposited into the contract
	// The ether trade value is ascribed to the ASK requester (seller)
	// Any excess sent ether is ascribed to the buyer (msg.sender)
	// Separately, each party must execute withdrawPayment to collect their ether
	// Both the value and the excess move from msg.value to etherPaymentDue
	// Tokens are transfered from the ASK seller (requester) to the buyer (msg.sender)
	// Finally uncommit the tokens associated with this ASK request
	function acceptTheseAskRequestsAndBUY(uint[] _keys, uint[] _tokenAmounts, uint[] _dollarPrices) external notPaused payable {
		require (_keys.length == _tokenAmounts.length && _keys.length == _dollarPrices.length);
		uint totalValue = 0;
		for (uint i = 0; i < _keys.length; i++) {
			totalValue = add(totalValue, mul(_dollarPrices[i], _tokenAmounts[i]));
		}
		totalValue = div(totalValue, 10**9);
		for (uint j = 0; j < _keys.length; j++) {
			require (isRequestOpen(_keys[j]) && doesRequestMatch(_keys[j], _tokenAmounts[j], _dollarPrices[j], RequestType.IsAskRequestToSell));
			if (!permitTransfer(msg.sender)) {
				emit Message("acceptExistingAskRequestAndBUY", "User not permitted to buy");
				revert();
			}
			Request storage theRequest = requests[_keys[j]];
			address sellerAddress = theRequest.requesterAddress;
			if (holders[sellerAddress].balance < _tokenAmounts[j]) {
				emit Message("acceptExistingAskRequestAndBUY", "Seller has insufficient tokens");
				revert();
			}
			uint etherAmount = etherEquivalent(_dollarPrices[j], _tokenAmounts[j]);
			uint messageValuePortion = div(mul(div(mul(_dollarPrices[j], _tokenAmounts[j]), 10**9), msg.value), totalValue);
			if (etherAmount > messageValuePortion) {
				emit Message("acceptExistingAskRequestAndBUY", "User sent insufficient ether");
				revert();
			}
			if (!depositPayment()) {
				emit Message("acceptExistingAskRequestAndBUY", "Ether deposit failed");
				revert();
			}
			acceptRequest(_keys[j], etherAmount, msg.sender);
			releaseCommitment(_tokenAmounts[j], sellerAddress);
			totalEtherPaymentsDue = add(totalEtherPaymentsDue, messageValuePortion);
			splitEther(etherAmount, messageValuePortion, msg.sender, sellerAddress);
		}
	}

	// The seller (msg.sender) must have sufficient uncommitted tokens to sell
	// For each BID Request in the array:
	// The request must be an open BID (not already accepted or canceled)
	// Passed parameters also must equal request parameters (amount and price)
	// Ether price is determined by looking up USD exchange rate using FiatContract
	// The ether trade value cannot be greater than the ether deposited into the request
	// The ether trade value is conveyed to the seller (msg.sender)
	// Any excess desposited ether is returned to the request/originator
	// Both the value and the excess move from etherDeposited to etherPaymentDue
	// Separately, each party must execute withdrawPayment to collect their ETH
	// Tokens are transfered from the seller (msg.sender) to the BID buyer (requester)
	function acceptTheseBidRequestsAndSELL(uint[] _keys, uint[] _tokenAmounts, uint[] _dollarPrices) external notPaused {
		require (_keys.length == _tokenAmounts.length && _keys.length == _dollarPrices.length);
		for (uint i = 0; i < _keys.length; i++) {
			require (isRequestOpen(_keys[i]) && doesRequestMatch(_keys[i], _tokenAmounts[i], _dollarPrices[i], RequestType.IsBidRequestToBuy) && sufficientBalance(msg.sender, _tokenAmounts[i]));
			Request storage theRequest = requests[_keys[i]];
			address buyerAddress = theRequest.requesterAddress;
			uint etherAmount = etherEquivalent(_dollarPrices[i], _tokenAmounts[i]);
			uint etherDeposited = theRequest.etherDepositMade;
			if (etherDeposited < etherAmount) {
				emit Message("acceptExistingBidRequestAndSELL", "Bidder deposited insufficient ether and should CANCEL bid.");
				revert();
			}
			acceptRequest(_keys[i], etherAmount, msg.sender);
			makeDepositDue(etherDeposited);
			splitEther(etherAmount, etherDeposited, buyerAddress, msg.sender);
		}
	}

	function isRequestOpen(uint _key) internal returns (bool) {
		Request storage theRequest = requests[_key];
		if (theRequest.date == 0 || theRequest.hasBeenClosed) {
			emit Message("isRequestOpen", "Request already accepted or canceled");
			return false;
		}
		return true;
	}

	function doesRequestMatch(uint _key, uint _tokenAmount, uint _dollarPrice, uint8 _requestType) internal returns (bool) {
		Request storage theRequest = requests[_key];
		if (theRequest.tokenAmount != _tokenAmount || theRequest.dollarPrice != _dollarPrice || theRequest.requestType != _requestType) {
			emit Message("doesRequestMatch", "Request amount/price/type mismatch");
			return false;
		}
		return true;
	}

	function releaseCommitment(uint _tokenAmount, address _requester) internal {
		if(holders[_requester].commitments >= _tokenAmount) {
			holders[_requester].commitments = sub(holders[_requester].commitments, _tokenAmount);
		} else {
			holders[_requester].commitments = 0;
			emit Message("releaseCommitment", "Amount exceeds commitment");
		}
		if (totalTokensCommitted >= _tokenAmount) {
			totalTokensCommitted = sub(totalTokensCommitted, _tokenAmount);
		} else {
			totalTokensCommitted = 0;
			emit Message("releaseCommitment", "Amount exceeds totalTokensCommitted");
		}
	}

	function splitEther(uint _payment, uint _deposit, address _buyer, address _seller) internal {
		// This never should happen, but just in case
		if (_payment > _deposit) {
			_payment = _deposit;
			emit Message("splitEther", "Payment exceeds deposit");
		}
		uint excessEther = sub(_deposit, _payment);
		holders[_seller].etherPaymentDue = add(holders[_seller].etherPaymentDue, _payment);
		holders[_buyer].etherPaymentDue = add(holders[_buyer].etherPaymentDue, excessEther);
	}

	function makeDepositDue(uint _deposit) internal {
		totalEtherPaymentsDue = add(totalEtherPaymentsDue, _deposit);
		if (totalEtherDepositsMade >= _deposit) {
			totalEtherDepositsMade = sub(totalEtherDepositsMade, _deposit);
		} else {
			totalEtherDepositsMade = 0;
			emit Message("makeDepositDue", "Deposit exceeds totalEtherDepositsMade");
		}
	}

	function acceptRequest(uint _key, uint _etherAmount, address _acceptor) internal {
		Request storage theRequest = requests[_key];
		theRequest.hasBeenClosed = true;
		theRequest.acceptanceDT = now;
		theRequest.acceptorAddress = _acceptor;
		uint _tokenAmount = theRequest.tokenAmount;
		address _requester = theRequest.requesterAddress;
		if (theRequest.requestType == RequestType.IsBidRequestToBuy) {
			holders[_acceptor].balance = sub(holders[_acceptor].balance, _tokenAmount);
			holders[_requester].balance = add(holders[_requester].balance, _tokenAmount);
		} else {  // IsAskRequestToSell
			holders[_requester].balance = sub(holders[_requester].balance, _tokenAmount);
			holders[_acceptor].balance = add(holders[_acceptor].balance, _tokenAmount);
		}
		emit AcceptRequest(_key, _etherAmount, _tokenAmount, theRequest.dollarPrice, _acceptor);
	}

	// DApp should withdraw ether payment promptly from the contract and convey to sender
	function withdrawPayment() public notPaused returns (uint paymentMade) {
		uint amount = holders[msg.sender].etherPaymentDue;
		holders[msg.sender].etherPaymentDue = 0;
		// These never should happen, but just in case
		if (amount > contractAddress.balance) {
			amount = contractAddress.balance;
			emit Message("withdrawPayment", "Amount exceeds contract balance");
		}
		if (totalEtherPaymentsDue < amount) {
			totalEtherPaymentsDue = amount;
			emit Message("withdrawPayment", "Amount exceeds totalEtherPaymentsDue");
		}
		if (amount > 0) {
			totalEtherPaymentsDue = sub(totalEtherPaymentsDue, amount);
			msg.sender.transfer(amount);
		}
		emit WithdrawPayment(msg.sender, amount);
		return amount;
	}

	// ------------------------------------------------------------------------
	// GET FUNCTIONS
	// ------------------------------------------------------------------------

	function totalSupply() public view returns (uint) {
		return _totalSupply;
	}

	function balanceOf(address tokenOwner) public view returns (uint balance) {
		return holders[tokenOwner].balance;
	}

	function getHolders() public view returns (address[]) {
		return holderAddresses;
	}

	function getHolderInfo(address _address) public view
		returns (uint, uint, uint, uint, string, bool) {
		Holder storage holder = holders[_address];
		return (holder.affirmDate, holder.verifyDate, holder.commitments,
		holder.etherPaymentDue, holder.verifyMetaData, holder.isTrustedVerifier);
	}

	function getRequestKeys() public view returns (uint[]) {
		return requestKeys;
	}

	function getRequest(uint _key) public view
		returns (uint, uint, uint, uint, address, bool, uint, address, uint8) {
		Request storage request = requests[_key];
		return (request.date, request.tokenAmount, request.dollarPrice,
		request.etherDepositMade, request.requesterAddress, request.hasBeenClosed,
		request.acceptanceDT, request.acceptorAddress, request.requestType);
	}

	function contractSetting(uint8 _setting) public view returns (uint) {
		if (_setting == Settings.SeedAmount)    { return _seedAmount; }
		if (_setting == Settings.DaysStale)     { return _daysStale; }
		if (_setting == Settings.DaysVerify)    { return _daysVerify; }
		if (_setting == Settings.MinPrice)		{ return _minPrice; }
		if (_setting == Settings.MaxPrice)		{ return _maxPrice; }
	}


	// ------------------------------------------------------------------------
	// RESTRICTED and ONLY-OWNER FUNCTIONS
	// ------------------------------------------------------------------------

	function mintTokens(uint _amount) external onlyAdmin returns (bool success) {
		_totalSupply = add(_totalSupply, _amount);
		holders[admin].balance = add(holders[admin].balance, _amount);
		emit MintTokens(_amount, holders[admin].balance);
		return true;
	}

	function burnTokens(uint _amount) external onlyAdmin returns (bool success) {
		require (_totalSupply >= _amount && holders[admin].balance >= _amount);
		_totalSupply = sub(_totalSupply, _amount);
		holders[admin].balance = sub(holders[admin].balance, _amount);
		emit BurnTokens(_amount, holders[admin].balance);
		return true;
	}

	// Admin can reclaim any misplaced tokens, or tokens in the hands of a bad actor
	function reclaimMisplacedTokens(address _account) external onlyAdmin returns (bool success) {
		releaseCommitment(holders[_account].commitments, _account);
		uint _amount = holders[_account].balance;
		holders[_account].balance = 0;
		holders[admin].balance = add(holders[admin].balance, _amount);
		emit ReclaimMisplacedTokens(_account, _amount);
		return true;
	}

	function toggleTrustedVerifier(address _verifier) external onlyAdmin returns (bool success) {
		holders[_verifier].isTrustedVerifier = ! holders[_verifier].isTrustedVerifier;
		emit ToggleTrustedVerifier(_verifier, holders[_verifier].isTrustedVerifier);
		return true;
	}

	function changeSetting(uint8 _setting, uint _value) external onlyAdmin returns (bool success) {
		bool _success = false;
		if (_setting == Settings.SeedAmount) {
			if (_value <= 10**16) {                         // 0.01 tokens max
				_seedAmount = _value;
				_success = true;
			}
		} else if (_setting == Settings.DaysStale)  {
			if (_value >= 30) {      						// 30 days min
				_daysStale = _value;
				_success = true;
			}
		} else if (_setting == Settings.DaysVerify) {
			if (_value <= _daysStale && _value >= 30) {      // 30 days min
				_daysVerify = _value;
				_success = true;
			}
		} else if (_setting == Settings.MinPrice) {
			if ((_maxPrice == 0 || _value <= _maxPrice) && _value >= 0) {		// less than or equal to max
				_minPrice = _value;
				_success = true;
			}
		} else if (_setting == Settings.MaxPrice) {
			if ((_minPrice == 0 || _value >= _minPrice) && _value >= 0) {		// greater than or equal to min
				_maxPrice = _value;
				_success = true;
			}
		}
		emit SettingChange(msg.sender, _setting, _value);
		return _success;
	}

	// Owner can update Admin Address
	function setAdminAddress(address _address) external returns (bool success) {
		require (msg.sender == owner);
		admin = _address;
		return true;
	}

	// Admin can update Owner Address
	function setOwnerAddress(address _address) external onlyAdmin returns (bool success) {
		owner = _address;
		return true;
	}

	// Admin can update FiatContract Address
	function setFiatContractAddress(address _address) external onlyAdmin returns (bool success) {
		fiatContract = FiatContract(_address);
		return true;
	}

	// Admin can transfer out any accidentally sent ERC20 tokens
	function transferAnyERC20Token(address _tokenAddress, uint _tokens) external onlyAdmin returns (bool success) {
		require (_tokenAddress != contractAddress);
		return ERC20Interface(_tokenAddress).transfer(admin, _tokens);
	}

	// Admin can transfer out any accidentally sent ether
	function withdrawExcessEther() external onlyAdmin returns (uint amount) {
		if (contractAddress.balance <= add(totalEtherPaymentsDue, totalEtherDepositsMade)) {
			emit Message("withdrawExcessEther", "No excess ether");
			return 0;
		}
		amount = sub(contractAddress.balance, add(totalEtherPaymentsDue, totalEtherDepositsMade));
		msg.sender.transfer(amount);
		emit WithdrawPayment(msg.sender, amount);
		return amount;
	}

	// Admin can pause the contract in case of emergency
	function togglePause() external onlyAdmin {
		pause = !pause;
		emit Message("togglePause", pause ? "paused" : "running");
	}


	// ------------------------------------------------------------------------
	// EVENT DEFINITIONS
	// ------------------------------------------------------------------------

	// Triggered when a new account is seeded.
	event AffirmStatus(address indexed _sender, 
address indexed _account, uint _affirmDate,uint _value
)
;

	// Triggered whenever updateVerifyDate(address _account, string _verifyMetaData) is called.
	event VerifyStatus(address indexed _verifier, address indexed _account, uint _verifyDate, string _verifyMetaData);

	// Triggered when tokens are transferred.
	event Transfer(address indexed _from, address indexed _to, uint _value);

	// Triggered when tokens are transferred with metadata.
	event TransferX(address indexed _from, address indexed _to, uint _value, string _transferMetaData);

	// Triggered whenever approve(address _spender, uint _value) is called.
	event Approval(address indexed _owner, address indexed _spender, uint _value);

	// Triggered when a user creates a new market Bid/Ask request to Buy/Sell.
	event MakeRequest(uint _key, uint _date, uint _tokenAmount, uint _dollarPrice, uint8 _requestType, address indexed _requester);

	// Triggered when a user or admin cancels an existing request created by that user.
	event CancelRequest(uint _key, address indexed _sender, uint _deposit, uint _amount);

	// Triggered when a user accepts a market request and Buys or Sells at the request price.
	event AcceptRequest(uint _key, uint _etherAmount, uint _tokenAmount, uint _dollarPrice, address indexed _acceptor);

	// Triggered when ether is withdrawn from contract.
	event WithdrawPayment(address indexed _account, uint _payment);

	// Triggered whenever tokens are minted to admin account.
	event MintTokens(uint _value, uint _balance);

	// Triggered whenever tokens are burned from admin account.
	event BurnTokens(uint _value, uint _balance);

	// Triggered when lost tokens are reclaimed to admin account.
	event ReclaimMisplacedTokens(address indexed _from, uint _tokenAmount);

	// Triggered whenever a trusted verifier's status is changed.
	event ToggleTrustedVerifier(address indexed _verifier, bool _isTrustedVerifier);

	// Triggered whenever a change is made to a contract parameter.
	event SettingChange(address _admin, uint8 _setting, uint _value);

	// Triggered when certain functions throw an error or other message.
	event Message(string fName, string message);
}
