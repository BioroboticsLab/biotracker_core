#include "../../interpreter/PyQPainter.h"
#include <iostream>
#include <boost/python.hpp>


//http://www.boost.org/doc/libs/1_59_0/libs/python/doc/tutorial/doc/html/python/exposing.html

using namespace boost::python;

namespace BioTracker {
namespace Core {
namespace Interpreter {

void PyQPainter::setQPainter(QPainter *p) {
    m_painter = p;
}

void PyQPainter::drawArc(int x, int y, int w, int h, int startAngle,
                         int spanAngle) {

}

void PyQPainter::drawChord(int x, int y, int w, int h, int startAngle,
                           int spanAngle) {

}

void PyQPainter::drawEllipse(int x, int y, int w, int h) {

}

void PyQPainter::drawLine(int x1, int y1, int x2, int y2) {

}

void PyQPainter::drawPie(int x, int y, int w, int h, int startAngle,
                         int spanAngle) {

}

void PyQPainter::drawPoint(int x, int y) {

}

void PyQPainter::drawRect(int x, int y, int w, int h) {

}

void PyQPainter::drawText(int x, int y, std::__cxx11::string text) {

}

void PyQPainter::eraseRect(int x, int y, int w, int h) {

}

void PyQPainter::fillRect(int x, int y, int w, int h, tuple color) {
    std::cout << "fillRect in c++ " << x << " " << y << std::endl;
}

void PyQPainter::setBrush(tuple color) {

}

}
}
}

object PyQPainterClass;
//object pyQPainterInstance;
BOOST_PYTHON_MODULE(libpybiotracker) {
    PyQPainterClass =
        class_<BioTracker::Core::Interpreter::PyQPainter>("PyQPainter")
        .def("drawEllipse", &BioTracker::Core::Interpreter::PyQPainter::drawEllipse)
        .def("drawLine", &BioTracker::Core::Interpreter::PyQPainter::drawLine)
        .def("drawPie", &BioTracker::Core::Interpreter::PyQPainter::drawPie)
        .def("drawPoint", &BioTracker::Core::Interpreter::PyQPainter::drawPoint)
        .def("drawRect", &BioTracker::Core::Interpreter::PyQPainter::drawRect)
        .def("drawText", &BioTracker::Core::Interpreter::PyQPainter::drawText)
        .def("eraseRect", &BioTracker::Core::Interpreter::PyQPainter::eraseRect)
        .def("fillRect", &BioTracker::Core::Interpreter::PyQPainter::fillRect)
        .staticmethod("fillRect")
        .def("drawChord", &BioTracker::Core::Interpreter::PyQPainter::drawChord)
        .def("drawArc", &BioTracker::Core::Interpreter::PyQPainter::drawArc);
}

