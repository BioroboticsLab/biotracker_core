#include "RangeSlider.h"
#include <QLineEdit>

RangeSlider::RangeSlider(QWidget *parent)
	: QWidget(parent)
	, _gridLayout(this)
	, _labelStart("Start:", this)
	, _labelEnd("End:", this)
	, _sliderStart(Qt::Horizontal, this)
	, _sliderEnd(Qt::Horizontal, this)
	, _spinBoxStart(this)
	, _spinBoxEnd(this)
	, _settingBothValuesAtOnce(false)
{
	_spinBoxStart.findChild<QLineEdit*>()->setReadOnly(true);
	_spinBoxEnd.findChild<QLineEdit*>()->setReadOnly(true);

	_gridLayout.addWidget(&_labelStart,   0, 0);
	_gridLayout.addWidget(&_sliderStart,  0, 1);
	_gridLayout.addWidget(&_spinBoxStart, 0, 2);

	_gridLayout.addWidget(&_labelEnd,     1, 0);
	_gridLayout.addWidget(&_sliderEnd,    1, 1);
	_gridLayout.addWidget(&_spinBoxEnd,   1, 2);

	QObject::connect(&_spinBoxStart, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &_sliderStart,  &QSlider::setValue);
	QObject::connect(&_sliderStart,                                       &QSlider::valueChanged,  &_spinBoxStart, &QSpinBox::setValue);
	QObject::connect(&_sliderStart,                                       &QSlider::valueChanged,  this,           &RangeSlider::setValueStartForceRange);

	QObject::connect(&_spinBoxEnd,   static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &_sliderEnd,    &QSlider::setValue);
	QObject::connect(&_sliderEnd,                                         &QSlider::valueChanged,  &_spinBoxEnd,   &QSpinBox::setValue);
	QObject::connect(&_sliderEnd,                                         &QSlider::valueChanged,  this,           &RangeSlider::setValueEndForceRange);
}

void RangeSlider::setEnabled(bool b) {
	_sliderStart.setEnabled(b);
	_sliderEnd.setEnabled(b);
	_spinBoxStart.setEnabled(b);
	_spinBoxEnd.setEnabled(b);
}

void RangeSlider::setRange(int start, int end) {
	_sliderStart.setRange( start, end);
	_sliderEnd.setRange(   start, end);
	_spinBoxStart.setRange(start, end);
	_spinBoxEnd.setRange(  start, end);
}

void RangeSlider::setRange(int start, int end, int positionStart, int positionEnd) {
	this->setRange(start, end);
	this->setValue(positionStart, positionEnd);
}

void RangeSlider::setValueStartForceRange(int newStartVal)
{
	const int startVal = _sliderStart.value();
	const int endVal   = _sliderEnd.value();

	if (newStartVal > endVal)
	{
		newStartVal = endVal;
		_sliderStart.setValue(newStartVal);
	}
	if (newStartVal != startVal)
	{
		emit valueChangedStart(newStartVal);
		if (!_settingBothValuesAtOnce) {
			emit valueChanged(newStartVal, endVal);
		}
	}
}

void RangeSlider::setValueEndForceRange(int newEndVal)
{
	const int startVal = _sliderStart.value();
	const int endVal   = _sliderEnd.value();

	if (newEndVal < startVal)
	{
		newEndVal = startVal;
		_sliderEnd.setValue(newEndVal);
	}

	if (newEndVal != endVal)
	{
		emit valueChangedEnd(newEndVal);
		if (!_settingBothValuesAtOnce) {
			emit valueChanged(startVal, newEndVal);
		}
	}
}

void RangeSlider::setValueStart(int newStartVal) {
	_sliderStart.setValue(newStartVal);
	// automatically changes connected _spinBoxStart
}

void RangeSlider::setValueEnd(int newEndVal) {
	_sliderEnd.setValue(newEndVal);
	// automatically changes connected _spinBoxEnd
}

void RangeSlider::setValue(int newStartVal, int newEndVal)
{
	const int startVal = _sliderStart.value();
	const int endVal   = _sliderEnd.value();

	_settingBothValuesAtOnce = true; // disable emitting valueChanged inside
	                                 // the setValue*ForceRange methods
	                                 // to prevent a double emit when both
	                                 // values are changed at the same time

	/* If start is moved to the right, end has to be adjusted first.
	 * Otherwise newStartVal might collide with the old end value
	 * and thus be changed, although [newStartVal, newEndVal] is valid.
	 *
	 * e.g.
	 * [10, 20] is overwritten by [30, 40].
	 * Setting start to 30 would adjust it to 20.
	 */
	if (newStartVal > _sliderStart.value()) {
		this->setValueEnd(newEndVal);
		this->setValueStart(newStartVal);

	}
	else {
		this->setValueStart(newStartVal);
		this->setValueEnd(newEndVal);
	}

	_settingBothValuesAtOnce = false;
	newStartVal = _sliderStart.value();
	newEndVal   = _sliderEnd.value();
	if (newStartVal != startVal || newEndVal != endVal) {
		emit valueChanged   (newStartVal, newEndVal);
	}
}
