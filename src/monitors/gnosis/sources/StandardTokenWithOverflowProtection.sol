contract StandardTokenWithOverflowProtection is Token, SafeMath {

    mapping (address => uint256) balances;
    mapping (address => mapping (address => uint256)) allowed;
    uint256 public totalSupply;

    function transfer(address _to, uint256 _value) public returns (bool)
    {
        if (!safeToSubtract(balances[msg.sender], _value) || !safeToAdd(balances[_to], _value)) {
            throw;
        }
        balances[msg.sender] -= _value;
        balances[_to] += _value;
        Transfer(msg.sender, _to, _value);
        return true;
    }

    function transferFrom(address _from, address _to, uint256 _value) public returns (bool)
    {
        if (!safeToSubtract(balances[_from], _value) || !safeToSubtract(allowed[_from][msg.sender], _value) || !safeToAdd(balances[_to], _value)) {
            throw;
        }
        balances[_from] -= _value;
        allowed[_from][msg.sender] -= _value;
        balances[_to] += _value;
        Transfer(_from, _to, _value);
        return true;
    }

    function approve(address _spender, uint256 _value) public returns (bool) {
        allowed[msg.sender][_spender] = _value;
        Approval(msg.sender, _spender, _value);
        return true;
    }

    function allowance(address _owner, address _spender) constant public returns (uint256) {
        return allowed[_owner][_spender];
    }

    function balanceOf(address _owner) constant public returns (uint256) {
        return balances[_owner];
    }
}
