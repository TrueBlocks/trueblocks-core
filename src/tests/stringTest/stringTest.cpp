/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

#define TEST_STR SFString

//------------------------------------------------------------------------
void testRelational(void)
{
	string foo = "alpha"; TEST_STR xfoo(foo.c_str());
	string bar = "beta";  TEST_STR xbar(bar.c_str());

	if ( foo ==  bar) cout << "foo and bar are equal\n";
	if (xfoo == xbar) cout << "xfoo and xbar are equal\n";

	if ( foo !=  bar) cout << "foo and bar are not equal\n";
	if (xfoo != xbar) cout << "xfoo and xbar are not equal\n";

	if ( foo  <  bar) cout << "foo is less than bar\n";
	if (xfoo  < xbar) cout << "xfoo is less than xbar\n";

	if ( foo  >  bar) cout << "foo is greater than bar\n";
	if (xfoo  > xbar) cout << "xfoo is greater than xbar\n";

	if ( foo <=  bar) cout << "foo is less than or equal to bar\n";
	if (xfoo <= xbar) cout << "xfoo is less than or equal to xbar\n";

	if ( foo >=  bar) cout << "foo is greater than or equal to bar\n";
	if (xfoo >= xbar) cout << "xfoo is greater than or equal to xbar\n";

    cout << (min(foo,bar) == bar ? "bar is min" : "foo is min") << "\n";
    cout << (max(foo,bar) == bar ? "bar is max" : "foo is max") << "\n";
}

//------------------------------------------------------------------------
void testCompare(void)
{
	string str1 ("green apple"); TEST_STR xstr1 ("green apple");
	string str2 ("red apple");   TEST_STR xstr2("red apple");

	if ( str1.compare( str2)                    != 0) cout <<  str1 << " is not " <<  str2 << '\n';
	if (xstr1.compare(xstr2)                    != 0) cout << xstr1 << " is not " << xstr2 << '\n';

	if ( str1.compare(6,5,"apple")              == 0) cout << "still, " <<  str1 << " is an apple\n";
//	if (xstr1.compare(6,5,"apple")              == 0) cout << "still, " << xstr1 << " is an apple\n";

	if ( str2.compare(str2.size()-5,5,"apple")  == 0) cout << "and " <<  str2 << " is also an apple\n";
//	if (xstr2.compare(str2.size()-5,5,"apple")  == 0) cout << "and " << xstr2 << " is also an apple\n";

	if ( str1.compare(6,5, str2,4,5)            == 0) cout << "therefore, both are apples\n";
//	if (xstr1.compare(6,5,xstr2,4,5)            == 0) cout << "therefore, both are apples\n";
}

//------------------------------------------------------------------------
void testCStr(void)
{
	{
		string str ("Please split this sentence into tokens");
		char * cstr = new char [str.length()+1];
		strcpy (cstr, str.c_str());
		char *p = strtok(cstr," ");
		while (p!=0)
		{
			cout << p << '\n';
			p = strtok(NULL," ");
		}
		delete[] cstr;
	}

	TEST_STR str ("Please split this sentence into tokens");
	char * cstr = new char [str.length()+1];
	strcpy (cstr, str.c_str());
	char *p = strtok(cstr," ");
	while (p!=0)
	{
		cout << p << '\n';
		p = strtok(NULL," ");
	}
	delete[] cstr;
}

//------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	testRelational();
	testCompare();
	testCStr();
	return 0;
}
