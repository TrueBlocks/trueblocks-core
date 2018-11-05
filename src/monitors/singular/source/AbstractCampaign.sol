contract Campaign {
    /// @notice the campaign name
    /// @return contractual metadata which specifies the campaign name as a string
    function name() constant returns(string) {}

    /// @notice use to determine the contribution method abi
    /// @return will return a string that is the exact contributeMethodABI
    function contributeMethodABI() constant returns(string) {}

    /// @notice use to determine the contribution method abi
    /// @return will return a string that is the exact contributeMethodABI
    function refundMethodABI() constant returns(string) {}

    /// @notice use to determine the contribution method abi
    /// @return will return a string that is the exact contributeMethodABI
    function payoutMethodABI() constant returns(string) {}

    /// @notice use to determine the beneficiary destination for the campaign
    /// @return the beneficiary address that will receive the campaign payout
    function beneficiary() constant returns(address) {}

    /// @notice the time at which the campaign fails or succeeds
    /// @return the uint unix timestamp at which time the campaign expires
    function expiry() constant returns(uint256 timestamp) {}

    /// @notice the goal the campaign must reach in order for it to succeed
    /// @return the campaign funding goal specified in wei as a uint256
    function fundingGoal() constant returns(uint256 amount) {}

    /// @notice the goal the campaign must reach in order for it to succeed
    /// @return the campaign funding goal specified in wei as a uint256
    function amountRaised() constant returns(uint256 amount) {}

    // Campaign events
    event ContributionMade (address _contributor);
    event RefundPayoutClaimed(uint256 _contributionID);
    event BeneficiaryPayoutClaimed (address _beneficiary, uint256 _payoutAmount);
}
