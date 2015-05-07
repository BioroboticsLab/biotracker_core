#ifndef RectObject_H_
#define RectObject_H_

#include <opencv2/opencv.hpp>
#include <source/tracking/serialization/ObjectModel.h>

class RectObject : public ObjectModel
{
public:

	explicit RectObject();
	explicit RectObject(size_t _frameNr, cv::Point _rectStart, cv::Point _rectEnd);
	virtual ~RectObject() override;

	size_t getRectFrameNr() const;

	cv::Point getRectStart() const;
	cv::Point getRectEnd() const;

private:

	size_t		_frameNr;
	cv::Point	_rectStart;
	cv::Point	_rectEnd;

	// generate serialization functions
	friend class cereal::access;
	template <class Archive>
	void save(Archive& arch) const
	{
		 arch(
			CEREAL_NVP(_frameNr),
			CEREAL_NVP(_rectStart),
			CEREAL_NVP(_rectEnd));
	}

	template<class Archive>
	void load(Archive& arch)
	{
		 arch(
			CEREAL_NVP(_frameNr),
			CEREAL_NVP(_rectStart),
			CEREAL_NVP(_rectEnd)); 

	}
};

#endif