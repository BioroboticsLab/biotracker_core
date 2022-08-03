/****************************************************************************
 **
 ** This file is part of the BioTracker Framework
 ** by Andreas Jörg and BioTracker in version 2.
 **
 ****************************************************************************/

#ifndef BIOTRACKER3TEXTUREOBJECT_H
#define BIOTRACKER3TEXTUREOBJECT_H

#include "Interfaces/IModel/IModel.h"

#include <opencv2/opencv.hpp>
#include "QImage"
#include "QString"

/**
 * The TextureObject class in an IModel class. It is responsible for converting
 * cv::Mats to QImages. These QImages are then displayed in the
 * TextureObjectView. This class was adapted from the TextureObject class in
 * BioTracker 2.
 */
class TextureObject : public IModel
{
    Q_OBJECT
public:
    explicit TextureObject(QObject* parent = 0, QString name = "NoName");

    void    set(cv::Mat img);
    QString getName();

    QImage const& get() const
    {
        return m_texture;
    }
    int width() const
    {
        return m_texture.width();
    }
    int height() const
    {
        return m_texture.height();
    }

private:
    QString m_Name;
    cv::Mat m_img;
    QImage  m_texture;
};

#endif // BIOTRACKER3TEXTUREOBJECT_H
