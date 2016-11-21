#pragma once

#include <cv.h>

class Rectification;
class Settings;


class IImageProcessor {
  public:
    IImageProcessor(Rectification &rectification, Settings &settings) :
        _rectification(rectification),
        _settings(settings) {
    }

    virtual ~IImageProcessor(void) {}

    /**
     * This method processed the image and detects the found object.
     * @param image, the image to process,
     * @return void.
     */
    virtual void process(cv::Mat image) = 0;

  protected:
    const Rectification &_rectification;
    Settings &_settings;
};

