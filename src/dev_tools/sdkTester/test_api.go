package main

func (tr *Runner) RunApiTest(t *TestCase) (bool, bool, error) {
	tr.AppendLog(t)
	return true, true, nil
}
