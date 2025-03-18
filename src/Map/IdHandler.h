#pragma once

#include <cstdint>

namespace asc2 {

    class IdHandler {

        public:

            IdHandler() = default;

            [[nodiscard]] uint64_t getNewId() {
                lastId++;
                return lastId;
            }

            void updateUsedIds(uint64_t updateId) {
                if (updateId > lastId)
                    lastId = updateId;
            }

        private:

            uint64_t lastId = 0;

    };
}