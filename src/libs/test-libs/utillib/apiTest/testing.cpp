/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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

#if 1
//--------------------------------------------------------------
int main(int argc, const char * argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    //while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

//        while (!options.fileName.empty()) {
//            SFString file = nextTokenClear(options.fileName, '|');
//            cout << file << "\n";
//        }
    }

    return 0;
}

#else
//--------------------------------------------------------------------------------------------------------
// Include for GTEST library
#include <gtest/gtest.h>
// Code we want to test - we include the options.h header file so we can define here tests for this class
#include "options.h"
// IMPORTANT: To test a class we need to define a wrapper of the class we want to test
// In this example we want to test COptions so we define a COptionsTest class
class COptionsTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
	    // Common initialization code done always we run a test
        }

        virtual void TearDown() {
	    // Code here will be called immediately after each test
            // (right before the destructor).
	}
};
// At TEST functions we need to use TEST_F, we define a namespace and then inside each namespace, the name of the test
// These examples define the SquareRootTest and inside it the PositiveNos and NegativeNos tests
TEST_F(COptionsTest, WhatEverYouWant1) {
   COptions options;
   // do whatever you want here and then test an options method
   // code to do initializations, define variables
   // At ASSERT we can then compare the output of the member function selected with the value expected
    cerr << "Running test WhatEverYouWant1\n";
   ASSERT_EQ(6, 6);
}
TEST_F(COptionsTest, WhatEverYouWant2) {
    // The same here, define an object, do things and test result of member function
    cerr << "Running test WhatEverYouWant2\n";
    ASSERT_EQ(-1.0, -1.0);
}
// Initialize the library and run all the tests defined
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//--------------------------------------------------------------------------------------------------------
#endif // OLD_THOMAS_CODE
