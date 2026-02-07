#pragma once
#include <string>
#include <format>

static std::string toFileName(int id) {
    return fmt::format("{:02d}", id);
}

class CharacterDatabase {
public:
    static CharacterData load(int id) {
        std::string path = "assets\\characters\\" + std::to_string(id) + ".json";
        return loadJson(path);
    }
};

