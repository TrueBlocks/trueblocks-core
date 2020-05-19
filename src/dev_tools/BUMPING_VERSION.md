## Bumping the version

In `develop` branch:

	git checkout develop                          # go to the develop branch
	git pull                                      # make sure you have the latest code
	edit ../src/libs/utillib/version.cpp          # bump the version number
	make test-all                                 # a number of tests wlil fail but all should only
                                                  # fail due to version change. Resolve all tests.
	git commit -am "Final note"                   # mark the commit as final commit before m erging
	git push                                      # push the develop branch to remote
	
Then (as per the second answer [here](https://stackoverflow.com/questions/14168677/m erge-development-branch-with-master)):

	git log                                        # note the commit hash
	git fetch origin master                        # pull a copy of master branch locally
	git m erge master                               # do not proceed until all m erge conflicts are solved (then, test and push)
	git log                                        # note the latest commit on develop
	git push origin develop:master                 # push the mer ged develop into master
	git checkout master                            # checkout local master
	git pull                                       # pull newly me rged master locally
	git log                                        # should be same commit as above
	
