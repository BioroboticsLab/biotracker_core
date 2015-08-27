#pragma once

#include <QOpenGLContext>

namespace BioTracker {
namespace Util {

class SharedOpenGLContext : public QOpenGLContext {
  public:
    SharedOpenGLContext(QOpenGLContext *mainContext)
        : QOpenGLContext(nullptr) {
        QSurfaceFormat format = mainContext->format();
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setOption(QSurfaceFormat::DebugContext);

        setFormat(format);
        setShareContext(mainContext);
        bool isCreated = create();
        assert(isCreated);
    }
};

}
}
