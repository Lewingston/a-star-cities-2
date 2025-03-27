#pragma once

#include "../Map/Intersection.h"

#include <utility>

namespace asc2 {

    class Solver {

        public:

            Solver();

            [[nodiscard]] static std::pair<const Intersection&, const Intersection&> selectStartAndEndNode();

        private:
    };
}