contract DAOToken is StandardToken {

    string constant public name = "Gnosis Token";
    string constant public symbol = "GNO";
    uint8 constant public decimals = 18;

    DAOAuction public daoAuction;
    function transfer(address to, uint256 value) public returns (bool success)
    {
        // tokenLaunched
        if (!daoAuction.tokenLaunched() && msg.sender != address(daoAuction))
            throw;
        return super.transfer(to, value);
    }

    function transferFrom(address from, address to, uint256 value) public returns (bool success)
    {
        // tokenLaunched
        if (!daoAuction.tokenLaunched() && msg.sender != address(daoAuction))
            throw;
        return super.transferFrom(from, to, value);
    }

    function DAOToken(address _daoAuction) {
        daoAuction = DAOAuction(_daoAuction);
        uint _totalSupply = 10000000 * 10**18;
        balances[_daoAuction] = _totalSupply;
        totalSupply = _totalSupply;
    }
}
