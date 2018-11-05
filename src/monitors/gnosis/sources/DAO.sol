contract DAO {

    EventFactory public eventFactory;

    address public wallet;
    address public owner;

    function changeDAO(address daoAddress) external
    {
        // isWallet
        if (msg.sender != wallet)
            throw;
        eventFactory.changeDAO(daoAddress);
    }

    function setup(address eventFactoryAddress, address walletAddress) external
    {
        // isOwner
        if (msg.sender != owner)
            throw;
        if (address(eventFactory) != 0 || wallet != 0) {
            throw;
        }
        eventFactory = EventFactory(eventFactoryAddress);
        wallet = walletAddress;
    }

    function DAO() {
        owner = msg.sender;
    }

    function calcBaseFee(address sender, address token, uint tokenCount) constant external returns (uint fee)
    {
        return 0;
    }

    function calcBaseFeeForShares(address sender, address token, uint shareCount) constant external returns (uint fee)
    {
        return 0;
    }
}
