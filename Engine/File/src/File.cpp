

#include "File.h"
#include "md5.h"

namespace Azul
{

	bool File::IsHandleValid(File::Handle fh) noexcept
	{
		DWORD flags;
		return GetHandleInformation(fh, &flags);

	}

	File::Error File::Hash(unsigned char* buffer, unsigned int buffLen, unsigned int& out)
	{

		if (buffer == nullptr || buffLen < 1u) {
			return File::Error::HASH_FAIL;
		}
		MD5Output hash;
		MD5Buffer(buffer, buffLen, hash);

		out = hash.dWord_0 ^ hash.dWord_1 ^ hash.dWord_2 ^ hash.dWord_3;

		return File::Error::SUCCESS;
	}

	File::Error File::GetDirNames(const char* path, FileName*& FileArrayOut, const char* ext, unsigned int& FileCount)
	{
		//get first dir entry
		WIN32_FIND_DATA DirIt;
		File::Handle status = FindFirstFile(path, &DirIt);
		FileCount = 0;
		unsigned int longestStr = 0;

		//walk through dir and count entries
		if (status != INVALID_HANDLE_VALUE) {

			while (FindNextFile(status, &DirIt) != 0) {

				size_t FileNameLength = strlen(DirIt.cFileName);

				//check extension
				if (FileNameLength > 4 &&
					DirIt.cFileName[FileNameLength - 4] == ext[0] &&
					DirIt.cFileName[FileNameLength - 3] == ext[1] &&
					DirIt.cFileName[FileNameLength - 2] == ext[2] &&
					DirIt.cFileName[FileNameLength - 1] == ext[3])
				{
					FileCount++;
					unsigned int strLen = strlen(DirIt.cFileName);
					if (strLen > longestStr) {
						longestStr = strLen;
					}
				}
			}
			FindClose(status);

			//printf("\nFile Count: %d\n", FileCount);
		}
		else {
			return File::Error::DIR_FAIL;
		}

		//walk through dir and add to array
		FileArrayOut = new File::FileName[FileCount];
		status = FindFirstFile(path, &DirIt);

		if (status != INVALID_HANDLE_VALUE) {

			int i = 0;

			while (FindNextFile(status, &DirIt) != 0) {

				size_t FileNameLength = strlen(DirIt.cFileName);

				//check extension
				if (FileNameLength > 4 &&
					DirIt.cFileName[FileNameLength - 4] == ext[0] &&
					DirIt.cFileName[FileNameLength - 3] == ext[1] &&
					DirIt.cFileName[FileNameLength - 2] == ext[2] &&
					DirIt.cFileName[FileNameLength - 1] == ext[3])
				{
					FileArrayOut[i].name = new char[++longestStr];
					strncpy_s(FileArrayOut[i++].name, longestStr, DirIt.cFileName, strlen(DirIt.cFileName));
				}
			}

			FindClose(status);
		}
		else {
			delete[] FileArrayOut;
			FileArrayOut = nullptr;
			return File::Error::DIR_FAIL;
		}

		return File::Error::SUCCESS;
	}

	//File::Error File::GetDirNames(const char* path, FileName*& FileArrayOut, unsigned int& FileCount)
	//{
	//	//get first dir entry
	//	WIN32_FIND_DATA DirIt;
	//	File::Handle status = FindFirstFile(path, &DirIt);
	//	FileCount = 0;
	//
	//	//walk through dir and count entries
	//	if (status != INVALID_HANDLE_VALUE) {
	//
	//		while (FindNextFile(status, &DirIt) != 0) {
	//
	//			size_t FileNameLength = strlen(DirIt.cFileName);
	//
	//			//check extension
	//			if (FileNameLength > 4 &&
	//				DirIt.cFileName[FileNameLength - 4] == '.')
	//			{
	//				FileCount++;
	//			}
	//		}
	//		FindClose(status);
	//		//printf("\nFile Count: %d\n", FileCount);
	//	}
	//	else {
	//		return File::Error::DIR_FAIL;
	//	}
	//
	//	//walk through dir and add to array
	//	FileArrayOut = new File::FileName[FileCount];
	//	status = FindFirstFile(path, &DirIt);
	//
	//	if (status != INVALID_HANDLE_VALUE) {
	//
	//		int i = 0;
	//
	//		while (FindNextFile(status, &DirIt) != 0) {
	//
	//			size_t FileNameLength = strlen(DirIt.cFileName);
	//
	//			//check extension
	//			if (FileNameLength > 4 &&
	//				DirIt.cFileName[FileNameLength - 4] == '.')
	//			{
	//				//printf("FileName: %s\n", DirIt.cFileName);
	//				FileArrayOut[i++].name = DirIt.cFileName;
	//			}
	//		}
	//
	//		FindClose(status);
	//	}
	//	else {
	//		delete[] FileArrayOut;
	//		FileArrayOut = nullptr;
	//		return File::Error::DIR_FAIL;
	//	}
	//
	//	return File::Error::SUCCESS;
	//}

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

	File::Error File::Size(File::Handle fh, unsigned int& size) noexcept
	{
		//Get current offset of file pointer
		DWORD originalOffset = 0;
		if (File::Tell(fh, originalOffset) != File::Error::SUCCESS) {
			return File::Error::SIZE_FAIL;
		}

		//seek to end of file
		if (File::Seek(fh, File::Location::END, 0) != File::Error::SUCCESS) {
			return File::Error::SIZE_FAIL;
		}

		//get file size via file pointer offset
		DWORD offset = 0;
		if (File::Tell(fh, offset) != File::Error::SUCCESS) {
			return File::Error::SIZE_FAIL;
		}

		size = (unsigned int)offset;

		//move file pointer back to original position
		if (File::Seek(fh, File::Location::BEGIN, (int)originalOffset) != File::Error::SUCCESS) {
			return File::Error::SIZE_FAIL;
		}

		return File::Error::SUCCESS;
	}


}

// ---  End of File ---------------
