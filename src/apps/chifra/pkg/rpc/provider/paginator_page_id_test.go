package provider

import "testing"

func TestPageIdPaginator_NextPage(t *testing.T) {
	paginator := NewPageIdPaginator("", "latest", 10)
	var err error

	err = paginator.NextPage()
	if err != nil {
		t.Fatal(err)
	}
	if paginator.page != "latest" {
		t.Fatal("wrong page after initialization:", paginator.page)
	}
	_ = paginator.SetNextPage("next")

	err = paginator.NextPage()
	if err != nil {
		t.Fatal(err)
	}
	if paginator.page != "next" {
		t.Fatal("wrong page", paginator.page)
	}
}

func TestPageIdPaginator_Done(t *testing.T) {
	paginator := NewPageIdPaginator("", "", 10)
	paginator.done = true

	err := paginator.NextPage()
	if err == nil {
		t.Fatal("expected error")
	}
	if err != ErrPaginatorDone {
		t.Fatal("expected ErrPaginatorDone")
	}
}
