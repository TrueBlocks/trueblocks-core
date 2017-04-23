#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "sfdate.h"

//-------------------------------------------------------------------------------
extern SFString getCWD(const SFString& filename=nullString);

//-------------------------------------------------------------------------------
class SFTime;

//-------------------------------------------------------------------------------
class SFos
{
public:
//	static void     fileCreateDate       ( const SFString& filename, SFTime *ret);
//	static SFTime   fileCreateDate       ( const SFString& filename);
	static void     fileLastModifyDate   ( const SFString& filename, SFTime *ret);
	static SFTime   fileLastModifyDate   ( const SFString& filename);
	static SFUint32 fileSize             ( const SFString& file );

	static uint32_t nFiles               ( const SFString& path );
	static void     listFiles            ( uint32_t& nFiles, SFString *files, const SFString& mask );
	static void     listFilesOrFolders   ( uint32_t& nItems, SFString *items, const SFString& mask );
	static uint32_t nFolders             ( const SFString& path );
	static void     listFolders          ( uint32_t& nDirs, SFString *dirs, const SFString& mask);
	static void     copyFolder           ( const SFString& from, const SFString& to );

	static int      removeFolder         ( const SFString& folder);
	static int      removeFile           ( const SFString& file );

	static int      copyFile             ( const SFString& from, const SFString& to );
	static int      copyFile_Special     ( const SFString& from, const SFString& to );
	static int      moveFile             ( const SFString& from, const SFString& to );

	static void     sleep                ( float units );

	static SFString doCommand            (const SFString& cmd);
	static SFString doCommand            (const SFString& tmpPath, const SFString& cmd);
};

//-------------------------------------------------------------------------------
inline bool validPath(const SFString& str)
{
	// No empty paths, path must end with '/', no DOS paths, no double slashes
	return (!str.empty() && str.endsWith('/') && !str.Contains("\\") && !str.Contains("//"));
}

//-------------------------------------------------------------------------------
inline uint32_t nFilesInFolder(const SFString& mask)
{
	uint32_t nFiles = 0;
	SFos::listFiles(nFiles, NULL, mask);
	return nFiles;
}

//-------------------------------------------------------------------------------------------------------------
inline bool fileExists(const SFString& file)
{
	struct stat statBuf;
	return !file.empty() && stat((const char *)file, &statBuf) == 0;
}

//-------------------------------------------------------------------------------------------------------------
extern bool folderExists   (const SFString& path);
extern bool establishFolder(const SFString& path, SFString& created);
inline bool establishFolder(const SFString& path) { SFString unused; return establishFolder(path,unused); }

//-------------------------------------------------------------------------------------------------------------
inline bool waitForCreate(const SFString& filename)
{
	uint32_t max = 500;

	uint32_t cnt=0;
	while (cnt<max && !fileExists(filename))
		cnt++;

	return fileExists(filename);
}

