#pragma once

#include <memory>

#include <boost/optional.hpp>

#include <opencv2/opencv.hpp>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

#include "biotracker/TextureObject.h"
#include "biotracker/BioTrackerApp.h"

#include "biotracker/util/stdext.h"

class QOpenGLContext;

namespace BioTracker {
namespace Gui {

class VideoView : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
  public:
    enum class Mode : uint8_t {
        INTERACTION = 0,
        PANZOOM
    };

    VideoView(QWidget *parent, Core::BioTrackerApp &facade);

    Mode getMode() const {
        return m_currentMode;
    }

    Core::TextureObject &getTexture() {
        return m_texture;
    }

  public Q_SLOTS:
    void setMode(const Mode mode);
    void fitToWindow();

private Q_SLOTS:
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

  private:
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

        boost::optional<CurrentPanState> panState;
    };

    QOpenGLDebugLogger m_openGLLogger;

    /**
     * @brief Current State of the VideoWidget. In Interaction mode, mouse and keyboard event will be forwarded to
     * the currently active tracking algorithm.
     */
    Mode m_currentMode;

    /**
     * @brief Pan/Zoom state variables.
     */
    PanZoomState m_panZoomState;

    /**
     * @brief Ratio of widget size and image size
     */
    float m_screenPicRatio;

    /**
     * @brief Wrapper for the OpenGL texture. Also contains the original image as opencv matrix.
     */
    Core::TextureObject m_texture;

    Core::BioTrackerApp &m_biotracker;

    /**
     * @brief m_painter
     */
    QPainter m_painter;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    /**
    * @brief unprojectScreenPos
    * @param mouseCoords coordinates relative to window
    * @return coordinates relative to image
    */
    QPoint unprojectScreenPos(QPoint mouseCoords);

    /**
     * @brief projectPicturePos
     * @param imageCoords coordinates relative to image
     * @return coordinates relative to window
     */
    QPoint projectPicturePos(QPoint imageCoords);

    void keyPressEvent(QKeyEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
};
}
}
