/*
 * SpinBoxWithSlider.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: tobias
 */

#include "SpinBoxWithSlider.h"
#include <stdexcept> // std::invalid_argument

int pow(int base, unsigned int exp) {
	return exp == 0 ? 1 : base * pow(base, exp - 1);
}



SteppedSpinBox::SteppedSpinBox(QWidget *p)
	: QSpinBox(p)
{
}

int SteppedSpinBox::valueFromText(const QString &text) const  {
	int value = text.toInt();
	value = qMax(value, this->minimum());
	value = qMin(value, this->maximum());
	value -= (value - this->minimum()) % this->singleStep();
	return value;
}

SpinBoxWithSlider::SpinBoxWithSlider(QWidget *parent, const QString &name, int min, int max, int start_value, int step_size)
	: QWidget(parent)
	, m_step_size(step_size)
	, m_layout(this)
	, m_name(name, this)
	, m_slider(this)
	, m_spinbox(this)
{
	{
		if (m_step_size < 1) {
			throw std::invalid_argument("step isn't strictly positive");
		}
		if (! (min < max)) {
			throw std::invalid_argument("invalid range");
		}
		if (! (min <= start_value && start_value <= max)) {
			throw std::invalid_argument("start value isn't in range");
		}
		if ((max - min) % m_step_size) {
			throw std::invalid_argument("range length isn't a multiple of step size");
		}
		if ((start_value - min) % m_step_size) {
			throw std::invalid_argument("invalid start value");
		}
	}

	m_spinbox.setRange(min, max);
	m_spinbox.setValue(start_value);

	m_slider.setOrientation(Qt::Horizontal);
	m_slider.setRange(0, (max - min) / m_step_size);
	m_slider.setValue((start_value - min) / m_step_size);


	QObject::connect(&m_spinbox, SIGNAL(valueChanged(int)),
	                 this,      SLOT(SpinBoxValueChanged(int)));
	QObject::connect(&m_slider,  SIGNAL(valueChanged(int)),
	                 this,      SLOT(SliderValueChanged(int)));

	m_spinbox.setSingleStep(m_step_size);

	m_layout.setMargin(3);
	m_layout.setSpacing(3);

	m_layout.addWidget(&m_name);
	m_layout.addWidget(&m_slider);
	m_layout.addWidget(&m_spinbox);
}

SpinBoxWithSlider::SpinBoxWithSlider(QWidget *parent, QFormLayout *layout, const QString &name, int min, int max, int start_value, int step_size)
	: SpinBoxWithSlider(parent, name, min, max, start_value, step_size)
{
	if (layout) {
		layout->addRow(this);
	}
}

void SpinBoxWithSlider::SpinBoxValueChanged(int val) {
	m_slider.setValue((val - m_spinbox.minimum()) / m_step_size );
	emit valueChanged(this->value());
}
void SpinBoxWithSlider::SliderValueChanged(int value) {
	m_spinbox.setValue(m_spinbox.minimum() + value * m_step_size);
	emit valueChanged(this->value());
}

DoubleSpinBoxWithSlider::DoubleSpinBoxWithSlider(QWidget *parent, const QString &name, double min, double max, double start_value, int precision)
	: QWidget(parent)
	, m_factor(pow(10, precision))
	, m_layout(this)
	, m_name(name, this)
	, m_slider(this)
	, m_spinbox(this)
{
	m_spinbox.setRange(min, max);
	m_spinbox.setValue(start_value);
	m_spinbox.setDecimals(precision);
	m_spinbox.setSingleStep(1.0 / m_factor);

	m_slider.setOrientation(Qt::Horizontal);
	m_slider.setRange(static_cast<int>(m_factor * min), static_cast<int>(m_factor * max));
	m_slider.setValue(static_cast<int>(m_factor * start_value));


	QObject::connect(&m_spinbox, SIGNAL(valueChanged(double)),
	                 this,      SLOT(SpinBoxValueChanged(double)));
	QObject::connect(&m_slider,  SIGNAL(valueChanged(int)),
	                 this,      SLOT(SliderValueChanged(int)));

	m_layout.addWidget(&m_name);
	m_layout.addWidget(&m_slider);
	m_layout.addWidget(&m_spinbox);
}

DoubleSpinBoxWithSlider::DoubleSpinBoxWithSlider(QWidget *parent, QFormLayout *layout, const QString &name, double min, double max, double start_value, int precision)
	: DoubleSpinBoxWithSlider(parent, name, min, max, start_value, precision)
{
	if (layout) {
		layout->addRow(this);
	}
}

void DoubleSpinBoxWithSlider::SpinBoxValueChanged(double val) {
	m_slider.setValue(static_cast<int>(val * m_factor));
	emit valueChanged(this->value());
}

void DoubleSpinBoxWithSlider::SliderValueChanged(int value) {
	m_spinbox.setValue(static_cast<double>(value) / m_factor);
	emit valueChanged(this->value());
}
