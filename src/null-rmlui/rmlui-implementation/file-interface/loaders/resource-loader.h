#pragma once
#include <regex>

#include "file-loader.h"

namespace null::rml {
    struct resource_loader_t : public i_file_loader {
    public:
        struct file_t : public i_file {
        public:
            std::string file{ };
            size_t read_position{ };

        public:
            file_t(std::string_view name, std::string_view type) : i_file(name) {
                if(memory::resource_t* resource = memory::c_module::self().find_resource(name, type))
                    file = memory::resource_cast_t<std::string>::cast(resource->load());
            }

        public:
            void open() override { }
            void close() override { }

            size_t read(void* buffer, size_t size) override {
                size_t bytes = std::min(size, file.size() - read_position);
                if(!bytes) return 0;

                if(buffer) {
                    for(size_t bytes_left = bytes, copy_size{ }; bytes_left > 0; copy_size = std::min(bytes_left, file.size() - read_position)) {
                        buffer = (void*)((std::uintptr_t)std::memcpy(buffer, file.data() + read_position, copy_size) + copy_size);

                        read_position += copy_size; bytes_left -= copy_size;
                    }
                } else read_position += bytes;

                return bytes;
            }

            bool seek(long offset, int origin) override {
                int new_read_position{ };

                switch(origin) {
                    case SEEK_SET: new_read_position = offset; break;
                    case SEEK_CUR: new_read_position = read_position + offset; break;
                    case SEEK_END: new_read_position = file.size() + offset; break;
                }

                if(new_read_position >= 0 && new_read_position <= file.size()) {
                    read_position = new_read_position;
                    return true;
                }

                return false;
            }

            size_t tell() const override { return read_position; }
        };

    public:
        static inline const std::regex regex{ "(\\[resource(:|\\|)([^\\]]*)\\]|\\[resource\\])\\s*([^\\s]*)" }; //@note: [resource:type] name or [resource|type] name

    public:
        resource_loader_t() { memory::c_module::self().load_resources(); }

    public:
        virtual bool create(std::string& path) override {
            std::smatch match{ };
            if(!std::regex_search(path, match, regex)) return false;
            if(match[3].matched && std::ranges::find(memory::c_module::self().resources, match[3].str(), &memory::resource_t::type) == memory::c_module::self().resources.end()) {
                utils::logger(utils::e_log_type::warning, "cant find \"{}\" resource", path);
                return false;
            }

            opened_files[path] = std::make_unique<file_t>(match[4].str(), match[3].matched ? match[3].str() : "");
            return true;
        }
    } inline resource_loader{ };
}