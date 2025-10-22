#ifndef PRIMITIVEFACTORY_H
#define PRIMITIVEFACTORY_H

#include <QObject>
#include <QThread>
#include <QVector3D>
#include <QColor>
#include <QList>
#include "Primitive.h"

class PrimitiveFactory : public QObject
{
    Q_OBJECT

public:
    explicit PrimitiveFactory(QObject* parent = nullptr);

public slots:
    void generatePrimitives(const QString& type, int count,
                            float width, float height, float depth);

signals:
    void primitivesGenerated(const QList<Primitive*>& primitives);
    void generationFinished();

private:
    Primitive* createPrimitive(const QString& type,
                               const QVector3D& position,
                               const QVector3D& size,
                               const QColor& color);
};



#endif // PRIMITIVEFACTORY_H
