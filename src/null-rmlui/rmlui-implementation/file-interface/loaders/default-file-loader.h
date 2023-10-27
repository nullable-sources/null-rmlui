#pragma once
#include <filesystem>

#include "file-loader.h"

namespace null::rml {
	struct default_file_loader_t : public i_file_loader {
	public:
		struct file_t : public i_file {
		public:
			FILE* file{ };

		public:
			file_t(std::string_view _path) : i_file(_path) { open(); }
			~file_t() { close(); }

		public:
			void open() override { file = fopen(path.data(), "rb"); }
			void close() override { if(file) { fclose(file); file = nullptr; } }

			size_t read(void* buffer, size_t size) override { return fread(buffer, 1, size, file); }
			bool seek(long offset, int origin) override { return fseek(file, offset, origin) == 0; }
			size_t tell() const override { return ftell(file); }
		};
		
		struct i_formatter {
		public:
			virtual std::string format(const std::string& path) = 0;
		};

		struct formatter_t : public i_formatter {
		public:
			std::regex regex{ };
			std::string replace{ };

		public:
			formatter_t(const std::regex& _regex, std::string_view _replace) : regex(_regex), replace(_replace) { }

		public:
			std::string format(const std::string& path) override { return std::regex_replace(path, regex, replace); }
		};

	private:
		std::vector<std::unique_ptr<i_formatter>> formatters{ };

	public:
		default_file_loader_t() { add_formatter(std::regex("\\{system-fonts\\}"), std::format("{}\\fonts", std::getenv("windir"))); }

	public:
		virtual bool create(std::string& path) override {
			for(const auto& formatter : formatters) path = formatter->format(path);
			if(!std::filesystem::exists(path)) return false;
			opened_files[path] = std::make_unique<file_t>(path);
			return true;
		}

		void add_formatter(const std::regex& regex, std::string_view replace) { formatters.push_back(std::make_unique<formatter_t>(regex, replace)); }
	} inline default_file_loader{ };
}