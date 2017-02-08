#ifndef TRACKERPARAMETER_H
#define TRACKERPARAMETER_H


#include "Interfaces/IModel/IModel.h"

class TrackerParameter : public IModel
{
    Q_OBJECT
public:
    TrackerParameter(QObject *parent = 0);

    void setThreshold(int x);

    int getThreshold();

private:
    int m_Threshold;
};

#endif // TRACKERPARAMETER_H
