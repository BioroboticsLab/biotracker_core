#ifndef BIOTRACKER3APP_H
#define BIOTRACKER3APP_H

#include <QObject>
#include "QString"
#include "Interfaces/IBioTrackerContext.h"
class BioTracker3App : public QObject
{
    Q_OBJECT
public:
    explicit BioTracker3App(QObject *parent = 0);

    void runBioTracker();
    void setBioTrackerContext(IBioTrackerContext *context);
    IBioTrackerContext *getBioTrackerContext();

Q_SIGNALS:

public Q_SLOTS:

private:
    IBioTrackerContext *m_BioTrackerContext;

};

#endif // BIOTRACKER3APP_H
