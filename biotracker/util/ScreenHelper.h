#pragma once

#include <QRect>              // QRect
#include <QPoint>             // QPoint

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
 * @param window
 * @param viewport
 * @return
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

//inline QPoint imageToScreenCoords() {

//}

/**
 * @brief screenToImageCoords
 * Translate the window coordinates to the actual image coords
 * @param zoomState
 * @param im_w image width
 * @param im_h image height
 * @param w width of the videoview
 * @param h height of the videoview
 * @param poi Point in Screen Coords
 * @return
 */
inline QPoint screenToImageCoords(
    const BioTracker::Core::PanZoomState zoomState,
    const int im_w, const int im_h,
    const int w, const int h,
    QPoint poi) {
    QPoint result(0, 0);

    QRect viewport;
    QRect window;
    const float viewport_offset =
        calculate_viewport(im_w, im_h, w, h, window, viewport);

    const float zoom = 1 + zoomState.zoomFactor;

    // back-translate the pan to non-zoomed coordinate space
    float realPanX = -zoomState.panX;
    float realPanY = -zoomState.panY;
    realPanX += (viewport.x());
    realPanY += (viewport.y());

    // The image in screen coords
    QRect actualIm(
        static_cast<int>(realPanX), static_cast<int>(realPanY),
        static_cast<int>((im_w / viewport_offset) * zoom),
        static_cast<int>((im_h / viewport_offset) * zoom)
    );

    const float im_wf = static_cast<float>(im_w);
    const float im_hf = static_cast<float>(im_h);
    const float one_step_x = im_wf / actualIm.width();
    const float one_step_y = im_hf / actualIm.height();

    const float transformedX = (-actualIm.x() + poi.x()) * one_step_x;
    const float transformedY = (-actualIm.y() + poi.y()) * one_step_y;

    result.setX(static_cast<int>(transformedX));
    result.setY(static_cast<int>(transformedY));

    if (!actualIm.contains(poi)) {

        // both x and y coords are oor so we need to put our result in one
        // of the 4 corners
        //     * <-- POI
        //        .-------.
        //        | Image |
        //
        if (poi.x() < actualIm.x()) {
            result.setX(0);
        } else {
            result.setX(im_w);
        }
        if (poi.y() < actualIm.y()) {
            result.setY(0);
        } else {
            result.setY(im_h);
        }

        if (poi.y() > actualIm.y() && poi.y() < actualIm.y() + actualIm.height()) {
            //   POI  .-------.
            //    |   | Image |
            //    v   |       |
            //    *   |       |
            result.setY(static_cast<int>(one_step_y * poi.y()));
        } else if (poi.x() > actualIm.x() && poi.x() < actualIm.x() + actualIm.width()) {
            //          * <-- POI
            //        .-------.
            //        | Image |
            //
            result.setX(static_cast<int>(one_step_x * poi.x()));
        }
    }

    return result;
}
}
}
}
