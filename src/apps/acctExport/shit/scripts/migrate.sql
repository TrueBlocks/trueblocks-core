DROP DATABASE IF EXISTS tokenomics_local_testing;
DROP DATABASE IF EXISTS tokenomics_local;

SELECT "----------- Creating databases --------------------" AS '';
CREATE DATABASE tokenomics_local;
CREATE DATABASE tokenomics_local_testing;
USE tokenomics_local;

SELECT '-------- table: eth_price_usd -------------------------' AS '';
CREATE TABLE IF NOT EXISTS eth_price_usd (
    timeStamp         INT(11) UNSIGNED NOT NULL,
    exchangeRate      DECIMAL(25, 18) UNSIGNED NOT NULL)
DEFAULT CHARACTER SET utf8mb4;
ALTER TABLE eth_price_usd ADD PRIMARY KEY eth_price_usd_pkey (timeStamp);
DESCRIBE price;

SELECT '-------- table: token_price_eth -------------------------' AS '';
CREATE TABLE IF NOT EXISTS token_price_eth (
    timeStamp         INT(11) UNSIGNED NOT NULL,
    tokenAddress      VARCHAR(42) NOT NULL,
    exchangeRate      DECIMAL(25, 18) UNSIGNED NOT NULL)
DEFAULT CHARACTER SET utf8mb4;
ALTER TABLE token_price_eth ADD PRIMARY KEY token_price_eth_pkey (timeStamp, tokenAddress);
DESCRIBE token_price_eth;

SELECT "----------- table: abi_spec --------------------" AS '';
CREATE TABLE abi_spec (
    encoding VARBINARY(10) NOT NULL,
    canonical JSON NOT NULL)
DEFAULT CHARACTER SET utf8mb4;
ALTER TABLE abi_spec ADD PRIMARY KEY abi_spec_pkey (encoding);
DESCRIBE abi_spec;

SELECT '-------- table: block -------------------------' AS '';
CREATE TABLE IF NOT EXISTS block (
    blockNumber       INT UNSIGNED NOT NULL,
    timeStamp         INT(11) UNSIGNED NOT NULL,
    isFinalized       BOOLEAN NOT NULL DEFAULT 0)
DEFAULT CHARACTER SET utf8mb4;
ALTER TABLE block ADD PRIMARY KEY block_pkey (blockNumber);
DESCRIBE block;

SELECT '-------- table: monitor -------------------------' AS '';
CREATE TABLE IF NOT EXISTS monitor (
    monitorAddress     VARCHAR(42) NOT NULL,
    nickname           VARCHAR(100),
    firstBlock         INT UNSIGNED,
    monitorStatus      BOOLEAN DEFAULT 1)
DEFAULT CHARACTER SET utf8mb4;
ALTER TABLE monitor ADD PRIMARY KEY monitor_pkey (monitorAddress);
DESCRIBE monitor;

SELECT '-------- table: monitor_group -------------------------' AS '';
CREATE TABLE IF NOT EXISTS monitor_group (
    monitorGroupID     INT UNSIGNED NOT NULL,
    monitorGroupName   VARCHAR(100))
DEFAULT CHARACTER SET utf8mb4;
ALTER TABLE monitor_group ADD PRIMARY KEY monitor_group_pkey (monitorGroupID);
DESCRIBE monitor_group;

SELECT '-------- table: monitor_monitor_group -------------------------' AS '';
CREATE TABLE IF NOT EXISTS monitor_monitor_group (
    monitorGroupID     INT UNSIGNED NOT NULL,
    monitorAddress     VARCHAR(42) NOT NULL)
DEFAULT CHARACTER SET utf8mb4;
    ALTER TABLE monitor_monitor_group
    ADD CONSTRAINT monitor_monitor_group_monitorgroupid_foreign FOREIGN KEY (monitorGroupID)
    REFERENCES monitor_group (monitorGroupID)
    ON DELETE CASCADE;
ALTER TABLE monitor_monitor_group
    ADD CONSTRAINT monitor_monitor_group_monitoraddress_foreign FOREIGN KEY (monitorAddress)
    REFERENCES monitor (monitorAddress)
    ON DELETE CASCADE;
ALTER TABLE monitor_monitor_group ADD PRIMARY KEY monitor_monitor_group_pkey (monitorGroupID, monitorAddress);
DESCRIBE monitor_monitor_group;

SELECT '-------- table: transaction -------------------------' AS '';
CREATE TABLE transaction (
    blockNumber        INT UNSIGNED NOT NULL,
    transID            INT UNSIGNED NOT NULL,
    traceID            INT UNSIGNED NOT NULL,
    fromAddress        VARCHAR(42) NOT NULL,
    toAddress          VARCHAR(42) NOT NULL,
    valueWei           DECIMAL(38,0) UNSIGNED NOT NULL,
    gasUsed            BIGINT UNSIGNED NOT NULL,
    gasPrice           BIGINT UNSIGNED NOT NULL,
    isError            BOOLEAN NOT NULL DEFAULT 0,
    encoding           VARCHAR(10) NOT NULL,
    articulated        JSON)
DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_bin;
ALTER TABLE transaction
    ADD CONSTRAINT transaction_blockNumber_foreign FOREIGN KEY (blockNumber)
    REFERENCES block (blockNumber)
    ON DELETE CASCADE;
ALTER TABLE transaction ADD PRIMARY KEY transaction_pkey (blockNumber, transID, traceID);
DESCRIBE transaction;

SELECT '-------- table: monitor_transaction -------------------------' AS '';
CREATE TABLE monitor_transaction (
    monitorAddress     VARCHAR(42) NOT NULL,
    blockNumber        INT UNSIGNED NOT NULL,
    transID            INT UNSIGNED NOT NULL,
    traceID            INT UNSIGNED NOT NULL)
DEFAULT CHARACTER SET utf8mb4;
ALTER TABLE monitor_transaction
    ADD CONSTRAINT monitor_transaction_blockNumber_transID_traceID_foreign FOREIGN KEY (blockNumber, transID, traceID)
    REFERENCES transaction (blockNumber, transID, traceID)
    ON DELETE CASCADE;
ALTER TABLE monitor_transaction
    ADD CONSTRAINT monitor_transaction_monitorAddress_foreign FOREIGN KEY (monitorAddress)
    REFERENCES monitor (monitorAddress)
    ON DELETE CASCADE;
ALTER TABLE monitor_transaction
    ADD PRIMARY KEY monitor_transaction_pkey (monitorAddress, blockNumber, transID, traceID);
DESCRIBE monitor_transaction;
