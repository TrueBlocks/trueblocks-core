/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern void doTheTests(void);
extern void doBiggerTests(void);
//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    if (isTestMode()) {
        doTheTests();
        doBiggerTests();
    } else {
        for (auto command : options.commandLines) {
            if (!options.parseArguments(command))
                return 0;
            string_q in = argv[1];
            cout << "in: " << in << "\n";
            string_q hex = chr_2_HexStr(in);
            cout << "hex: " << hex << "\n";
            string_q out = keccak256(hex);
            cout << "out: " << out << "\n";
        }
    }
    return 0;
}

struct s1 {
    const char* const test;
    const char* const expected;
};

s1 xxx[] = {
    {"0x68656c6c6f20776f726c64", "0x47173285a8d7341e5e972fc677286384f802f8ef42a5ec5f03bbfa254cb01fad"},
    {"", "0xc5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470"},
    {"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"
     " ",
     "0x4fdc431ca3869f7a79a3a43acbb0019c769c667ad29fb36896209969b8550af7"},
    {"The quick brown fox jumps over the lazy dog",
     "0x4d741b6f1eb29cb2a9b9911c82f56fa8d73b04959d3d9d222895df6c0b28aa15"},
    {"The quick brown fox jumps over the lazy dog.",
     "0x578951e24efd62a3d63a86f7cd19aaa53c898fe287d2552133220370240b572d"},
};

bool debug = true;
#define TEST(AA, BB)                                                                                                   \
    if (debug) {                                                                                                       \
        cout << "A: " << AA << endl;                                                                                   \
    }                                                                                                                  \
    if (debug) {                                                                                                       \
        cout << "B: " << BB << endl;                                                                                   \
    }                                                                                                                  \
    cout << "diff: " << substitute(AA, BB, "") << endl;

//--------------------------------------------------------------
void doTheTests(void) {
    for (size_t i = 0; i < 1; i++) {
        for (auto& t : xxx) {
            string_q test = string_q(t.test);
            cout << test << endl;
            cout << chr_2_HexStr(test) << endl;
            TEST(keccak256(test), t.expected);
            TEST(keccak256(chr_2_HexStr(test)), t.expected);
        }
    }
}

//--------------------------------------------------------------
const char* test_text =
    "As Estha stirred the thick jam he thought Two Thoughts and the Two Thoughts he thought were "
    "these: a) Anything can happen to anyone. and b) It is best to be prepared.";

struct keccak_test_case {
    const size_t input_size;
    const char* const expected_hash256;
};

