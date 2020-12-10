
#ifndef FILE_H
#define FILE_H

namespace Azul {
	class File
	{
	public:

		class FileName {

		public:
			FileName() :name(nullptr) {}
			~FileName() {
				if (this->name != nullptr) {
					delete this->name;
				}
			}
			FileName(const FileName&) = delete;
			FileName& operator = (const FileName&) = delete;

			//data
			char* name;
		};

		typedef void* Handle;

		enum class Mode
		{
			READ = 0x7A000000,
			WRITE,
			READ_WRITE
		};

		enum class Location
		{
			BEGIN = 0x7B000000,
			CURRENT,
			END
		};

		enum class Error
		{
			SUCCESS = 0x7C000000,
			OPEN_FAIL,
			CLOSE_FAIL,
			WRITE_FAIL,
			READ_FAIL,
			SEEK_FAIL,
			TELL_FAIL,
			FLUSH_FAIL,

			HASH_FAIL,
			SIZE_FAIL,
			DIR_FAIL,

			UNDEFINED
		};

	public:
		static File::Error Open(File::Handle& fh, const char* const fileName, File::Mode mode) noexcept;
		static File::Error Close(File::Handle fh) noexcept;
		static File::Error Write(File::Handle fh, const void* const buffer, const DWORD inSize) noexcept;
		static File::Error Read(File::Handle fh, void* const _buffer, const DWORD _size) noexcept;
		static File::Error Seek(File::Handle fh, File::Location location, int offset) noexcept;
		static File::Error Tell(File::Handle fh, DWORD& offset) noexcept;
		static File::Error Flush(File::Handle fh) noexcept;
		static File::Error Size(File::Handle fh, unsigned int& size) noexcept;
		static bool IsHandleValid(File::Handle fh) noexcept;

		static File::Error Hash(unsigned char* buffer, unsigned int buffLen, unsigned int& out);
		static File::Error GetDirNames(const char* path, FileName*& FileArrayOut, const char* ext, unsigned int& FileCount);
		//static File::Error GetDirNames(const char* path, FileName*& FileArrayOut, unsigned int& FileCount);

	};
}
#endif

// ---  End of File ---------------
