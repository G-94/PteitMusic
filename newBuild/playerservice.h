#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <QObject>

class PlayerService : public QObject
{
    Q_OBJECT
public:
    explicit PlayerService(QObject *parent = nullptr);

signals:
};

#endif // PLAYERSERVICE_H
