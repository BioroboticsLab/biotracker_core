#include "settings/SystemCompatibilityCheck.h"

//#include <QGLFormat>

bool SystemCompatibilityCheck::checkOpenGLSupport() {
    return true;//QGLFormat::hasOpenGL();
}
