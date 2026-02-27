#ifndef HISTORYSERVICE_H
#define HISTORYSERVICE_H

#include <QObject>

class HistoryService : public QObject
{
    Q_OBJECT
public:
    explicit HistoryService(QObject *parent = nullptr);

signals:
};

#endif // HISTORYSERVICE_H
