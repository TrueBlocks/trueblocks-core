contract SingularDTVCrowdfunding {
    function twoYearsPassed() returns (bool);
    function startDate() returns (uint);
    function CROWDFUNDING_PERIOD() returns (uint);
    function TOKEN_TARGET() returns (uint);
    function valuePerShare() returns (uint);
    function fundBalance() returns (uint);
    function campaignEndedSuccessfully() returns (bool);
}