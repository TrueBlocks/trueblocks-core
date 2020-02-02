/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <string>
#include <algorithm>
#include "utillib.h"
#include "testing.h"

//------------------------------------------------------------------------
class CThisTest : public testing::Test {
public:
  CThisTest(void) : Test() {
  }
  virtual void SetUp(void) {
  }
  virtual void TearDown(void) {
  }
};

//------------------------------------------------------------------------
TEST_F(CThisTest, TestYaml) {
  cout << TESTID("place for a yaml test when we have some yamly stuff", 20);
  return true;
}
}

#include "options.h"
//------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
  etherlib_init(quickQuitHandler);

  COptions options;
  if (!options.prepareArguments(argc, argv))
    return 0;

  for (auto command : options.commandLines) {
    if (!options.parseArguments(command))
      return 0;
    LOAD_TEST(TestYaml);
  }

  return testing::RUN_ALL_TESTS();
}
