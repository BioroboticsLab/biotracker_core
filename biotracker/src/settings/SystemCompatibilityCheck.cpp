#include "settings/SystemCompatibilityCheck.h"

#include <QGLFormat>

bool SystemCompatibilityCheck::checkOpenGLSupport() {
    return QGLFormat::hasOpenGL();
}
