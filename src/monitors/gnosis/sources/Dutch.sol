pragma solidity 0.4.10;

contract Token {
    function transfer(address to, uint256 value) returns (bool success);
    function transferFrom(address from, address to, uint256 value) returns (bool success);
    function approve(address spender, uint256 value) returns (bool success);

    // This is not an abstract function, because solc won't recognize generated getter functions for public variables as functions.
    function totalSupply() constant returns (uint256 supply) {}
    function balanceOf(address owner) constant returns (uint256 balance);
    function allowance(address owner, address spender) constant returns (uint256 remaining);

    event Transfer(address indexed from, address indexed to, uint256 value);
    event Approval(address indexed owner, address indexed spender, uint256 value);
}


/// @title Dutch auction contract - distribution of Gnosis tokens using an auction.
/// @author Stefan George - <stefan.george@consensys.net>
contract DutchAuction {

    event BidSubmission(address indexed sender, uint256 amount);

    uint constant public MAX_TOKENS_SOLD = 9000000 * 10**18; // 9M
    uint constant public WAITING_PERIOD = 7 days;

    Token public gnosisToken;
    address public wallet;
    address public owner;
    uint public ceiling;
    uint public priceFactor;
    uint public startBlock;
    uint public endTime;
    uint public totalReceived;
    uint public finalPrice;
    mapping (address => uint) public bids;
    Stages public stage;

    enum Stages {
        AuctionDeployed,
        AuctionSetUp,
        AuctionStarted,
        AuctionEnded,
        TradingStarted
    }

    modifier atStage(Stages _stage) {
        if (stage != _stage)
            throw;
        _;
    }

    modifier isOwner() {
        if (msg.sender != owner)
            throw;
        _;
    }

    modifier isWallet() {
        if (msg.sender != wallet)
            throw;
        _;
    }

    modifier isValidPayload() {
        if (msg.data.length != 4 && msg.data.length != 36)
            throw;
        _;
    }

    modifier timedTransitions() {
        if (stage == Stages.AuctionStarted && calcTokenPrice() <= calcStopPrice())
            finalizeAuction();
        if (stage == Stages.AuctionEnded && now > endTime + WAITING_PERIOD)
            stage = Stages.TradingStarted;
        _;
    }

    function DutchAuction(address _wallet, uint _ceiling, uint _priceFactor) public
    {
        if (_wallet == 0 || _ceiling == 0 || _priceFactor == 0)
            throw;
//0xbe4e25443Df23382228E6347b06565edD838f901
        owner = msg.sender;
//0x851b7f3ab81bd8df354f0d7640efcd7288553419
        wallet = _wallet;
//0x34f086f3b33b68400000
        ceiling = _ceiling;
//0x1194
        priceFactor = _priceFactor;
        stage = Stages.AuctionDeployed;
    }

    function setup(address _gnosisToken) public isOwner atStage(Stages.AuctionDeployed)
    {
        if (_gnosisToken == 0)
            throw;
        gnosisToken = Token(_gnosisToken);
        if (gnosisToken.balanceOf(this) != MAX_TOKENS_SOLD)
            throw;
        stage = Stages.AuctionSetUp;
    }

    function startAuction() public isWallet atStage(Stages.AuctionSetUp)
    {
        stage = Stages.AuctionStarted;
        startBlock = block.number;
    }

    function changeSettings(uint _ceiling, uint _priceFactor) public isWallet atStage(Stages.AuctionSetUp)
    {
        ceiling = _ceiling;
        priceFactor = _priceFactor;
    }

    function calcCurrentTokenPrice() public timedTransitions returns (uint)
    {
        if (stage == Stages.AuctionEnded || stage == Stages.TradingStarted)
            return finalPrice;
        return calcTokenPrice();
    }

    function updateStage() public timedTransitions returns (Stages)
    {
        return stage;
    }

    function bid(address receiver) public payable isValidPayload timedTransitions atStage(Stages.AuctionStarted) returns (uint amount)
    {
        if (receiver == 0)
            receiver = msg.sender;
        amount = msg.value;

        uint maxWei = (MAX_TOKENS_SOLD / 10**18) * calcTokenPrice() - totalReceived;
        uint maxWeiBasedOnTotalReceived = ceiling - totalReceived;
        if (maxWeiBasedOnTotalReceived < maxWei)
            maxWei = maxWeiBasedOnTotalReceived;

        if (amount > maxWei) {
            amount = maxWei;
            if (!receiver.send(msg.value - amount))
                throw;
        }

        if (amount == 0 || !wallet.send(amount))
            throw;
        bids[receiver] += amount;
        totalReceived += amount;
        if (maxWei == amount)
            finalizeAuction();
        BidSubmission(receiver, amount);
    }

    function claimTokens(address receiver) public isValidPayload timedTransitions atStage(Stages.TradingStarted)
    {
        if (receiver == 0)
            receiver = msg.sender;
        uint tokenCount = bids[receiver] * 10**18 / finalPrice;
        bids[receiver] = 0;
        gnosisToken.transfer(receiver, tokenCount);
    }

    function calcStopPrice() constant public returns (uint)
    {
        return totalReceived * 10**18 / MAX_TOKENS_SOLD + 1;
    }

    function calcTokenPrice() constant public returns (uint)
    {
        return priceFactor * 10**18 / (block.number - startBlock + 7500) + 1;
    }

    function finalizeAuction() private
    {
        stage = Stages.AuctionEnded;
        if (totalReceived == ceiling)
            finalPrice = calcTokenPrice();
        else
            finalPrice = calcStopPrice();
        uint soldTokens = totalReceived * 10**18 / finalPrice;
        gnosisToken.transfer(wallet, MAX_TOKENS_SOLD - soldTokens);
        endTime = now;
    }
}
