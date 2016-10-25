#pragma once

#include "boost/optional.hpp"

namespace BioTracker {
namespace Core {

/**
* @brief Used to store mouse cursor offsets while panning.
*/
struct CurrentPanState {
    QPointF lastPos;

    CurrentPanState(const QPointF lastPos)
        : lastPos(lastPos) {
    }
};

/**
 * @brief Stores the current zoom and pan offsets. While panning, panState stores the last mouse cursor position.
 */
struct PanZoomState {
    float zoomFactor = 0.f;
    float panX       = 0.f;
    float panY       = 0.f;
    bool isChanged = false;

    boost::optional<CurrentPanState> panState;
};

}
}
