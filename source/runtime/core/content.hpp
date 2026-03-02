#pragma once

#include <filesystem>

#include <core/id.hpp>

namespace pspedit {

struct content_database_entry {
    object_id id;
    std::filesystem::path path;
};

struct content_database {


    std::vector<content_database_entry> entries;
};

}