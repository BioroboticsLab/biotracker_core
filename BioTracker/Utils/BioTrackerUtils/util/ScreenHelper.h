#pragma once

#include <QRect>              // QRect
#include <QPoint>             // QPoint
#include "PanZoomState.h"
#include "math.h"

/**
 * Screen helper functions
 */
namespace BioTracker {
namespace Core {
namespace ScreenHelper {

/**
 * @brief calculate_viewport
 * @param im_w width of the image
 * @param im_h height of the image
 * @param w width of the videoview
 * @param h height of the videoview
 * @param window OUT parameter
 * @param viewport OUT parameter
 */
inline float calculate_viewport(
    const int im_w, const int im_h,
    const int w, const int h,
    QRect &window, QRect &viewport) {

    // We use setWindow and setViewport to fit the video into the
    // given video widget frame (with width "w" and height "h")
    // we later need to adjust an offset caused the use of different
    // dimensions for window and viewport.
    const float im_wf = static_cast<float>(im_w);
    const float im_hf = static_cast<float>(im_h);
    const float w_f = static_cast<float>(w);
    const float h_f = static_cast<float>(h);

    float ratio = 0;
    int offset_top = 0;
    int offset_left = 0;

    if ((im_wf/im_hf) > (w_f/h_f)) {
        // image ratio is bigger (wider image than window)
        ratio = im_wf / w_f;
        const float px = w_f / im_wf;
        const float px_im_hf = px * im_hf;
        offset_top = h/2 - static_cast<int>(px_im_hf / 2);
    } else {
        // window ratio is bigger (narrow image)
        ratio = im_hf / h_f;
        const float px = h_f / im_hf;
        const float px_im_wf = px * im_wf;
        offset_left = w/2 - static_cast<int>(px_im_wf / 2);
    }

    const int screen_w = static_cast<int>(im_wf * ratio);
    const int screen_h = static_cast<int>(im_hf * ratio);

    window.setX(0);
    window.setY(0);
    window.setWidth(screen_w);
    window.setHeight(screen_h);

    viewport.setX(offset_left);
    viewport.setY(offset_top);
    viewport.setWidth(im_w);
    viewport.setHeight(im_h);

    // adjust the panning as the viewport is potentially scewed
    // and mouse movements given by the window are not translated
    // one-to-one anymore
    return screen_w / im_wf;
}

/**
 * @brief getImDimsInScreenCoords
 * Calculates the actual dimension of the image with all zoom, pan and
 * viewport transformations applied
 * @return a rectangle that represents the position and dimension of the
 * image in the videoview element
 */
inline QRect getImDimsInScreenCoords(
    const PanZoomState zoomState,
    const int im_w, const int im_h,
    const int w, const int h) {

    QRect viewport, window;
    const float viewportRatio = calculate_viewport(im_w, im_h, w, h, window, viewport);
    const float zoom = 1 + zoomState.zoomFactor;
    // back-translate the pan to non-zoomed coordinate space
    float realPanX = -zoomState.panX;
    float realPanY = -zoomState.panY;
    realPanX += (viewport.x());
    realPanY += (viewport.y());

    // The image in screen coords
    QRect actualIm(
        static_cast<int>(realPanX), static_cast<int>(realPanY),
        static_cast<int>((im_w / viewportRatio) * zoom),
        static_cast<int>((im_h / viewportRatio) * zoom)
    );
    return actualIm;
}

/**
 * @brief imageToScreenCoords
 */
inline QPoint imageToScreenCoords(
    const PanZoomState zoomState,
    const int im_w, const int im_h,
    const int w, const int h,
    const QPoint poi) {
    QPoint result;
    const QRect actualIm = getImDimsInScreenCoords(zoomState, im_w, im_h, w, h);
    const float one_step_x = actualIm.width() / static_cast<float>(im_w);
    const float one_step_y = actualIm.height() / static_cast<float>(im_h);
    const int actualPosXInIm = static_cast<int>(round(poi.x() * one_step_x));
    const int actualPosYInIm = static_cast<int>(round(poi.y() * one_step_y));
    result.setX(actualPosXInIm + actualIm.x());
    result.setY(actualPosYInIm + actualIm.y());
    return result;
}

/**
 * @brief screenToImageCoords
 * Translate the window coordinates to the actual image coords
 * @param im_w image width
 * @param im_h image height
 * @param w width of the videoview
 * @param h height of the videoview
 * @param poi Point in Screen Coords
 */
inline QPoint screenToImageCoords(
    const BioTracker::Core::PanZoomState zoomState,
    const int im_w, const int im_h,
    const int w, const int h,
    QPoint poi) {
    QPoint result(0, 0);

    // The image in screen coords
    QRect actualIm = getImDimsInScreenCoords(zoomState, im_w, im_h, w, h);

    const float im_wf = static_cast<float>(im_w);
    const float im_hf = static_cast<float>(im_h);
    const float one_step_x = im_wf / actualIm.width();
    const float one_step_y = im_hf / actualIm.height();

    const float transformedX = (-actualIm.x() + poi.x()) * one_step_x;
    const float transformedY = (-actualIm.y() + poi.y()) * one_step_y;

    result.setX(static_cast<int>(round(transformedX)));
    result.setY(static_cast<int>(round(transformedY)));

    return result;
}

/**
 * @brief zoomTo
 * Modifies the panzoomstate so that the cursor (zoomCenter) will point to the same
 * location in the image as prior to the zoom
 * @param im_w width of the image (in px)
 * @param im_h height of the image (in px)
 * @param w width of the videoview element
 * @param h height of the videoview element
 */
inline PanZoomState zoomTo(
    PanZoomState state,
    const int im_w,
    const int im_h,
    const int w,
    const int h,
    const float deltaZoom,
    const QPoint zoomCenter) {

    const QPoint imPos = screenToImageCoords(state, im_w, im_h, w, h, zoomCenter);

    const float oldZoomFactor = state.zoomFactor;
    const float newZoomFactor = state.zoomFactor - (deltaZoom/2000);

    if (newZoomFactor <= -1.0f) {
        // MAX VALUE
        return state;
    }

    const float zoom = 1 + newZoomFactor;

    const float oldPanX = state.panX / (1 + oldZoomFactor);
    const float oldPanY = state.panY / (1 + oldZoomFactor);

    // zoom with origin in (0/0)
    state.panX = oldPanX * zoom;
    state.panY = oldPanY * zoom;
    state.zoomFactor = newZoomFactor;

    // As we do not want to zoom to (0/0) but rather keep the focus on the chosen
    // image position (zoomCenter) we now have to "back-translate" to our initial
    // position
    const QPoint translatedZoomCenter = imageToScreenCoords(state, im_w, im_h, w, h, imPos);
    const QPoint translate = translatedZoomCenter - zoomCenter;
    state.panX += translate.x();
    state.panY += translate.y();
    state.isChanged = true;
    return state;
}

}
}
}
