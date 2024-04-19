package provider

import "testing"

func TestPageNumberPaginator_NextPage(t *testing.T) {
	paginator := NewPageNumberPaginator(1, 1, 10)
	var err error

	err = paginator.NextPage()
	if err != nil {
		t.Fatal(err)
	}
	if paginator.page != 1 {
		t.Fatal("wrong page after initialization:", paginator.page)
	}

	err = paginator.NextPage()
	if err != nil {
		t.Fatal(err)
	}
	if paginator.page != 2 {
		t.Fatal("wrong page", paginator.page)
	}
}

func TestPageNumberPaginator_Done(t *testing.T) {
	paginator := NewPageNumberPaginator(1, 1, 10)
	paginator.done = true

	err := paginator.NextPage()
	if err == nil {
		t.Fatal("expected error")
	}
	if err != ErrPaginatorDone {
		t.Fatal("expected ErrPaginatorDone")
	}
}
