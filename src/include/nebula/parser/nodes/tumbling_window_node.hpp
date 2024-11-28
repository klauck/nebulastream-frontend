//
// Created by Usama Bin Tariq on 10.10.24.
//

#pragma once
#include "window_range_node.hpp"

namespace nebula {

    class TumblingWindowNode : public WindowRangeNode {
      public:
          TumblingWindowNode(): WindowRangeNode(WindowRangeNodeType::TUMBLING_WINDOW) {}
    };
}