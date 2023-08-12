#pragma once
#include <null-sdk.h>

namespace null::rml {
	struct i_file_loader {
	public:
		struct i_file {
		public:
			std::string path{ };

		public:
			i_file(std::string_view _path) : path{ _path } { }
			virtual ~i_file() { }

		public:
			virtual void open() = 0;
			virtual void close() = 0;

			virtual size_t read(void* buffer, size_t size) = 0;
			virtual bool seek(long offset, int origin) = 0;
			virtual size_t tell() const = 0;
		};

	public:
		static inline std::unordered_map<std::string, std::unique_ptr<i_file>> opened_files{ };

	public:
		virtual bool create(std::string& path) = 0;
	};
}