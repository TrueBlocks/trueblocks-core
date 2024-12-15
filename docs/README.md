# TrueBlocks Docs

[![Deploy Hugo Site](https://github.com/TrueBlocks/trueblocks-docs/actions/workflows/deploy_website.yaml/badge.svg)](https://github.com/TrueBlocks/trueblocks-docs/actions/workflows/deploy_website.yaml)

Repo for the [TrueBlocks.io website](https://trueblocks.io).

Note: There is only a `main` branch which is protected. Make any PRs against this branch and we will review and merge if approriate.

## Contributing to the docs

To develop on this site, you need `yarn` and `hugo`.

To run locally, clone the repo then:

```[shell]
yarn install
hugo
hugo serve --ignoreCache --disableFastRender
```

Open your browser to [the local copy](http://localhost:1313).

## Important directories

- The homepage is in `layouts/index.html`.
- Content templates are stored in `archetypes`.
- `layouts` contains all information about templates.
- `config/_default/menus.toml` contains the structure of the site.

## How to add menu items

Open up `config/_default/menus.toml`.

### Main sections

```TOML
[[main]]
name = "NewSection"
url = "/newsection/"
weight = 10
```

The more "weight", the farther to the left.

You must also add a corresponding file in `layouts` for your new section.

## Adding new pages

Add you pages to the `content` folder:

- if it's the first page in a subfolder, call it `_index.md`.
- add subpages to the same directory.

The new page uses the default layout. If you need to use "list" mode, add `layout: single` in the front matter.

If you need a different layout for the pages, create a new directory in `layouts` using the same name as in `content` folder.

## Contributing

We love contributors. Please see information about our [work flow](https://github.com/TrueBlocks/trueblocks-core/blob/develop/docs/BRANCHING.md) before proceeding.

1. Fork this repository into your own repo.
2. From the `main` branch, create a new branch: `git checkout -b <branch_name>`.
3. Make changes to your local branch and commit them to your forked repo: `git commit -m '<commit_message>'`
4. Push back to the original branch: `git push origin TrueBlocks/trueblocks-docs`
5. Create the pull request.

## Contact

If you have questions, comments, or complaints, please join the discussion on our discord server which is [linked from our website](https://trueblocks.io).

## List of Contributors

Thanks to the following people who have contributed to this project:

- [tjayrush](https://github.com/tjayrush)
- [MattDodsonEnglish](https://github.com/MattDodsonEnglish)
