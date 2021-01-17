# Branching

## Quick Legend

<table>
  <thead>
    <tr>
      <th>Instance</th>
      <th>Branch</th>
      <th>Description, Instructions, Notes</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Master</td>
      <td>master</td>
      <td>Accepts merges from Working and Hotfixes</td>
    </tr>
    <tr>
      <td>Working</td>
      <td>develop</td>
      <td>Accepts merges from Features/Issues and Hotfixes</td>
    </tr>
    <tr>
      <td>Features/Issues</td>
      <td>feature-*</td>
      <td>Always branch off HEAD of Working</td>
    </tr>
    <tr>
      <td>Hotfix</td>
      <td>hotfix-*</td>
      <td>Always branch off Master</td>
    </tr>
  </tbody>
</table>

## Main Branches

The main repository will always hold at least two branches:

* `master`
* `develop`

The active branch should be considered `origin/develop` and will be the branch where the source code of `HEAD` always reflects the latest development changes for the next release. As a developer, you should branch and merge from `develop`.

Consider `origin/master` to represent the latest code deployed to production. During day-to-day development, the `master` branch will not be changed.

When `develop` branch is stable and ready to be deployed, changes will be merged into `master` and tagged with a release number. *How this is done in detail is discussed below.*

## Supporting Branches

Supporting branches are used to aid parallel development between team members, ease tracking of features, and assist in fixing live production problems. Unlike the main branches, these branches have a limited lifetime. Evenutally, usually when the branch is merged, they will be removed.

The different types of branches are:

* Feature branches
* Issue branches
* Hotfix branches

Each of these branches have a specific purpose and are bound to strict rules as to which branch they should originate from and which branch they will be merged into.

### Feature Branches

Feature branches are used when developing a new feature or enhancement. When the feature will be released may not be known. Feature branches will only be merged into the `develop` branch.

Before merging a feature branch, changes to the `develop` branch made subsequent to the feature branch will be merged into the feature branch and any conflicts resolved. This can be done at various times during the development of the feature or at the end, but all merge conflicts must be resolved before merging back into `develop`.

`<tbd number>` represents the project to which Project Management will be tracked.

* Must branch from: `develop`
* Must merge back into: `develop`
* Branch naming convention: `feature-#-<short-descr>`

#### Working with a feature branch

If the your branch is new, create the branch locally and then push to it to the repo. Feature branches should be made 'publicly' available to the entire team.

```
$ git checkout -b <feature-id> develop    // creates a local branch for the new feature
$ git push origin <feature-id>            // makes the new feature remotely available
```

Subsequent changes made to `develop` may be merged with:

```
$ git merge develop                       // merges changes from develop into feature branch
```

When completed, the branch will be merged into `develop` and the feature branch will be deleted.

```
$ git checkout develop                    // change to the develop branch  
$ git merge --no-ff feature-id            // makes sure to create a commit object during merge
$ git push origin develop                 // push merge changes
$ git push origin :feature-id             // deletes the remote branch
```

### Issue Branches

Issue branches differ from feature branches in that issue branches are created only when there's a bug whose fix can be delayed until the next release. Therefore, issue branches typically last no longer than one cycle. No matter when the issue branch is finished, it will always be merged into `develop`.

Although not as necessary as with a more long-lasting feature branch, an issue branch should be kept abreast of `develop`. All changes to `develop` must be merged into the issue branch before merging back to `develop` (as above).

* Must branch from: `develop`
* Must merge back into: `develop`
* Branch naming convention: `issue-#-<short-descr>`

#### Working with a bug branch

See above for instruction on creating, pushing and merging your issue branch.

### Hotfix Branches

Hotfix brances will be handled entire by the lead developer. They are generally pushed directly to `master`, and therefore may complicate merge conflicts.

* Must branch from: tagged `master`
* Must merge back into: `develop` and `master`
* Branch naming convention: `hotfix-<tbd number>`
