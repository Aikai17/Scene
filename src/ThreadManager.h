#ifndef THREADMANAGER_H
#define THREADMANAGER_H
#include <QObject>
#include "PrimitiveFactory.h"

class ThreadManager : public QObject
{
    Q_OBJECT

public:
    explicit ThreadManager(QObject* parent = nullptr);
    ~ThreadManager();

    Q_INVOKABLE void generateInThread(const QString& type, int count,
                                      float width, float height, float depth);

signals:
    void startGeneration(const QString& type, int count,
                         float width, float height, float depth);
    void primitivesReady(const QList<Primitive*>& primitives);

private:
    QThread* m_workerThread;
    PrimitiveFactory* m_factory;
};
#endif // THREADMANAGER_H
