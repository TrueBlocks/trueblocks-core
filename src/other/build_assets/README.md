## tools/scripts

This folder contains various tools and python scripts that provide simple, helpful utilities that aid in debugging and understanding Ethereum blocks, transactions and accounts. 

---
`Usage:`    ethscan.py [ block | hash | accounts ]

`Purpose:`  Opens EtherScan (http://etherscan.com) website to either a given block number, a given block or transaction hash, or a given Ethereum address. You may specify any number of the above in any combination.

---
`Usage:`    ethgoog.py address(es)

`Purpose:`  Searches Google for the given address(es) while eliminating results from blockchain scrapers. This makes it easier to find references to a given Ethereum address if one is searching for information about that address.

---
`Usage:`    pylint.py <source_code_file>

`Purpose:`  We use this source code linter to insure consistent code style. If you add code to the repository, please use this tool before submitting your changes.

---
`Notes:` Three scripts found in this folder (`clearCache.py`, `replaceTag.py`, and `testCheck.py`) support the build process and should not be modified.


####Contents of CMakeLists.txt file:
```
# minimum cmake version supported
cmake_minimum_required (VERSION 2.6)

# script project
project (build_scripts)

# user facing stuff
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/ethgoog.py     DESTINATION ${SCRIPTS_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/ethscan.py     DESTINATION ${SCRIPTS_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)

# various build suport scripts
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/testCheck.py   DESTINATION ${SCRIPTS_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/replaceTag.py  DESTINATION ${SCRIPTS_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/clearCache.py  DESTINATION ${SCRIPTS_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)

# source tree traversing to search, edit, or lint code
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/editAll        DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/findAll        DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/hatchAll       DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/pylint.py      DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/lintAll        DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/clearLocks     DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/rmBlock        DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)

# compare and copy test cases
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/doTesting      DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/doTestingAll   DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/doTestingState DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/diffTest       DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/showTest       DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/cpTest         DESTINATION ${BUILD_DEST_PATH} FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE)

# Installation steps
install(PROGRAMS ethgoog.py    DESTINATION bin)
install(PROGRAMS ethscan.py    DESTINATION bin)
install(PROGRAMS testCheck.py  DESTINATION bin)
install(PROGRAMS replaceTag.py DESTINATION bin)
install(PROGRAMS clearCache.py DESTINATION bin)
```

**Powered by Qblocks<sup>&trade;</sup>**

