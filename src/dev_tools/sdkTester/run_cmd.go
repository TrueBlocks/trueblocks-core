package main

func (tr *Runner) RunCmdTest(t *TestCase) (bool, bool, error) {
	tr.AppendLog(t)
	return true, true, nil
}

func (t *TestCase) CmdTest() (string, error) {
	return "", nil
}
