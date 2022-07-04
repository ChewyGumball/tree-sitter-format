#pragma once

#include <string_view>

namespace tree_sitter_format {

template<typename T>
struct Setting {
    std::string_view name;
    T value;
};

static constexpr Setting<bool> 


}
