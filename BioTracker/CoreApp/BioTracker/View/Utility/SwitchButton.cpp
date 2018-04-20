#include "SwitchButton.h"
#include "QPainter"
#include "QMouseEvent"

void SwitchButton::setHeight(int h) {
	_height = h - 2 * _margin;
}

SwitchButton::SwitchButton(QString s0, QString s1, QWidget *parent) : QAbstractButton(parent),
_height(16),
_opacity(0.000),
_switch(false),
_margin(3),
_thumb("#d5d5d5"),
_anim(new QPropertyAnimation(this, "offset", this)),
_s0(s0),
_s1(s1)
{
	setOffset(_height / 2);
	_y = _height / 2;
	setBrush(QColor("#009688"));
};


void SwitchButton::paintEvent(QPaintEvent *e) {
	QPainter p(this);
	p.setPen(Qt::NoPen);
	if (isEnabled()) {

		//background rect
		//p.setBrush(_switch ? brush() : Qt::black);
		p.setBrush(_switch ? Qt::green : Qt::red);
		//p.setOpacity(_switch ? 0.5 : 0.38);
		p.setOpacity(0.5);
		p.setRenderHint(QPainter::Antialiasing, true);
		p.drawRoundedRect(QRect(_margin, _margin, width() - 2 * _margin, height() - 2 * _margin), 8.0, 8.0);

		//handle
		p.setBrush(_thumb);
		p.setOpacity(1.0);
		p.drawEllipse(QRectF(offset() - (_height / 2), _y - (_height / 2), height(), height()));

		//text
		p.setPen(Qt::black);
		QFont font = p.font();
		font.setPixelSize(height() - 3 * _margin);
		p.setFont(font);
		QRectF textRect = QRectF(_margin, _margin, width() - 2 * _margin, height() - 2 * _margin);
		p.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter, _switch ? _s1 : _s0);
	}
	else {
		p.setRenderHint(QPainter::Antialiasing, true);
		p.setBrush(Qt::black);
		p.setOpacity(0.12);
		p.drawRoundedRect(QRect(_margin, _margin, width() - 2 * _margin, height() - 2 * _margin), 8.0, 8.0);
		p.setOpacity(1.0);
		p.setBrush(QColor("#BDBDBD"));
		p.drawEllipse(QRectF(offset() - (_height / 2), _y - (_height / 2), height(), height()));

		//text
		p.setPen(Qt::black);
		QFont font = p.font();
		font.setPixelSize(height() - 3 * _margin);
		p.setFont(font);
		QRectF textRect = QRectF(_margin, _margin, width() - 2 * _margin, height() - 2 * _margin);
		p.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter, _switch ? _s1 : _s0);
	}
}

void SwitchButton::mouseReleaseEvent(QMouseEvent *e) {
	if (e->button() & Qt::LeftButton) {
		_switch = _switch ? false : true;
		animateSwitch();

		emitSetEnabled(_switch);
	}
	QAbstractButton::mouseReleaseEvent(e);
}

void SwitchButton::enterEvent(QEvent *e) {
	setCursor(Qt::PointingHandCursor);
	QAbstractButton::enterEvent(e);
}

void SwitchButton::animateSwitch() {
	if (_switch) {
		_anim->setStartValue(_height / 2);
		_anim->setEndValue(width() - _height);
		_anim->setDuration(240);
		_anim->start();
	}
	else {
		_anim->setStartValue(offset());
		_anim->setEndValue(_height / 2);
		_anim->setDuration(240);
		_anim->start();
	}
}

QSize SwitchButton::sizeHint() const {
	return QSize(8 * (_height + _margin), _height + 2 * _margin);
}