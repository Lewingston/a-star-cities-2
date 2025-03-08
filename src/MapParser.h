#pragma once

#include <nlohmann/json.hpp>

#include <string>

using json = nlohmann::json;

namespace asc2 {

    class MapParser{

        public:

            struct Node {
                const uint64_t id;
                const double   lon;
                const double   lat;
                const json     data;
            };

            struct Way {
                uint64_t id;
                std::vector<std::reference_wrapper<Node>> nodes;
                json data;
            };

            MapParser() = default;

            void loadFromFile(const std::string& filePath);
            void parse();

        private:

            void parseElements(const json& data);
            void parseWays();

            void parseNode(const json& data);
            void parseWay(const json& data);

            json data;

            std::map<uint64_t, Node> nodes;
            std::vector<Way> ways;
    };
}