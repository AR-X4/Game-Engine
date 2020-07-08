//--------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "File.h"

namespace Azul
{

	bool File::IsHandleValid(File::Handle fh) noexcept
	{
		DWORD flags;
		return GetHandleInformation(fh, &flags);
		
	}

	File::Error File::Open(File::Handle& fh, const char* const fileName, File::Mode mode) noexcept
	{

		DWORD OpenMode = 0;

		switch (mode) {
		case Mode::READ:
			OpenMode = GENERIC_READ;
			break;
		case Mode::WRITE:
			OpenMode = GENERIC_WRITE;
			break;
		case Mode::READ_WRITE:
			OpenMode = GENERIC_READ | GENERIC_WRITE;
			break;
		}

		fh = CreateFileA(fileName, OpenMode, 0, 0, OPEN_ALWAYS, 0, 0);
		

		if (INVALID_HANDLE_VALUE != fh) {
			return File::Error::SUCCESS;
		}

		return 	File::Error::OPEN_FAIL;
	}

	File::Error File::Close(const File::Handle fh) noexcept
	{
		
		if (File::IsHandleValid(fh) == 0) {
			return File::Error::CLOSE_FAIL;
		}
		
		if (CloseHandle(fh) == 1) {
			return File::Error::SUCCESS;
		}

		return File::Error::CLOSE_FAIL;
	}

	File::Error File::Write(File::Handle fh, const void* const buffer, const DWORD inSize) noexcept
	{

		if (WriteFile(fh, buffer, inSize, 0, 0) == 1) {
			return File::Error::SUCCESS;
		}

		return 	File::Error::WRITE_FAIL;
	}

	File::Error File::Read(File::Handle fh, void* const buffer, const DWORD inSize) noexcept
	{
		
		if (ReadFile(fh, buffer, inSize, 0, 0) == 1) {
			return File::Error::SUCCESS;
		}

		return 	File::Error::READ_FAIL;
	}

	File::Error File::Seek(File::Handle fh, File::Location location, int offset) noexcept
	{
		DWORD SeekLocation = 0;

		switch (location) {
		case File::Location::BEGIN:
			SeekLocation = FILE_BEGIN;
			break;
		case File::Location::CURRENT:
			SeekLocation = FILE_CURRENT;
			break;
		case File::Location::END:
			SeekLocation = FILE_END;
			break;
		}

		if (SetFilePointer(fh, offset, 0, SeekLocation) != INVALID_SET_FILE_POINTER) {
			return File::Error::SUCCESS;
		}

		return 	File::Error::SEEK_FAIL;
	}

	File::Error File::Tell(File::Handle fh, DWORD& offset) noexcept
	{
		offset = SetFilePointer(fh, 0, 0, FILE_CURRENT);

		if (offset != INVALID_SET_FILE_POINTER) {
			return File::Error::SUCCESS;
		}

		return 	File::Error::TELL_FAIL;
	}

	File::Error File::Flush(File::Handle fh) noexcept
	{

		if (FlushFileBuffers(fh) == 1) {
			return File::Error::SUCCESS;
		}

		return 	File::Error::FLUSH_FAIL;
	}

}

// ---  End of File ---------------
