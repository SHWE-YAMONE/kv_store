#pragma once
#include <string_view>
#include <vector>
#include <optional>

enum class ParseStatus { SUCCESS, INCOMPLETE, ERROR };

struct Command {
    std::string_view name;
    std::vector<std::string_view> args;
    void clear() { name = {}; args.clear(); }
};

class RESPParser {
public:
    ParseStatus parse(std::string_view& buf, Command& cmd);

private:
    std::optional<std::string_view> readLine(std::string_view buf, size_t& pos);
};