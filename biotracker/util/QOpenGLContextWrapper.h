#pragma once

#include <QGuiApplication>
#include <QOpenGLContext>
#include <QSurface>

namespace BioTracker {
namespace Util {

class QOpenGLContextWrapper : public QOpenGLContext {
  public:
    QOpenGLContextWrapper(QObject *parent)
        : QOpenGLContext(parent) {
        create();
    }
};

}
}
