#pragma once
#include <boost/python.hpp>
#include <structmember.h>
#include <QPainter>

namespace BioTracker {
namespace Core {
namespace Interpreter {

/**
 * @brief The PyQPainter class
 *
 * Wrapper class for QPainter -> Python
 *
 */
class PyQPainter {
  public:
    PyQPainter() {};

    void setQPainter(QPainter *p);

    void drawArc(int x, int y, int w, int h, int startAngle, int spanAngle);
    void drawChord(int x, int y, int w, int h, int startAngle, int spanAngle);
    void drawEllipse(int x, int y, int w, int h);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawPie(int x, int y, int w, int h, int startAngle, int spanAngle);
    void drawPoint(int x, int y);
    void drawRect(int x, int y, int w, int h);
    void drawText(int x, int y, std::string text);
    void eraseRect(int x, int y, int w, int h);
    void fillRect(int x, int y, int w, int h, boost::python::tuple color);
    void setBrush(boost::python::tuple color);

  private:
    QPainter *m_painter;

};

}
}
}
