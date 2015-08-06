#pragma once

#include <QOpenGLContext>

namespace BioTracker {
namespace Util {

class SharedOpenGLContext : public QOpenGLContext
{
public:
    SharedOpenGLContext(QOpenGLContext *mainContext)
        : QOpenGLContext(nullptr)
    {
        setFormat(mainContext->format());
        setShareContext(mainContext);
        create();
    }
};

}
}
