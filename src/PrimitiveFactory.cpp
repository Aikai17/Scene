#include "PrimitiveFactory.h".h"
#include <QRandomGenerator>
#include <QDebug>

PrimitiveFactory::PrimitiveFactory(QObject* parent) : QObject(parent) {}

void PrimitiveFactory::generatePrimitives(const QString& type, int count,
                                          float width, float height, float depth) {
    qDebug() << "Worker thread:" << QThread::currentThread();

    QList<Primitive*> primitives;
    QRandomGenerator* random = QRandomGenerator::global();

    for (int i = 0; i < count; ++i) {
        QVector3D position(
            random->bounded(-300, 300),
            random->bounded(-300, 300),
            random->bounded(-300, 300)
            );

        QColor color(
            random->bounded(256),
            random->bounded(256),
            random->bounded(256)
            );

        // Имитация сложных вычислений
        QThread::msleep(10);

        primitives.push_back(createPrimitive(type, position,
                        QVector3D(width, height, depth), color));

    }

    qDebug() << "Generated" << primitives.size() << "primitives in worker thread";
    emit primitivesGenerated(primitives);
    emit generationFinished();
}

Primitive* PrimitiveFactory::createPrimitive(const QString& type,
                                             const QVector3D& position,
                                             const QVector3D& size,
                                             const QColor& color) {
    Primitive::PrimitiveType primitiveType;

    if (type == "Sphere") primitiveType = Primitive::Sphere;
    else if (type == "Pyramid") primitiveType = Primitive::Pyramid;
    else primitiveType = Primitive::Box;

    return new Primitive(primitiveType, position, size, color);
}


