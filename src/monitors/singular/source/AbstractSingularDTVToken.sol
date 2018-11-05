import "AbstractToken.sol";


contract SingularDTVToken is Token {
    function issueTokens(address _for, uint tokenCount) returns (bool);
}