#ifndef IMODEL_H
#define IMODEL_H

#include <QObject>

class IModel : public QObject {
    Q_OBJECT
  public:
    explicit IModel(QObject *parent = 0);
    ~IModel();

  Q_SIGNALS:

    void notifyView();

  public Q_SLOTS:
};

#endif // IMODEL_H
