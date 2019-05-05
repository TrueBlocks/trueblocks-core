# get the current working branch, commit hash, and timestamp
BRANCH=`git rev-parse --abbrev-ref HEAD`
HASH=`git log -1 --format=%h`
TIMESTAMP=`git log -1 --format=%ct`
echo "branch: " ${BRANCH}
echo "hash: " ${HASH}
echo "timestamp: " ${TIMESTAMP}
cat ./build_scripts/version_git.template | sed 's/-YEAR-/2019/' | sed "s/-BRANCH-/${BRANCH}/" | sed "s/-HASH-/${HASH}/" | sed "s/-TS-/${TIMESTAMP}/"

#-
#ls -l version_git.cpp
#touch version_git.cpp
#echo " THIS IS ME DOING SOME SHIT"
#ls -l version_git.cpp
#$1 $2
#/*-------------------------------------------------------------------------------------------
# * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
# * copyright (c) [YEAR] Great Hill Corporation (http://greathill.com)
# *
# * This program is free software: you may redistribute it and/or modify it under the terms
# * of the GNU General Public License as published by the Free Software Foundation, either
# * version 3 of the License, or (at your option) any later version. This program is
# * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
# * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# * General Public License for more details. You should have received a copy of the GNU General
# * Public License along with this program. If not, see http://www.gnu.org/licenses/.
# *-------------------------------------------------------------------------------------------*/
##include "basetypes.h"
##include "version.h"
#
#namespace qblocks {
#
#//    string_q GIT_COMMIT_BRANCH = "[BRANCH]";
#    string_q GIT_COMMIT_HASH = "[HASH]";
#    timestamp_t GIT_COMMIT_TS = [TS];
#
#}  // namespace qblocks
#
