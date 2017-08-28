/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"
#include "accountname.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

   // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("fastest");

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool loaded = loadData();
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (!loaded) {
            usage(configPath(NAMES_FILENAME) + " is empty. Use ethName -e to add some names. Quitting...");
            exit(0);
        }

        SFString fmt = (options.addrOnly ? "[{ADDR}]" : "");
        if (options.list) {
            if (options.count)
                cout << accounts.getCount() << " items\n";
            for (uint32_t i = 0 ; i < accounts.getCount() ; i++)
                cout << accounts[i].Format(fmt).Substitute("\n", " ").Substitute("  ", " ") << "\n";
            exit(0);
        }

        SFString ret = showName(options);
        if (!ret.empty())
            cout << ret;
        else if (verbose)
            cout << "Address '" << options.addr << "' not found\n";
        cout.flush();
    }

    return 0;
}

//-----------------------------------------------------------------------
uint32_t countOf(const SFString& addr) {
    uint32_t cnt = 0;
    for (uint32_t i = 0 ; i < accounts.getCount() ; i++)
        if (accounts[i].addr % addr)
            cnt++;
    return cnt;
}

//-----------------------------------------------------------------------
SFString showName(const COptions& options) {
    SFString ret;
    uint32_t hits = 0;
    SFString fmt = (options.addrOnly ? "[{ADDR}]" : "");
    for (uint32_t i = 0 ; i < accounts.getCount() ; i++) {
        if (accounts[i].Match(options.addr, options.name, options.source, options.matchCase, options.all)) {
            ret += (accounts[i].Format(fmt).Substitute("\n", " ").Substitute("  ", " ") + "\n");
            hits++;
        }
    }

    if (options.count)
        ret = asString(hits) + " match" + (hits==1?"":"es") + "\n" + (verbose ? ret : "");
    return ret;
}

//-----------------------------------------------------------------------
SFString setName(const SFString& addr, const SFString& name) {
    CStringExportContext ctx;
    ctx << "setName(" << addr << "," << name << ");\n";
    return ctx.str;
}

//-----------------------------------------------------------------------
CAccountNameArray accounts;

//---------------------------------------------------------------------------
bool CAccountName::Match(const SFString& s1, const SFString& s2, const SFString& s3, bool matchCase, bool all) {

    SFString theAddr   = addr;
    SFString theName   = name + " " + symbol;
    SFString theSource = source;

    bool m11 = (matchCase ? theAddr.Contains(s1)   : theAddr.ContainsI(s1));
    bool m12 = (matchCase ? theName.Contains(s1)   : theName.ContainsI(s1));
    bool m13 = (matchCase ? theSource.Contains(s1) : theSource.ContainsI(s1));
    bool m2  = (matchCase ? theName.Contains(s2)   : theName.ContainsI(s2));
    bool m3  = (matchCase ? theSource.Contains(s3) : theSource.ContainsI(s3));

    if (!s1.empty() && !s2.empty() && !s3.empty())
        return m11 && m2 && m3;  // all three must match

    if (!s1.empty() && !s2.empty())
        return m11 && m2;  // addr and name must both match

    if (s1.empty())
        return false;  // nothing matches

    // We have only s1
    return (all ? m11 || m12 || m13 : m11 || m12);
}

extern const char *STR_DEFAULT_DATA;
//-----------------------------------------------------------------------
bool loadData(void) {
    if (accounts.getCount() > 0)
        return true;

    if (!folderExists(configPath("configs/")))
        establishFolder(configPath("configs/"));

    SFString contents = StripAny(asciiFileToString(configPath(NAMES_FILENAME)), "\t\n ");
    contents.ReplaceAll("\t\t", "\t");
    if (contents.empty()) {
        stringToAsciiFile(configPath(NAMES_FILENAME), STR_DEFAULT_DATA);
        contents = STR_DEFAULT_DATA;
    }

    SFString custom = asciiFileToString(configPath(CUSTOM_NAMES));
    custom.ReplaceAll("\t\t", "\t");
    if (!isTestMode()) {
        contents += custom;
    }

    contents.ReplaceAll("\t\t", "\t");
    if (contents.empty()) {
        stringToAsciiFile(configPath(NAMES_FILENAME), STR_DEFAULT_DATA);
        contents = STR_DEFAULT_DATA;
    }

    while (!contents.empty()) {
        SFString line = nextTokenClear(contents, '\n');
        if (!line.startsWith("#")) {
            if (countOf('\t', line) < 2) {
                cerr << "Line " << line << " does not contain two tabs.\n";

            } else {
                CAccountName account(line);
                accounts[accounts.getCount()] = account;
            }
        }
    }
    return true;
}

