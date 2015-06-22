#pragma once

#include <QOpenGLContext>

namespace BioTracker {
namespace Util {

class QOpenGLContextWrapper : public QOpenGLContext
{
public:
    QOpenGLContextWrapper(QObject *parent)
        : QOpenGLContext(parent)
    {
        create();
    }
};

}
}
