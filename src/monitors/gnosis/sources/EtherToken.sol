contract EtherToken is StandardTokenWithOverflowProtection {
    string constant public name = "Ether Token";
    string constant public symbol = "ETH";
    uint8 constant public decimals = 18;
    function deposit() external payable {
        if (   !safeToAdd(balances[msg.sender], msg.value)
            || !safeToAdd(totalSupply, msg.value))
        {
            throw;
        }
        balances[msg.sender] += msg.value;
        totalSupply += msg.value;
    }

    function withdraw(uint amount) external {
        if (   !safeToSubtract(balances[msg.sender], amount)
            || !safeToSubtract(totalSupply, amount))
        {
            throw;
        }
        balances[msg.sender] -= amount;
        totalSupply -= amount;
        if (!msg.sender.send(amount)) {
            throw;
        }
    }
}
