contract DAODutchAuction {

    event BidSubmission(address indexed investor, uint256 amount);

    Token public daoToken;

    address public tokenWallet;
    address public etherWallet;
    address public owner;
    uint public startBlock;
    uint public endTime;
    uint public totalRaised;
    uint public finalPrice;
    mapping (address => uint) public bids;
    Stages public stage = Stages.AuctionStarted;

    enum Stages {
        AuctionStarted,
        AuctionEnded
    }

    uint constant public WAITING_PERIOD = 7 days;
    uint constant public FUNDING_GOAL = 1250000 ether;
    uint constant public TOTAL_TOKENS = 10000000; // 10M
    uint constant public MAX_TOKENS_SOLD = 9000000; // 9M

    function bid() public payable
    {
        // timedTransitions
        if (stage == Stages.AuctionStarted && calcTokenPrice() <= calcStopPrice()) {
            finalizeAuction();
        // atStage
        if (stage != Stages.AuctionStarted) {
            throw;

        uint investment = msg.value;
        if (totalRaised + investment > FUNDING_GOAL) {
            investment = FUNDING_GOAL - totalRaised;
            if (!msg.sender.send(msg.value - investment)) {
                throw;
            }
        }
        if (investment == 0 || !etherWallet.send(investment)) {
            throw;
        }
        bids[msg.sender] += investment;
        totalRaised += investment;
        if (totalRaised == FUNDING_GOAL) {
            finalizeAuction();
        }
        BidSubmission(msg.sender, investment);
    }

    function finalizeAuction() private
    {
        stage = Stages.AuctionEnded;
        if (totalRaised == FUNDING_GOAL) {
            finalPrice = calcTokenPrice();
        }
        else {
            finalPrice = calcStopPrice();
        }
        uint soldTokens = totalRaised * 10**18 / finalPrice;
        daoToken.transfer(tokenWallet, TOTAL_TOKENS * 10**18 - soldTokens);
        endTime = block.timestamp;
    }

    function claimTokens() public
    {
        // timedTransitions
        if (stage == Stages.AuctionStarted && calcTokenPrice() <= calcStopPrice()) {
            finalizeAuction();
        // atStage
        if (stage != Stages.AuctionEnded) {
            throw;
        uint tokenCount = bids[msg.sender] * 10**18 / finalPrice;
        bids[msg.sender] = 0;
        daoToken.transfer(msg.sender, tokenCount);
    }

    function setup(address _daoToken, address _tokenWallet, address _etherWallet) external
    {
        // isOwner
        if (msg.sender != owner)
            throw;
        if (tokenWallet != 0 || etherWallet != 0 || address(daoToken) != 0) {
            throw;
        }
        tokenWallet = _tokenWallet;
        etherWallet = _etherWallet;
        daoToken = Token(_daoToken);
    }

    function DAODutchAuction() {
        startBlock = block.number;
        owner = msg.sender;
    }

    function calcStopPrice() constant public returns (uint stopPrice)
    {
        return totalRaised / MAX_TOKENS_SOLD;
    }

    function calcTokenPrice() constant public returns (uint tokenPrice)
    {
        return 20000 * 1 ether / (block.number - startBlock + 1);
    }

    function tokenLaunched() external returns (bool launched)
    {
        // timedTransitions
        if (stage == Stages.AuctionStarted && calcTokenPrice() <= calcStopPrice()) {
            finalizeAuction();
        return endTime + WAITING_PERIOD < block.timestamp;
    }

    function updateStage() external returns (Stages _stage)
    {
        // timedTransitions
        if (stage == Stages.AuctionStarted && calcTokenPrice() <= calcStopPrice()) {
            finalizeAuction();
        return stage;
    }

    function calcCurrentTokenPrice() external returns (uint tokenPrice)
    {
        // timedTransitions
        if (stage == Stages.AuctionStarted && calcTokenPrice() <= calcStopPrice()) {
            finalizeAuction();
        if (stage == Stages.AuctionEnded) {
            return finalPrice;
        }
        return calcTokenPrice();
    }
}
