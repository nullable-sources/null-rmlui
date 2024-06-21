#pragma once
#include <RmlUi/Core.h>
#include <null-sdk.h>

#include "loaders/resource-loader.h"
#include "loaders/default-file-loader.h"

namespace null::rml {
    class c_file_interface : public Rml::FileInterface {
    public:
        std::vector<i_file_loader*> file_loaders{ &default_file_loader, &resource_loader };

    public:
        c_file_interface() { }

    public:
        Rml::FileHandle Open(const Rml::String& path) override;
        void Close(Rml::FileHandle file) override { i_file_loader::opened_files.erase(((i_file_loader::i_file*)file)->path); }

        size_t Read(void* buffer, size_t size, Rml::FileHandle file) override { return ((i_file_loader::i_file*)file)->read(buffer, size); }
        bool Seek(Rml::FileHandle file, long offset, int origin) override { return ((i_file_loader::i_file*)file)->seek(offset, origin); }
        size_t Tell(Rml::FileHandle file) override { return ((i_file_loader::i_file*)file)->tell(); }
    }; inline std::unique_ptr<c_file_interface> file_interface{ };
}