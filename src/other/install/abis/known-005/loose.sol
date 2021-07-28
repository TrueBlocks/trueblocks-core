contract A {
  function storeBlockWithFeeAndRecipient(bytes,int256,int256) { }
  function donate((address,uint256,address)[] dontations) { }
  function mint(address recipient, uint256 amount) { }
  function changeMembership(address member,uint256 amt,bool,string name) { }
  function doit() { }
  function register(bytes32 label, string subdomain, address subdomainOwner, address resolver, address referrer) { }
  function request() { }
  function batchTransfer(address _tokenAddr, address[] _holders ,uint256[] _amts) { }
  function killBounty(uint256 _bountyID) { }
  function MultiTransfer(address _tokenAddr, address[] _holders, uint256[] _amts) { }
  function multisend(address[] _holders, uint256[] _amts) { }
  function registerUsers(address[] users) { }
  function multimint(address[] _holders, uint256[] _amt) { }
  function create(address[] _owners, uint256 _required) { }
  function claim() { }
  function setCompleted(uint256 completed) { }
  function submitProposal(address applicant, uint256 sharesRequested, uint256 lootRequested, uint256 tributeOffered, address tributeToken, uint256 paymentRequested, address paymentToken, string details) { }
  function sponsorProposal(uint256 proposalId) { }
  function submitVote(uint256 proposalIndex, uint8 uintVote) { }
  function executeSubscription(address from, address to, address tokenAddress, uint256 tokenAmount, uint256 periodSeconds, uint256 gasPrice, uint256 nonce, bytes signature) { }
  function fulfillBounty(uint256 _bountyID, string _dataIPFSHash, address _standardBounties, uint256 _standardBountiesVersion, address[] _members, bytes[] _signatures, uint256[] _salts) { }
  function deploy(string,uint256,uint256,uint256) { }
  function clone(address _to, uint256 _tokenId, uint256 _numClonesRequested) { }
  function pay(address src, uint256 srcAmount, address dest, address destAddress, uint256 maxDestAmount, uint256 minConversionRate, address walletId, bytes paymentData, bytes hint, address kyberNetworkProxy) { }
  function presentto( ) { }
  function present(address[] nanee) { }
  function airDrop(address parent, uint256[] amounts, address[] droptargets) { }
  function splitTransfer(address toFirst,address toSecond,uint256 valueFirst,uint256 valueSecond,address tokenAddress) external { }
  function migrateAll(uint256[] tokenIds) { }
  function sendTokensArray(address[] beneficiaries, uint256 amount) { }
  function buy(uint256 id, uint256 amount) { }
  function setAddr(bytes32 node, address addr) { }
  function execute(bytes val_0) { }
  function withdrawToken(address token, uint256 amount) { }
  function trade(address tokenGet, uint256 amountGet, address tokenGive, uint256 amountGive, uint256 expires, uint256 nonce, address user, uint8 v, bytes32 r, bytes32 s, uint256 amount) { }
  function cancelAuction(uint256 val_0) { }
  function bid(uint256 val_0) { } 
  function mintBatchFrozenTokens(address[] accounts, uint256[] amounts) { }
  function execute(address _target, bytes _data)  { } 
  function multiGenerate(address _merkleMineContract, address[] _recipients, bytes _merkleProofs) { } 
  function build() { } 
  function airdropTokens(address[] _recipients, uint256[] _amounts) { } 
  function batchSend(address _token, uint160 _lotSize, uint256[] _transfers) { } 
  function mintToAddresses(address[] addresses, uint256 amount) { } 
  function split(address targetFork, address targetNoFork) { } 
  function massNotify(address[] _owners) { } 
  function transferPresaleTokens(address destination, uint256 amount) { } 
  function drop(address token, address[] recipients, uint256[] values) { } 
  function send(address[] receivers, uint256[] values) { } 
  function multisend(address _tokenAddr, address[] dests, uint256[] values) { } 
  function mintUserToManyEvents(uint256[] eventIds, address to) { } 
  function mintEventToManyUsers(uint256 eventId, address[] to) { } 
  function mintToken(uint256 _id, address _owner) { } 
  function upgrade(address _teamAddress, address _artistAddress) { } 
  function execute(address _wallet, bytes _data, uint256 _nonce, bytes _signatures, uint256 _gasPrice, uint256 _gasLimit) { } 
  function vote(uint256 videoID) { } 
  function execTransaction(address to, uint256 value, bytes data, uint8 operation, uint256 safeTxGas, uint256 dataGas, uint256 gasPrice, address gasToken, address refundReceiver, bytes signatures) { } 
  function singleValueAirdrop(address _addressOfToken, address[] _recipients, uint256 _value) { } 
  function multisendToken(address token, address[] _contributors, uint256[] _balances) { } 
  function proxyAssert(address dest, uint8 howToCall, bytes calldata) { } 
  function addAdmin(address _new_admin) { } 
  function renounceAdmin() { } 
  function invoke1CosignerSends(uint8 v, bytes32 r, bytes32 s, uint256 nonce, address authorizedAddress, bytes data) { } 
  function cancelOrder_(address[7] addrs, uint256[9] uints, uint8 feeMethod, uint8 side, uint8 saleKind, uint8 howToCall, bytes calldata, bytes replacementPattern, bytes staticExtradata, uint8 v, bytes32 r, bytes32 s) { } 
  function atomicMatch_(address[14] addrs, uint256[18] uints, uint8[8] feeMethodsSidesKindsHowToCalls, bytes calldataBuy, bytes calldataSell, bytes replacementPatternBuy, bytes replacementPatternSell, bytes staticExtradataBuy, bytes staticExtradataSell, uint8[2] vs, bytes32[5] rssMetadata) { } 
  function register(uint256 _maxGasPrice, uint256 _maxFeePerClaim) { }
}
