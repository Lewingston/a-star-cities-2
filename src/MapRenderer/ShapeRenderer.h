#pragma once

#include "../Map/Way.h"

#include<vector>

namespace asc2 {

    class ShapeRenderer {

        public:

            ShapeRenderer(const Way& outerShape, const std::vector<std::reference_wrapper<const Way>>& innerShapes);

        private:

    };
}