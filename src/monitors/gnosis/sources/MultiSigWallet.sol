contract MultiSigWallet {

    event Confirmation(address sender, bytes32 transactionHash);
    event Revocation(address sender, bytes32 transactionHash);
    event Submission(address sender, bytes32 transactionHash);
    event Execution(address sender, bytes32 transactionHash);
    event Deposit(address sender, uint value);
    event OwnerAddition(address owner);
    event OwnerRemoval(address owner);
    event RequiredUpdate(uint required);

    mapping (bytes32 => Transaction) public transactions;
    mapping (bytes32 => mapping (address => bool)) public confirmations;
    mapping (address => bool) public isOwner;
    address[] public owners;
    uint public required;

    struct Transaction {
        address destination;
        uint value;
        bytes data;
        uint nonce;
        bool executed;
    }

    function addOwner(address owner) external
    {
        // ownerDoesNotExist
        if (isOwner[owner])
            throw;
        // onlyWallet
        if (msg.sender != address(this))
            throw;
        isOwner[owner] = true;
        owners.push(owner);
        OwnerAddition(owner);
    }

    function removeOwner(address owner) external
    {
        // ownerExists
        if (!isOwner[owner])
            throw;
        // onlyWallet
        if (msg.sender != address(this))
            throw;
        for (uint i=0; i<owners.length - 1; i++) {
            if (owners[i] == owner)
                isOwner[owner] = false;
            if (!isOwner[owner])
                owners[i] = owners[i+1];
        }
        owners.length -= 1;
        if (required > owners.length)
            required = owners.length;
        OwnerRemoval(owner);
    }

    function updateRequired(uint _required) external
    {
        // maxRequired
        if (_required > owners.length)
            throw;
        // onlyWallet
        if (msg.sender != address(this))
            throw;
        required = _required;
        RequiredUpdate(_required);
    }

    function confirmationCount(bytes32 transactionHash) constant public returns (uint count)
    {
        for (uint i=0; i<owners.length; i++)
            if (confirmations[transactionHash][owners[i]])
                count += 1;
    }

    function submitTransaction(address destination, uint value, bytes data, uint nonce) external returns (bytes32 transactionHash)
    {
        // notExecuted
        if (transactions[sha3(destination, value, data, nonce)].executed)
            throw;
        // ownerExists
        if (!isOwner[msg.sender])
            throw;
        transactionHash = sha3(destination, value, data,  nonce);
        transactions[transactionHash] = Transaction({
            destination: destination,
            value: value,
            data: data,
            nonce: nonce,
            executed: false
        });
        Submission(msg.sender, transactionHash);
        confirmTransaction(transactionHash);
    }

    function confirmTransaction(bytes32 transactionHash) public
    {
        // notExecuted
        if (transactions[transactionHash].executed)
            throw;
        // ownerExists
        if (!isOwner[msg.sender])
            throw;
        confirmations[transactionHash][msg.sender] = true;
        Confirmation(msg.sender, transactionHash);
        if (confirmationCount(transactionHash) >= required) {
            Transaction tx = transactions[transactionHash];
            tx.executed = true;
            if (!tx.destination.call.value(tx.value)(tx.data))
                throw;
            Execution(msg.sender, transactionHash);
        }
    }

    function revokeConfirmation(bytes32 transactionHash) external
    {
        // notExecuted
        if (transactions[transactionHash].executed)
            throw;
        // ownerExists
        if (!isOwner[msg.sender])
            throw;
        confirmations[transactionHash][msg.sender] = false;
        Revocation(msg.sender, transactionHash);
    }

    function MultiSigWallet(address[] _owners, uint _required)
    {
        // maxRequired
        if (_required > owners.length)
            throw;
        for (uint i=0; i<_owners.length; i++)
            isOwner[_owners[i]] = true;
        required = _required;
        owners = _owners;
    }

    function() payable
    {
        if (msg.value > 0)
            Deposit(msg.sender, msg.value);
    }
}
