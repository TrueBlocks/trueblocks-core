import "AbstractSingularDTVToken.sol";
import "AbstractSingularDTVCrowdfunding.sol";


/// @title Fund contract - Implements revenue distribution.
/// @author Stefan George - <stefan.george@consensys.net>
contract SingularDTVFund {

    /*
     *  External contracts
     */
    SingularDTVToken public singularDTVToken;
    SingularDTVCrowdfunding public singularDTVCrowdfunding;

    /*
     *  Storage
     */
    address public owner;
    address constant public workshop = {{MistWallet}};
    uint public totalRevenue;

    // User's address => Revenue at time of withdraw
    mapping (address => uint) public revenueAtTimeOfWithdraw;

    // User's address => Revenue which can be withdrawn
    mapping (address => uint) public owed;

    /*
     *  Modifiers
     */
    modifier noEther() {
        if (msg.value > 0) {
            throw;
        }
        _
    }

    modifier onlyOwner() {
        // Only guard is allowed to do this action.
        if (msg.sender != owner) {
            throw;
        }
        _
    }

    modifier campaignEndedSuccessfully() {
        if (!singularDTVCrowdfunding.campaignEndedSuccessfully()) {
            throw;
        }
        _
    }

    /*
     *  Contract functions
     */
    /// @dev Deposits revenue. Returns success.
    function depositRevenue()
        external
        campaignEndedSuccessfully
        returns (bool)
    {
        totalRevenue += msg.value;
        return true;
    }

    /// @dev Withdraws revenue for user. Returns revenue.
    /// @param forAddress user's address.
    function calcRevenue(address forAddress) internal returns (uint) {
        return singularDTVToken.balanceOf(forAddress) * (totalRevenue - revenueAtTimeOfWithdraw[forAddress]) / singularDTVToken.totalSupply();
    }

    /// @dev Withdraws revenue for user. Returns revenue.
    function withdrawRevenue()
        external
        noEther
        returns (uint)
    {
        uint value = calcRevenue(msg.sender) + owed[msg.sender];
        revenueAtTimeOfWithdraw[msg.sender] = totalRevenue;
        owed[msg.sender] = 0;
        if (value > 0 && !msg.sender.send(value)) {
            throw;
        }
        return value;
    }

    /// @dev Credits revenue to owed balance.
    /// @param forAddress user's address.
    function softWithdrawRevenueFor(address forAddress)
        external
        noEther
        returns (uint)
    {
        uint value = calcRevenue(forAddress);
        revenueAtTimeOfWithdraw[forAddress] = totalRevenue;
        owed[forAddress] += value;
        return value;
    }

    /// @dev Setup function sets external contracts' addresses.
    /// @param singularDTVTokenAddress Token address.
    function setup(address singularDTVCrowdfundingAddress, address singularDTVTokenAddress)
        external
        noEther
        onlyOwner
        returns (bool)
    {
        if (address(singularDTVCrowdfunding) == 0 && address(singularDTVToken) == 0) {
            singularDTVCrowdfunding = SingularDTVCrowdfunding(singularDTVCrowdfundingAddress);
            singularDTVToken = SingularDTVToken(singularDTVTokenAddress);
            return true;
        }
        return false;
    }

    /// @dev Contract constructor function sets guard address.
    function SingularDTVFund() noEther {
        // Set owner address
        owner = msg.sender;
    }
}
