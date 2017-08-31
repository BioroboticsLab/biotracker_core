#pragma once
#include "Interfaces/IModel/IModel.h"
#include "AreaInfoElement.h"

class AreaInfo : public IModel
{
	Q_OBJECT
public:
	AreaInfo(QObject *parent = 0);

	std::shared_ptr<AreaInfoElement> _rect;
	std::shared_ptr<AreaInfoElement> _apperture;
};