keccak_test_case test_cases[] = {
    {0, "0xc5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470"},
    {1, "0x03783fac2efed8fbc9ad443e592ee30e61d65f471140c10ca155e937b435b760"},
    {2, "0x8f9d2f3ed6ac64e8772bbb33bf0d0830e39c0d587064f29acd10a298bb540f6b"},
    {3, "0x275613398ce7fd7cb405a72511be5fee893fd6ea8fdc8d5b162ab5427a800c03"},
    {4, "0x010d5029011787fe9f9eaab2ceaf8ff50f2baffcef2a43c0a869ee422c7eaead"},
    {5, "0x817a5511bc0c271b37c00f64f90ddca328a04b15046cd0c755119991c1ab985f"},
    {6, "0x4a5c773d1f7e9ee199319c7dbe00a14b9a0a37f7bf36848e7612b84db808ce32"},
    {7, "0xe227500b366e01498e7d3cd255c218d5e5f476cbc39545a6d24e8ff9a8aefba3"},
    {8, "0x3199aa06ffa932894093e0c91eb2217d7e8d8250b8cc427104bd0d1954cc1fc2"},
    {9, "0x71af15db68a9c46d404979e3138515e5268a30daa82a0ae5903679e445dfe2a1"},
    {10, "0x3ad41ce4561a235f91c247beb6bbd078bd56b549fbee877a1b7412d3623aa596"},
    {11, "0xfd48650e368a8b90ae2ffdbb6a0b7986afa885de4abb1fcf73ddbd40d7e080f7"},
    {12, "0x3902a8482d98ab34ccd656009c5571cac0f9021ac9949732701b57e305d526c1"},
    {13, "0xd18395db4b8fbe43f9154f3b6cd2bfbb27565c201a944bb8d6a6cc38ac5d1daf"},
    {14, "0x53a32859785d569119cccb7c4a22a04e62f6bb9ffe9495a8b31c348d82722c19"},
    {15, "0xc7bceb7f9f4cd02610d1cb03838cc614d39b8fb1c42a34a80fc877a168ac96da"},
    {16, "0xbe165de8bc0b514e1fcad3ab171ab73d05b7a8db20c0572f86165efd5122c6ce"},
    {17, "0xe1e353f8f5ac4e641403f6f13350e509e8c1083fec714acc1df98afd76f13676"},
    {18, "0x2856656c4dbc71d87938e06604d076f58368839d1d9bf1a710b1c698e1becb88"},
    {19, "0x50edca5e811d8bcc59ba1a135774bc400f1721776b546fca7dc942e04539a90b"},
    {20, "0xee42804f5ed6b1aa8ebc32872289a90938f782e772b3c3be1912ba2be3bf5355"},
    {21, "0x44bc5fe4c92594f234b9dbbf9035ea9ddc6596f60b6f2b30a30358ccd0dba1b6"},
    {22, "0xc422a79d35560ec6a850fa69315aa1d418ec8ee85c570d68c576d6af3211ce35"},
    {23, "0x3f3ccf45b7e205a4c281d75ef1a5dbe0fe8d6e39c6428daa4c6983713160464c"},
    {24, "0xecf999704a93a39758966fdd4f797c6b4819ffc0fe6accf9acf02f7e482b0a34"},
    {25, "0xb5b67366d21d0b8b1b875ad82e0d0bd473e47e5daa49a19b1342a3f550e93d7d"},
    {26, "0x90ba495c62b44ef397bae3b3ac22be808020f6c4d2f6149cdfab31b04ac5dbb4"},
    {27, "0x0df6e5c95ab3ce26094752f14712b89c30030bf2a4c5ad5b9fffa50acbe20d4f"},
    {28, "0x0946571d65c1f802ad95ca1fc1ebfdbe0a8a5392ebe64e2d0bb5345cc8392f20"},
    {29, "0x942ca300fd42678f5e885d06a3d274041868d08042b1424bebb1adfa06135456"},
    {30, "0x52f875475248e9e43333a350228616e5c99ad2e577419ac944010bf32708c7da"},
    {31, "0xd0d044d7361913fed7e7035712306c6912d969d72d6dacfe516d44903c6b59f5"},
    {32, "0xed6ebf3c52ce4ba6689a7ed19ad29d0d9e2e057d9509e5daa5a3c148ae14cdd4"},
    {33, "0x3a23e7b6985b0b0f85cf54f0921095b178cd1c2e6d149106931e2cb98229f0d2"},
    {34, "0x8275a44d7a278f6b1dec2ef08f3d585b690997f1c7da44eda6823bb0d6bc4faf"},
    {35, "0xcedb1f8c428aa4303a59706798b2d9ecb074ee8e689137eb0737092eb8787afc"},
    {36, "0xc6cd5b90617121be1f789fde58afc67c48071d8dd5ffb740da61f95c4ffcc007"},
    {37, "0x3fc949fe7c4219425179ab8e8aff0ae923bf2e624e386f80881f7546cbb70cce"},
    {38, "0x2df5380528db739ca600721ab9b9f73629eddc312410d9dea1065cbff727838f"},
    {39, "0x99508ec2ddf83e6b8cb206b45993b7d2234ad897452da89610337713103785ce"},
    {40, "0xa09ebed7442d2dbdab501ae7136aa11f9c38f957fe037d1431d296554a2458fd"},
    {41, "0xed133e893a543acbf2b41f97b26765edd18d9ba847f5f1f98576bd05f9186aa5"},
    {42, "0xfb723e50d82b97f2f2e25930598c166b9c271f0529e8dee492ff9831758df6fd"},
    {43, "0x6aa444cb7ac3702805ccd871fe172c61a14d598463a45ff4f92f9f0edab4393e"},
    {44, "0x41a6df7c4b7b1fe75f0de23b3d82e48a9291f0bec6ff638f5dbffca38747ad43"},
    {45, "0x8a2f5d9a933e529ca284554057c83db8fcd5c278a650f0b31affdbed9c897587"},
    {46, "0x102dac44ec3a2dcc49026d6ec3560ea763605f5d8e413af4ecf65d54a72d1992"},
    {47, "0x3e1b7c0a882f589a998cedbb7f425c668c3225791b7344b57ca1a215dbdba1ec"},
    {48, "0x0f6baa5e4129bf4796eb46840dba51d7086fb5f7d27c0f2c956d7688f6eca80c"},
    {49, "0x52be9251251038393f870e321fa69d4d898e1475a08fb9d3954295e6d88e2606"},
    {50, "0x111cbe35de34321ec32e71b4b6c8fcc0de078b2c89a722153d04f7bf0af3e255"},
    {51, "0xa6d02ef82af5747437e12a8860dfc6c2f45534f8d36a53012d5dc4e970ad1050"},
    {52, "0x216347823bc50433e9055fe6057ffdd6151d2d25859725e132961bb073ed9f84"},
    {53, "0xf63811edd7f59114cf2679b5e25f4fb0a295f2f57463194b3b2faea88ce2042d"},
    {54, "0xf6377db0f43f983d208f41ce514ff58b1b0a417c196aa0ee0659371a62cb5f66"},
    {55, "0xfdae3a84eed58531f9833336a1fffa28f4c7cef68b1b6241422da6c212feb8ee"},
    {56, "0x64fefc464a771a37f22abe02dbc37bd42054f971e0f4ea44875c5974c20b6dfc"},
    {57, "0xd6f8da97620091de1bc7bae7c633846d06fef3ca9b716c5fbff812a1309c9172"},
    {58, "0x48e48c588912bb2d87819a2ae99430ec56b4523370715bba34ba75d0001c2a15"},
    {59, "0x030fbdca12cce80a3589ad56c733523baa258184429b620e19f40cc03ea7c622"},
    {60, "0xd1f1edaf2fd8699242ba6888cf4d523f9f37f5497152d46055970bc923d3c19c"},
    {61, "0x95ad70e63a5ce2ef49ce85a92f9ed244fbe62fce07aa99dea6a3d9ed2a579448"},
    {62, "0xab777e042d90ba746df35569ebb8445d2923a979faccbfbd33485df8c4ea0e75"},
    {63, "0xeccd3de4223b962451bcbcb2562b8393fee5da8ac6631dbbfab2e512652f6388"},
    {64, "0x8ffaed16298a9e6a35d97f987af34816486d33a5cdcac6bf09e4e038c39709e2"},
    {65, "0xbab06db94626843434cd7f6bb509c3b04159cef3a6be97840dac86818ba0590a"},
    {66, "0x8b29319b6ad4fe70e7c2db7ba1d1dd03b660d2232365e59a33d8c0349647f551"},
    {67, "0x1e29e6294ea3dad162c4f0d46c0be1e5ccb2c0d9b980a65b3a85e1619ffafdf0"},
    {68, "0x65138bd629c696eba60603648e11aa986c371ccf3c611770ed566740964869ac"},
    {69, "0x15b4dba681e98ca4b88d6d63c89ddbd1d368659ca785569696fc1f016de35a5c"},
    {70, "0x9519723488a7d2495d881ca675f8f7f3f917d0686ed71c4ae24b35960c9a1542"},
    {71, "0x7a68fd970bf9abc276321d8c8b1722abda92fcfc638ca425233cb658dd1217e6"},
    {72, "0xe5cab870fabb70f7846ac18d26b81ccfe7c2445ab9ee4a8f28f0b8d451b48863"},
    {73, "0xaf2200dbffdb48df3f930ff3f859a9b1ee6eeb009d0032ed0806b5c8c0e24eb1"},
    {74, "0x8d0e32efac6c14aff3154616d1f20569c8f5666fbdd1184b01658e378b311872"},
    {75, "0xcb735fd9d10537e0c7031bbca5b3dd6f4da94e81518fa483be6773ab3c60492a"},
    {76, "0x755d99523d5807b43b415c755a93863b2cf2d39a81a56e353f0d79b9d00a48f4"},
    {77, "0xcae59f7cc049fbef14b1c76fbb6edfbb645f728b8e5fe2973ce6709b1936368a"},
    {78, "0x5acbc5fd68cffd1e5b6bb8d83bc9c765e437c13ef7cc775de7ae338ad6241763"},
    {79, "0x600375f016ba76a9515eff01073e2edcc5c9701418b74b1f70dd54a77c11fefd"},
    {80, "0xd23960d07ebafd56f94a7a3c1b0766c45fc69d37446328a461d4f79b3760664f"},
    {81, "0x3ef3de18461824fcf3bfbb6b33dc461a856af105feefc417a31fc1d94c274120"},
    {82, "0x1f1d2c368e26a48aad6b6b88f7ea8bbc135f5009bfa6a7ae9d34d66535d90e46"},
    {83, "0x7e60eb4ec80eb892eb00156c3fabcee77aa32246c380c2de9f936ad6ba6e4e60"},
    {84, "0x7b9353f2551aa25e79bdcdfd5fba62c9f3a4d554883811447cfdacc38ea1ad78"},
    {85, "0x9f1b17eaefdc3cdf3dc8e01a2b0f4721fd2fbabc245dcd8fc93a009db9fadf82"},
    {86, "0xfc0ff24e8e0a505eb8d73a67a42ae58bbd787e865cfdec790c41e43aa76b086e"},
    {87, "0xa8d0493422757365283899e5273a6288d9fe9c5b39901a997eb9e5dd7cc0c936"},
    {88, "0x7c24d420d27fbe26286b5417debdd83fac126242218f366320d4d29f9e2fd0c1"},
    {89, "0x553d6597c8d872e3c004958a5dc11b75f8b8966e2346a51350b550af02ad9668"},
    {90, "0xd83907301da88e935e527664d78db36ea653aeb3328ef5384b185b8f44db272e"},
    {91, "0x5ca6551822724586fde5450760bf9d09a04311b19f5f085fcf2bb94aeb43a9be"},
    {92, "0x616dfe2fa664bc05785645d53de24cd03c71f52238cfae6bee9ebd557f39c81e"},
    {93, "0x5b2496c6f379d0dc027d166f38cac8fa905609131c4a498baf6ad9111c0a3a4d"},
    {94, "0x8d9b43e836e2a0705743612c7eb500d5672559d67e705dc88fabf89cb0d3cc40"},
    {95, "0x7fe320d7443ef4c23def64f983500bb83e4da8392866d7e7db55e10f102dd36a"},
    {96, "0x93029c254ce6a989516fed215a3b7c5af8915e8986f4ff55ab9e3231bfe67660"},
    {97, "0x5fe835b148f7973f434f694dbb0df42a52a03146d8fa1c8eac6ab2b9282aa138"},
    {98, "0x1c260bb47f463dc81d7e7bd10e73e2786d84adde6b5438fb11d589aad3ed1b37"},
    {99, "0x96e4c7d80bbd28248605bbe0277b37b9f3fec565e964d348071fe2f6fad5042e"},
    {100, "0x8f3cee69b22346788a562bae56732c5e5bb846b8b5126c6361697aec6af0bb86"},
    {101, "0x8c9104c1ef7224ead9690bbc9188f3194782c040b6ce4f8931b7176a82736fe8"},
    {102, "0x5df9756bd08f6d278e80e4ea2663833ba80d68627f390816e6b3f969e34ff7c6"},
    {103, "0x72ded758c4e92c680905e96db06d522ec961f2b4c810e4866150b9143a4df0ed"},
    {104, "0xe28b468ce82d4cfbb9b6e976e52ce48a7197dd98e3f87c5d617105a73d6eff98"},
    {105, "0x3745d07216de01be912245dc25a434f8a009c745c7d118af6fadb7c1ce262812"},
    {106, "0xd6bdcd1aaa6b6bddf4f477b9f4b62b16664aa81cc495ae5b79720bcafadc0df2"},
    {107, "0x4c77711c85c94ac6dab5ab4e6f2cdd6b2a39229255b1597a54e46f6f451b65bb"},
    {108, "0xe82492039b214c9d1672c30b2caf08c93fdb8abe7e95bcfa80eff68d500431c0"},
    {109, "0x048ec1578f785435d1f633eb06a605dde14ad259f66c35f2be20233f1a2cc066"},
    {110, "0xdb3c3365ea062b6cfb7b187a53a5c9178fa9f1b3beaced1caeb46aa4b7a0b79c"},
    {111, "0x2370e4167c6d9ae57c6fd963a7d0ecc4cb7d45186441378da38696da7e97e92d"},
    {112, "0xdce4bdb9dc07d44ebbf17b8639c063c751018eabf503329533da6cef6c376229"},
    {113, "0x4f6bfef2006307a99045773db57cf1d4e087248f32ced56eb878bfdeb6305d39"},
    {114, "0x644bd1cc20f0f8b841792ca80782a6f347bf1e805ab4d483d4716e2fe66e0d31"},
    {115, "0xeda21853c9a8fdc2b3fa98b36a58b57f4e4c710ff1d032a9b251977683adfc59"},
    {116, "0x42728c7746f3d062c24169d1af85a6d074a2978484f9b107bb3bb653f0d306ab"},
    {117, "0xdd7084bd4e9e76ba68b67e9a4452532759338f5b025b01ac3327ce97d001b398"},
    {118, "0x4e2beaf87c3582f365109a880246ef864a34a5a129e236540cce03659d96bbaa"},
    {119, "0xf1c951a77a8de59e1d66b28c2d5c90aaadfdf35f79fbeee55d67d8c03ac5c751"},
    {120, "0x74064b16aa4ce7aa8e2d95b26414c212d03fd1d8c71b620a2a4791302ce55ed3"},
    {121, "0x73d01fb15eb444a9c0b0d76955bfafc8bcc10b7b62a90ac87330db00bdeff7cc"},
    {122, "0xd5540b7d0fed68efc94f09c61f419beeb62bf5775247c863d94123ff673fa20b"},
    {123, "0x28bf8460791da731a208ff60d1a38404d8c2951b01a4f75c23e09ec65176e73b"},
    {124, "0x0aa12ebc808b8288bfc7616c547cd77e71248bad62db27bfef57bbabbe5014a4"},
    {125, "0x80957c683cc5362ea3acb198a49f610f21fd26aebae94973743455cd7a26f77d"},
    {126, "0x3357ec88b06ebc229bd420f9465243441407d10db0ec36a213ebf529d37b1ccb"},
    {127, "0xef54c7ff8d900b3e5ffe2ef12ebfa3bd6a0ab1caf18ca8742d8870ca2237d77a"},
    {128, "0x3c85ace511a98a57eb88332ca2413d851051286dfca6fccb8435bff882eee96c"},
    {129, "0x743c831d629e42f2d62c61a143148f9059d417b945a6362a916d1683baf86849"},
    {130, "0x09f5f1a743e2dc12a61a6265476ba80f6f3ac99c684ce182be720f704f1bee4e"},
    {131, "0x78c7c425c1286044a19aa5d2905a72fce1a456de93bdc10c93ec8dfa11f6e533"},
    {132, "0xa55347ca336bc0ddf62455b7d1a5af1b34044b85181eec70f29511a97bd84ddf"},
    {133, "0xf19ce00ea82acae77572315e6e67bf03feaddb407b47a6eee6c29feaaa94cbd6"},
    {134, "0x805fb87c6a9ae05dfa11bd901ef2e49d1b7e9ba1e1c8a172df98631b440ee0b0"},
    {135, "0x529d56f1797de6a688c2a3dda2138f0895c964b2fbd61ba24b9278a06ef90aa0"},
    {136, "0x2822ac036b9bc5d0c920d9e7f0abd6d6036701b513d2439104199fd6cd6d4ea4"},
    {137, "0x63b20400faf44a584ef6da89d234ee45437b8a23b740300c373d50ec0a03c171"},
    {138, "0x217ab71db1601700e4079adfbec1f9d3656b557df6fab4cf8351a617bdf6c3d5"},
    {139, "0xb34a1013259fdbe6568859fc72b82be660bd8fcad13140845e487d63c1575719"},
    {140, "0x4993e455f007d8954cbb110634618094e2d2f07aabb9460151a364718eca89f1"},
    {141, "0x3fb96e6f4e52b641e4fea88641d8a52a8b58eff80e76c2b10fc186412afd4eab"},
    {142, "0x1a1d8b791aad01f478b8053815a3e62ed95c5d26fbfce9a7f75a97127bd2c4c6"},
    {143, "0xfd2aeec83853dcd84b93d26d4a706c234cc176b69df8946931bf088e1ef981ad"},
    {144, "0x9d2d8ba34b055d23ebab3d1470c8a8b8ff6c8bea5e7d6570b68817bcc2cf40d8"},
    {145, "0xa7ae00d76af18d62e17d94bf97a6b961a4a203993f8ade970e3ccc5ba6a60c2e"},
    {146, "0xbb0bf557bd1d73940dc98ed9160c3fe1a63fbb53746f1c32000f6ae0427bd854"},
    {147, "0x69d5b06c803118c3bcfcd3990af4b420992f7561f9a17230e7d14198633646b9"},
    {148, "0x7b163784327e37b5ebb13d8b65c00e4865faf8bc76c52198c1f31fc2fcef9794"},
    {149, "0x16083d110706dd3b08a0c3cb1cdc82c8cb1da7ec6f038c175281eb246c5cce46"},
    {150, "0x407149ecd0c917b3660731fd97cd8957c6a437e878d3d19b3433e1b1ba71276e"},
    {151, "0x5f40c1b426341c89de69577bc918d1189350753fce25331602fb47c080b8cdcd"},
    {152, "0x989e0478a3f9154a6a8a55d616ce8057f97e93bfab618e25c29c5a8dceddd74c"},
    {153, "0xfb7d60e9612932cb41e02cd15b573ef61483ba7c0dd640aa7cdbec6acb03b617"},
    {154, "0xf16b6cea1479c6472e266d4816d091b4c0468e310d95025ca42c6f65744e713e"},
    {155, "0xcd717b2514bce6a4a890d5dd1c05606a87e1b2093fb4dd3376ef34fabefe85fd"},
    {156, "0xf229b862959bbf5a188fe389ad0e07aae63e5792fff39b61aeaf89b27566e97a"},
    {157, "0x8282d14db1031e6d6b57623d879916dfaee1c49d4348a10043b51336d5896fc3"},
    {158, "0x5390e03ac0c43fe3923f7530a11d07e6011952ce6c0b02a22b1ce67786ec7b97"},
    {159, "0x5ef1de3ec8bf8d3fe488f64c8689bdb253688e76f10f7eecf9fbf408ffd69684"},
    {160, "0xd8eb154e9cba0fe242aff63224bbfc24fa1e997f3d0a550224094b9d954c9a85"},
    {161, "0xe8eddeabcb48a1604105951bc431474ec3fbeba77f23c585066f8409e4362eb7"},
    {162, "0xe6c27a15203c184fdc6b36100f630ed15ff44e178f8390a02664f034cbd46e7d"},
    {163, "0x73789a930d42244c1179574b0d9df669774e180fb38eb9d3b1b354787a6935d3"},
    {164, "0xc8f5964b0cf9f40470927db763518d9b73136d70933ed265b180c64e71695afe"},
    {165, "0x591377841eeb928c4459736464197bda01a6bf716a911fbd9dceee8f58a8e0eb"},
    {166, "0x7913dce6ae700aa2b21b92eb34afacba68efa6db171dd5384de495bfecb31ed1"},
};

//----------------------------------------------------------------
void doBiggerTests(void) {
    for (size_t i = 0; i < 10; i++) {
        for (auto& t : test_cases) {
            string_q test = string_q(test_text).substr(0, t.input_size);
            string_q h1 = keccak256(test);
            string_q h2 = t.expected_hash256;
            TEST(h1, h2);
        }
    }
}
