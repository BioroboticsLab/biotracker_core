#ifndef ICONTROLLERSTRATEGY_H
#define ICONTROLLERSTRATEGY_H

#include <QObject>
#include "Interfaces/IView.h"
#include "Interfaces/imodel.h"

class IController;
class IControllerStrategy : public QObject
{
    Q_OBJECT
public:
    explicit IControllerStrategy(QObject *parent = 0, IController *controller = 0);

    virtual void execute() = 0;

Q_SIGNALS:

public Q_SLOTS:

protected:
    IController *m_Controller;
};

#endif // ICONTROLLERSTRATEGY_H
