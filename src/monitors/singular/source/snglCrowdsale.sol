import "AbstractSingularDTVToken.sol";
import "AbstractSingularDTVFund.sol";


/// @title Token Creation contract - Implements token creation functionality.
/// @author Stefan George - <stefan.george@consensys.net>
contract SingularDTVCrowdfunding {

    /*
     *  External contracts
     */
    SingularDTVToken public singularDTVToken;
    SingularDTVFund public singularDTVFund;

    /*
     *  Constants
     */
    uint constant public CAP = 1000000000; // 1B tokens is the maximum amount of tokens
    uint constant public CROWDFUNDING_PERIOD = 4 weeks; // 1 month
    uint constant public TOKEN_LOCKING_PERIOD = 2 years; // 2 years
    uint constant public TOKEN_TARGET = 534000000; // Goal threshold

    /*
     *  Enums
     */
    enum Stages {
        CrowdfundingGoingAndGoalNotReached,
        CrowdfundingEndedAndGoalNotReached,
        CrowdfundingGoingAndGoalReached,
        CrowdfundingEndedAndGoalReached
    }

    /*
     *  Storage
     */
    address public owner;
    uint public startDate;
    uint public fundBalance;
    uint public baseValue = 1250 szabo; // 0.00125 ETH
    uint public valuePerShare = baseValue; // 0.00125 ETH

    // participant address => value in Wei
    mapping (address => uint) public investments;

    // Initialize stage
    Stages public stage = Stages.CrowdfundingGoingAndGoalNotReached;

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
        // Only owner is allowed to do this action.
        if (msg.sender != owner) {
            throw;
        }
        _
    }

    modifier minInvestment() {
        // User has to send at least the ether value of one token.
        if (msg.value < valuePerShare) {
            throw;
        }
        _
    }

    modifier atStage(Stages _stage) {
        if (stage != _stage) {
            throw;
        }
        _
    }

    modifier atStageOR(Stages _stage1, Stages _stage2) {
        if (stage != _stage1 && stage != _stage2) {
            throw;
        }
        _
    }

    modifier timedTransitions() {
        uint crowdfundDuration = now - startDate;
        if (crowdfundDuration >= 22 days) {
            valuePerShare = baseValue * 1500 / 1000;
        }
        else if (crowdfundDuration >= 18 days) {
            valuePerShare = baseValue * 1375 / 1000;
        }
        else if (crowdfundDuration >= 14 days) {
            valuePerShare = baseValue * 1250 / 1000;
        }
        else if (crowdfundDuration >= 10 days) {
            valuePerShare = baseValue * 1125 / 1000;
        }
        else {
            valuePerShare = baseValue;
        }
        if (crowdfundDuration >= CROWDFUNDING_PERIOD) {
            if (stage == Stages.CrowdfundingGoingAndGoalNotReached) {
                stage = Stages.CrowdfundingEndedAndGoalNotReached;
            }
            else if (stage == Stages.CrowdfundingGoingAndGoalReached) {
                stage = Stages.CrowdfundingEndedAndGoalReached;
            }
        }
        _
    }

    /*
     *  Contract functions
     */
    /// dev Validates invariants.
    function checkInvariants() constant internal {
        if (fundBalance > this.balance) {
            throw;
        }
    }

    /// @dev Can be triggered if an invariant fails.
    function emergencyCall()
        external
        noEther
        returns (bool)
    {
        if (fundBalance > this.balance) {
            if (this.balance > 0 && !singularDTVFund.workshop().send(this.balance)) {
                throw;
            }
            return true;
        }
        return false;
    }

    /// @dev Allows user to create tokens if token creation is still going and cap not reached. Returns token count.
    function fund()
        external
        timedTransitions
        atStageOR(Stages.CrowdfundingGoingAndGoalNotReached, Stages.CrowdfundingGoingAndGoalReached)
        minInvestment
        returns (uint)
    {
        uint tokenCount = msg.value / valuePerShare; // Token count is rounded down. Sent ETH should be multiples of valuePerShare.
        if (singularDTVToken.totalSupply() + tokenCount > CAP) {
            // User wants to create more tokens than available. Set tokens to possible maximum.
            tokenCount = CAP - singularDTVToken.totalSupply();
        }
        uint investment = tokenCount * valuePerShare; // Ether spent by user.
        // Send change back to user.
        if (msg.value > investment && !msg.sender.send(msg.value - investment)) {
            throw;
        }
        // Update fund's and user's balance and total supply of tokens.
        fundBalance += investment;
        investments[msg.sender] += investment;
        if (!singularDTVToken.issueTokens(msg.sender, tokenCount)) {
            // Tokens could not be issued.
            throw;
        }
        // Update stage
        if (stage == Stages.CrowdfundingGoingAndGoalNotReached) {
            if (singularDTVToken.totalSupply() >= TOKEN_TARGET) {
                stage = Stages.CrowdfundingGoingAndGoalReached;
            }
        }
        // not an else clause for the edge case that the CAP and TOKEN_TARGET are reached in one call
        if (stage == Stages.CrowdfundingGoingAndGoalReached) {
            if (singularDTVToken.totalSupply() == CAP) {
                stage = Stages.CrowdfundingEndedAndGoalReached;
            }
        }
        checkInvariants();
        return tokenCount;
    }

    /// @dev Allows user to withdraw ETH if token creation period ended and target was not reached. Returns success.
    function withdrawFunding()
        external
        noEther
        timedTransitions
        atStage(Stages.CrowdfundingEndedAndGoalNotReached)
        returns (bool)
    {
        // Update fund's and user's balance and total supply of tokens.
        uint investment = investments[msg.sender];
        investments[msg.sender] = 0;
        fundBalance -= investment;
        // Send ETH back to user.
        if (investment > 0  && !msg.sender.send(investment)) {
            throw;
        }
        checkInvariants();
        return true;
    }

    /// @dev Withdraws ETH to workshop address. Returns success.
    function withdrawForWorkshop()
        external
        noEther
        timedTransitions
        atStage(Stages.CrowdfundingEndedAndGoalReached)
        returns (bool)
    {
        uint value = fundBalance;
        fundBalance = 0;
        if (value > 0  && !singularDTVFund.workshop().send(value)) {
            throw;
        }
        checkInvariants();
        return true;
    }

    /// @dev Sets token value in Wei.
    /// @param valueInWei New value.
    function changeBaseValue(uint valueInWei)
        external
        noEther
        onlyOwner
        returns (bool)
    {
        baseValue = valueInWei;
        return true;
    }

    /// @dev Returns true if 2 years have passed since the beginning of token creation period.
    function twoYearsPassed()
        constant
        external
        noEther
        returns (bool)
    {
        return now - startDate >= TOKEN_LOCKING_PERIOD;
    }

    /// @dev Returns if token creation ended successfully.
    function campaignEndedSuccessfully()
        constant
        external
        noEther
        returns (bool)
    {
        if (stage == Stages.CrowdfundingEndedAndGoalReached) {
            return true;
        }
        return false;
    }

    // updateStage allows calls to receive correct stage. It can be used for transactions but is not part of the regular token creation routine.
    // It is not marked as constant because timedTransitions modifier is altering state and constant is not yet enforced by solc.
    /// @dev returns correct stage, even if a function with timedTransitions modifier has not yet been called successfully.
    function updateStage()
        external
        timedTransitions
        noEther
        returns (Stages)
    {
        return stage;
    }

    /// @dev Setup function sets external contracts' addresses.
    /// @param singularDTVFundAddress Crowdfunding address.
    /// @param singularDTVTokenAddress Token address.
    function setup(address singularDTVFundAddress, address singularDTVTokenAddress)
        external
        onlyOwner
        noEther
        returns (bool)
    {
        if (address(singularDTVFund) == 0 && address(singularDTVToken) == 0) {
            singularDTVFund = SingularDTVFund(singularDTVFundAddress);
            singularDTVToken = SingularDTVToken(singularDTVTokenAddress);
            return true;
        }
        return false;
    }

    /// @dev Contract constructor function sets owner and start date.
    function SingularDTVCrowdfunding() noEther {
        // Set owner address
        owner = msg.sender;
        // Set start-date of token creation
        startDate = now;
    }

    /// @dev Fallback function always fails. Use fund function to create tokens.
    function () {
        throw;
    }
}
