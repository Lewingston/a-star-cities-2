#pragma once

#include "../Map/Way.h"

#include<vector>

namespace asc2 {

    class ShapeRenderer {

        public:

            ShapeRenderer(const std::vector<std::reference_wrapper<const Way>>& outerShapes,
                          const std::vector<std::reference_wrapper<const Way>>& innerShapes);

            [[nodiscard]] std::vector<std::pair<float, float>> getVertices() const;

        private:

            [[nodiscard]] std::vector<std::pair<float, float>> getVerticesForShape(const Way& outerShape) const;

            std::vector<std::reference_wrapper<const Way>> outerShapes;
            std::vector<std::reference_wrapper<const Way>> innerShapes;

    };
}