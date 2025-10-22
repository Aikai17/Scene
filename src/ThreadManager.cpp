#include "ThreadManager.h"
#include <QDebug>

ThreadManager::ThreadManager(QObject* parent) : QObject(parent) {
    m_workerThread = new QThread(this);
    m_factory = new PrimitiveFactory();

    m_factory->moveToThread(m_workerThread);

    connect(m_workerThread, &QThread::finished, m_factory, &QObject::deleteLater);
    connect(this, &ThreadManager::startGeneration,
            m_factory, &PrimitiveFactory::generatePrimitives);
    connect(m_factory, &PrimitiveFactory::primitivesGenerated,
            this, &ThreadManager::primitivesReady);

    m_workerThread->start();

    qDebug() << "ThreadManager created. Worker thread:" << m_workerThread;
}

ThreadManager::~ThreadManager() {
    m_workerThread->quit();
    m_workerThread->wait();
}

void ThreadManager::generateInThread(const QString& type, int count,
                                     float width, float height, float depth) {
    qDebug() << "Requesting generation in thread. Main thread:" << QThread::currentThread();
    emit startGeneration(type, count, width, height, depth);
}
