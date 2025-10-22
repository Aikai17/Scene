#pragma once
#include <QObject>
#include <QVector3D>
#include <QColor>
#include <QString>

class Primitive : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(QVector3D position READ position CONSTANT)
    Q_PROPERTY(QVector3D size READ size CONSTANT)
    Q_PROPERTY(QColor color READ color CONSTANT)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)

public:
    enum PrimitiveType { Box, Sphere, Pyramid };

    Primitive(PrimitiveType type, const QVector3D& position,
              const QVector3D& size, const QColor& color, QObject* parent = nullptr);

    QString type() const;
    QVector3D position() const;
    QVector3D size() const;
    QColor color() const;
    bool selected() const;
    void setSelected(bool selected);

signals:
    void selectedChanged(bool selected);

private:
    PrimitiveType m_type;
    QVector3D m_position;
    QVector3D m_size;
    QColor m_color;
    bool m_selected = false;
};