const char *STR_DEFAULT_DATA =
"#---------------------------------------------------------------------------------------------------\n"
"#  This is the ethName database. Format records as tab seperated lines with the following format:\n"
"#\n"
"#      Optional Symbol <tab> Name <tab> Address <tab> Source of the data <tab> Description <newline>\n"
"#\n"
"#---------------------------------------------------------------------------------------------------------------\n"
"# Sym	Name				Address						Source		Description\n"
"#---------------------------------------------------------------------------------------------------------------\n"
"DAO	The DAO				0xbb9bc244d798123fde783fcc1c72d3bb8c189413	Etherscan.io	The infamous DAO smart contract\n"
"QTUM	Qtum				0x9a642d6b3368ddc662CA244bAdf32cDA716005BC	Etherscan.io	Build Decentralized Applications that Simply Work Executable on mobile devices, compatible with major existing blockchain ecosystem\n"
"OMG	OMGToken			0xd26114cd6EE289AccF82350c8d8487fedB8A0C07	Etherscan.io	A public Ethereum-based financial technology for use in mainstream digital wallets\n"
"EOS	EOS				0x86Fa049857E0209aa7D9e616F7eb3b3B78ECfdb0	Etherscan.io	Infrastructure for Decentralized Applications\n"
"PAY	TenX Pay Token			0xB97048628DB6B661D4C2aA833e95Dbe1A905B280	Etherscan.io	TenX connects your blockchain assets for everyday use. TenX debit card and banking licence will allow us to be a hub for the blockchain ecosystem to connect for real-world use cases\n"
"ICN	ICONOMI				0x888666CA69E0f178DED6D75b5726Cee99A87D698	Etherscan.io	Digital Assets Management platform enables simple access to a variety of digital assets and combined Digital Asset Arrays\n"
"GNT	Golem Network Token		0xa74476443119A942dE498590Fe1f2454d7D4aC0d	Etherscan.io	Golem is going to create the first decentralized global market for computing power\n"
"REP	Augur (Reputation)		0xE94327D07Fc17907b4DB788E5aDf2ed424adDff6	Etherscan.io	Augur combines the magic of prediction markets with the power of a decentralized network to create a stunningly accurate forecasting tool\n"
"MKR	Maker				0xC66eA802717bFb9833400264Dd12c2bCeAa34a6d	Etherscan.io	Maker is a Decentralized Autonomous Organization that creates and insures the dai stablecoin on the Ethereum blockchain\n"
"SNT	Status Network Token		0x744d70FDBE2Ba4CF95131626614a1763DF805B9E	Etherscan.io	Status is an open source messaging platform and mobile browser to interact with decentralized applications that run on the Ethereum Network\n"
"CVC	Civic				0x41e5560054824eA6B0732E656E3Ad64E20e94E45	Etherscan.io	Giving businesses and individuals the tools to control and protect identities\n"
"GNO	Gnosis Token			0x6810e776880C02933D47DB1b9fc05908e5386b96	Etherscan.io	Crowd Sourced Wisdom - The next generation blockchain network. Speculate on anything with an easy-to-use prediction market\n"
"BAT	Basic Attention Token		0x0D8775F648430679A709E98d2b0Cb6250d2887EF	Etherscan.io	The Basic Attention Token is the new token for the digital advertising industry\n"
"DGD	Digix Global			0xE0B7927c4aF23765Cb51314A0E0521A9645F0E2A	Etherscan.io	Every asset represents a unique bullion bar sitting in designated securitised custodial vaults\n"
"BNT	Bancor Network Token		0x1F573D6Fb3F13d689FF844B4cE37794d79a7FF1C	Etherscan.io	Bancor Protocol is a standard for a new generation of cryptocurrencies called Smart Tokens\n"
"STORJ	StorjToken			0xB64ef51C888972c908CFacf59B47C1AfBC0Ab8aC	Etherscan.io	Blockchain-based, end-to-end encrypted, distributed object storage, where only you have access to your data\n"
"FUN	FunFair				0x419D0d8BdD9aF5e606Ae2232ed285Aff190E711b	Etherscan.io	FunFair is a decentralised gaming platform powered by Ethereum smart contracts\n"
"SNGLS	SingularDTV			0xaeC2E87E0A235266D9C5ADc9DEb4b2E29b54D009	Etherscan.io	A Blockchain Entertainment Studio, Smart Contract Rights Management Platform and Video On-Demand Portal\n"
"DNT	district0x Netwrk Token		0x0AbdAce70D3790235af448C88547603b945604ea	Etherscan.io	A network of decentralized markets and communities. Create, operate, and govern. Powered by Ethereum, Aragon, and IPFS\n"
"ANT	Aragon Network Token		0x960b236A07cf122663c4303350609A66A7B288C0	Etherscan.io	Create and manage unstoppable organizations. Aragon lets you manage entire organizations using the blockchain. This makes Aragon organizations more efficient than their traditional counterparties\n"
"EDG	Edgeless			0x08711D3B02C8758F2FB3ab4e80228418a7F8e39c	Etherscan.io	The Ethereum smart contract-based that offers a 0% house edge and solves the transparency question once and for all\n"
"1ST	FirstBlood Token		0xAf30D2a7E90d7DC361c8C4585e9BB7D2F6f15bc7	Etherscan.io	FirstBlood.io is a decentralized Esports gaming platform that is powered by the blockchain\n"
"RLC	iEx.ec Network Token		0x607F4C5BB672230e8672085532f7e901544a7375	Etherscan.io	Blockchain Based distributed cloud computing\n"
"WINGS	Wings				0x667088b212ce3d06a1b553a7221E1fD19000d9aF	Etherscan.io	A decentralized platform to create, join and manage projects\n"
"BQX	Bitquence			0x5Af2Be193a6ABCa9c8817001F45744777Db30756	Etherscan.io	People-powered cryptocurrency services for the blockchain\n"
"MLN	Melon Token			0xBEB9eF514a379B997e0798FDcC901Ee474B6D9A1	Etherscan.io	The Melon protocol is a blockchain protocol for digital asset management built on the Ethereum platform\n"
"ROL	Etheroll (Dice)			0x2e071D2966Aa7D8dECB1005885bA1977D6038A65	Etherscan.io	Provably fair online Ether gaming on the Ethereum Blockchain\n"
"LUN	Lunyr Token			0xfa05A73FfE78ef8f1a739473e462c54bae6567D9	Etherscan.io	Lunyr is an Ethereum-based decentralized world knowledge base which rewards users with app tokens for peer-reviewing and contributing information\n"
"	AdToken				0xD0D6D6C5Fe4a677D343cC433536BB717bAe167dD	Etherscan.io	adChain is a browser agnostic Ethereum based solution for digital advertising that seamlessly integrates with pre-existing RTB and programmatic industry standards\n"
"MCO	Monaco				0xB63B606Ac810a52cCa15e44bB630fd42D8d1d83d	Etherscan.io	Monaco is a cryptocurrency card. Spend and send money globally at interbank exchange rates\n"
"vSlice	vSlice Token			0x5c543e7AE0A1104f78406C340E9C64FD9fCE5170	Etherscan.io	An Ethereum Gaming Platform Token\n"
"HMQ	Humaniq				0xcbCC0F036ED4788F63FC0fEE32873d6A7487b908	Etherscan.io	Humaniq aims to be a simple and secure 4th generation mobile bank\n"
"TAAS	Token-as-a-Service		0xE7775A6e9Bcf904eb39DA2b68c5efb4F9360e08C	Etherscan.io	TaaS is a tokenized closed-end fund designed to reduce the risks and technical barriers of investing in the blockchain space\n"
"TKN	Monolith TKN			0xaAAf91D9b90dF800Df4F55c205fd6989c977E73a	Etherscan.io	The All Powerful Smart Contract Powered Debit Card\n"
"TRST	WeTrust (Trustcoin)		0xCb94be6f13A1182E4A4B6140cb7bf2025d28e41B	Etherscan.io	A platform for Trusted Lending Circles, powered by people and blockchain\n"
"XAUR	Xaurum				0x4DF812F6064def1e5e029f1ca858777CC98D2D81	Etherscan.io	Xaurum is unit of value on the golden blockchain, it represents an increasing amount of gold and can be exchanged for it by melting\n"
"TIME	ChronoBank (Time Token)		0x6531f133e6DeeBe7F2dcE5A0441aA7ef330B4e53	Etherscan.io	ChronoBank.io is a wide-ranging blockchain project, aimed at disrupting the HR/recruitment/finance industries\n"
"SAN	SANtiment Network Token		0x7C5A0CE9267ED19B22F8cae653F198e3E8daf098	Etherscan.io	A Better Way to Trade Crypto-Markets - Market Datafeeds, Newswires, and Crowd Sentiment Insights for the Blockchain World\n"
"NET	Nimiq Network Token		0xcfb98637bcae43C13323EAa1731cED2B716962fD	Etherscan.io	NimiqNetwork World's first Browser-based Blockchain & Ecosystem\n"
"GUP	Matchpool (Guppy)		0xf7B098298f7C69Fc14610bf71d5e02c60792894C	Etherscan.io	Matchpool is a platform that creates human connections\n"
"SWT	Swarm City Token		0xB9e7F8568e08d5659f5D29C4997173d84CdF2607	Etherscan.io	Swarm City is a decentralized peer to peer sharing economy\n"
"BCAP	Blockchain Capital		0xFf3519eeeEA3e76F1F699CCcE5E23ee0bdDa41aC	Etherscan.io	Blockchain Capital is a pioneer and the premier venture capital firm investing in Blockchain enabled technology\n"
"PLU	Pluton				0xD8912C10681D8B21Fd3742244f44658dBA12264E	Etherscan.io	With Plutus Tap & Pay, you can pay at any NFC-enabled merchant\n"
"EEAR	Eth Early Adoption Registry		0x713b73c3994442b533e6a083ec968e40606810ec	quickBlocks.io	An early, known-dead address useful for testing\n";
