import "AbstractCampaign.sol";
import "AbstractSingularDTVFund.sol";
import "AbstractSingularDTVCrowdfunding.sol";


/// @title Token Creation contract - Implements token creation functionality.
/// @author Stefan George - <stefan.george@consensys.net>
contract SingularDTVWeifund is Campaign {

    /*
     *  External contracts
     */
    SingularDTVFund constant singularDTVFund = SingularDTVFund({{SingularDTVFund}});
    SingularDTVCrowdfunding constant singularDTVCrowdfunding = SingularDTVCrowdfunding({{SingularDTVCrowdfunding}});

    string constant public name = "SingularDTV Campaign";
    string constant public contributeMethodABI = "fund()";
    string constant public refundMethodABI = "withdrawFunding()";
    string constant public payoutMethodABI = "withdrawForWorkshop()";

    /// @notice use to determine the beneficiary destination for the token creation
    /// @return the beneficiary address that will receive the token creation payout
    function beneficiary() constant returns(address) {
        return singularDTVFund.workshop();
    }

    /// @notice the time at which the token creation fails or succeeds
    /// @return the uint unix timestamp at which time the token creation expires
    function expiry() constant returns(uint256 timestamp) {
        return singularDTVCrowdfunding.startDate() + singularDTVCrowdfunding.CROWDFUNDING_PERIOD();
    }

    /// @notice the goal the token creation must reach in order for it to succeed
    /// @return the token creation goal specified in wei as a uint256
    function fundingGoal() constant returns(uint256 amount) {
        return singularDTVCrowdfunding.TOKEN_TARGET() * singularDTVCrowdfunding.valuePerShare();
    }

    /// @notice the goal the token creation must reach in order for it to succeed
    /// @return the token creation goal specified in wei as a uint256
    function amountRaised() constant returns(uint256 amount) {
        return singularDTVCrowdfunding.fundBalance();
    }
}
