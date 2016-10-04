#ifndef IPLAYERSTRATEGY_H
#define IPLAYERSTRATEGY_H

#include <QObject>

class IPlayerStrategy : public QObject
{
    Q_OBJECT
public:
    explicit IPlayerStrategy(QObject *parent = 0);

signals:

public slots:
};

#endif // IPLAYERSTRATEGY_H