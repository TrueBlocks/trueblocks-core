#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

//----------------------------------------------------------------------------------
class CFilename
{
private:
	CFilename(void) {} // cannot create default

	SFString path;
	SFString fileName;

public:
	CFilename(const SFString& fnIn);
	SFString getPath(void) const { return path; }
	SFString getFilename(void) const { return fileName; }
	SFString getFullPath(void) const { return (path+fileName).Substitute("//","/"); }
};

//----------------------------------------------------------------------------------
inline CFilename::CFilename(const SFString& fnIn)
{
	SFString fn = fnIn;
	if (!fn.startsWith('/') && !fn.startsWith('.') && !fn.startsWith('~'))
		fn = "./" + fn; // assume cwd
	fn.Replace("../", getCWD()+"xx/");
	fn.Replace("./",  getCWD());
	fn.Replace("~/",  getHomeFolder());
	fn.Replace("xx/", "../");

	if (fn.endsWith('/'))
	{
		path = fn;
		fileName = EMPTY;

	} else
	{
		path = fn.Left(fn.ReverseFind('/')+1);
		fileName = fn.Substitute(path, EMPTY);
	}
}
