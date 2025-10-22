#include "Primitive.h"
#include <QDebug>

Primitive::Primitive(PrimitiveType type, const QVector3D &position, const QVector3D &size, const QColor &color, QObject *parent)
    : QObject(parent)
    , m_type(type)
    , m_position(position)
    , m_size(size)
    , m_color(color)
{

}


QVector3D Primitive::position() const
{
    return m_position;
}

QVector3D Primitive::size() const
{
    return m_size;
}

QColor Primitive::color() const
{
    return m_color;
}

bool Primitive::selected() const
{
    return m_selected;
}

void Primitive::setSelected(bool selected)
{
    if (m_selected != selected) {
        m_selected = selected;
        qDebug() << "Primitive selection changed to:" << selected;
        emit selectedChanged(selected);
    }
}

QString Primitive::type() const
{
    switch(m_type){
    case Box: return "Box";
    case Sphere: return "Sphere";
    case Pyramid: return "Pyramid";
    default: return "Unknown";
    }
}
