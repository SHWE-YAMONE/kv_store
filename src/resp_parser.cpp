#include "resp_parser.hpp"
#include <string>
#include <iostream>

std::optional<std::string_view> RESPParser::readLine(std::string_view buf, size_t& pos) {
    size_t end = buf.find("\r\n", pos);
    if (end == std::string_view::npos)
        return std::nullopt;

    auto line = buf.substr(pos, end - pos);
    pos = end + 2;
    return line;
}

ParseStatus RESPParser::parse(std::string_view& buf, Command& cmd) {
    if (buf.empty()) return ParseStatus::INCOMPLETE;
    if (buf[0] != '*') return ParseStatus::ERROR;

    size_t pos = 0;
    auto line = readLine(buf, pos);

    if (!line) return ParseStatus::INCOMPLETE;

    try {
        int count = std::stoi(std::string(line->substr(1)));

        for (int i = 0; i < count; i++) {
            auto len_line = readLine(buf, pos);
            if (!len_line) return ParseStatus::INCOMPLETE;

            // Bulk strings start with '$'
            int len = std::stoi(std::string(len_line->substr(1)));

            // Check if we have the full string + CRLF
            if (pos + len + 2 > buf.size())
                return ParseStatus::INCOMPLETE;

            std::string_view data = buf.substr(pos, len);
            pos += len + 2;

            if (i == 0) cmd.name = data;
            else cmd.args.push_back(data);
        }
    } catch (...) {
        return ParseStatus::ERROR;
    }

    buf.remove_prefix(pos);
    
    return ParseStatus::SUCCESS;
}