#pragma once

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <opencv2/opencv.hpp> // cv::Point, cv::Rect
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Python.h>
#include <boost/python.hpp>
#include <QString>
#include <iostream>
#include <codecvt>        // std::codecvt_utf8
#include <sstream>
#include <numpy/ndarrayobject.h>
#include "../interpreter/PyQPainter.h"

/**
 * Python helper functions
 */
namespace PyHelper {

/**
 * @brief convert
 * Before calling this function it must be sure that the Python interpreter is initialized
 * @param mat
 * @return
 */
PyObject *convert(const cv::Mat &mat);

/**
 * @brief errorMessage
 * get the current error message
 * @return
 */
std::string errorMessage();

/**
 * @brief initNumpy
 * @return Nothing, but must return something to fit the numpy api
 */
void *initNumpy();

/**
 * @brief hasError
 * @return True when there is an Error on the Python interpreter, False otherwise
 */
bool hasError();

/**
 * @brief toString
 * @param o
 * @return
 */
std::string toString(PyObject *o);

/**
 * @brief infos
 * gives some arbitrary infos about the py interpreter
 * @return
 */
std::string infos();

/**
 * @brief isFunc
 * @param pFunc
 * @return True if pFunc is a function, otherwise False
 */
bool isFunc(PyObject *pFunc);

/**
 * @brief init
 * Must be called in the main thread and before any other thread is created
 */
void init();

}
