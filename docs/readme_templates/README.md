## README Templates

Each tool and application has a corresponding template file in this folder. You may edit these templates and then run `make readmes` to regenerate the actual README file in the underlying tool.

Note that `make` will automatically run `make readmes`, so generally, you can simple edit the templates like any source file and simply re-build.

**Note:**

You may not edit the command line options for a tool by editing the README template. Do that by modifying the `cmd_line_options.csv` file and then rebuilding. Generally, this is to be avoided as it will break test cases.
