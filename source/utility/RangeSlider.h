#pragma once

#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>


class RangeSlider: public QWidget {
	Q_OBJECT
public:
	
	explicit RangeSlider(QWidget *parent = nullptr);
	virtual ~RangeSlider() override = default;

	RangeSlider(const RangeSlider&) = delete;
	RangeSlider& operator=(const RangeSlider&) = delete;

	void setEnabled(bool b);

	void setRange(int start, int end);
	void setRange(int start, int end, int positionStart, int positionEnd);

private slots:
	void setValueStartForceRange(int);
	void setValueEndForceRange(int);

public slots:
	void setValueStart(int);
	void setValueEnd(int);
	void setValue(int, int);

signals:
	void valueChangedStart(int value);
	void valueChangedEnd(int value);
	void valueChanged(int start, int end);

private:
	QGridLayout _gridLayout;

	QLabel      _labelStart;
	QLabel      _labelEnd;

	QSlider     _sliderStart;
	QSlider     _sliderEnd;
	QSpinBox    _spinBoxStart;
	QSpinBox    _spinBoxEnd;

	bool        _settingBothValuesAtOnce;
};



