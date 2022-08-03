#ifndef BIOTRACKER3APP_H
#define BIOTRACKER3APP_H

#include <QObject>
#include "QString"
#include "Interfaces/IBioTrackerContext.h"
#include "QPointer"

class BioTracker3App : public QObject
{
    Q_OBJECT
public:
    explicit BioTracker3App(QObject* parent = 0);

    void                runBioTracker();
    void                setBioTrackerContext(IBioTrackerContext* context);
    IBioTrackerContext* getBioTrackerContext();

Q_SIGNALS:

public Q_SLOTS:

private:
    QPointer<IBioTrackerContext> m_BioTrackerContext;
};

#endif // BIOTRACKER3APP_H
