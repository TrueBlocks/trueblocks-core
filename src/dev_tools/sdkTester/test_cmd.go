package main

func (tr *Runner) RunCmdTest(t *TestCase) (bool, bool, error) {
	tr.AppendLog(t)
	return true, true, nil
}
