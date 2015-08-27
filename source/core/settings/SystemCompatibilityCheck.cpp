#include "SystemCompatibilityCheck.h"

#include <QGLFormat>

bool SystemCompatibilityCheck::checkOpenGLSupport() {
    return QGLFormat::hasOpenGL();
}
