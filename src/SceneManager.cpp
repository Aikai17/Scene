#include "SceneManager.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QRandomGenerator>

SceneManager::SceneManager(QObject* parent)
    : QAbstractListModel(parent)
    , m_threadManager(new ThreadManager(this))
{
    connect(m_threadManager, &ThreadManager::primitivesReady,
            this, &SceneManager::handlePrimitivesGenerated);

    qDebug() << "SceneManager created";
}

SceneManager::~SceneManager() {
    clearScene();
}

int SceneManager::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : m_primitives.size();
}

QVariant SceneManager::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_primitives.size())
        return QVariant();

    Primitive* primitive = m_primitives[index.row()];

    switch (role) {
    case TypeRole: return primitive->type();
    case PositionRole: return primitive->position();
    case SizeRole: return primitive->size();
    case ColorRole: return primitive->color();
    case SelectedRole: return primitive->selected();
    default: return QVariant();
    }
}

QHash<int, QByteArray> SceneManager::roleNames() const {
    return {
        {TypeRole, "type"},
        {PositionRole, "position"},
        {SizeRole, "size"},
        {ColorRole, "color"},
        {SelectedRole, "selected"},
        {IdRole, "id"}
    };
}

void SceneManager::addPrimitives(const QString& type, int count,
                                 float width, float height, float depth) {
    qDebug() << "Adding" << count << type << "primitives. Main thread:" << QThread::currentThread();

    if (count <= 0) {
        qWarning() << "Invalid count:" << count;
        return;
    }

    emit generationStarted();
    m_threadManager->generateInThread(type, count, width, height, depth);
}

void SceneManager::handlePrimitivesGenerated(const QList<Primitive*>& primitives) {
    qDebug() << "Received" << primitives.size() << "primitives in main thread:" << QThread::currentThread();

    if (primitives.isEmpty()) {
        emit generationFinished();
        return;
    }

    beginInsertRows(QModelIndex(), m_primitives.size(),
                    m_primitives.size() + primitives.size() - 1);

    for (Primitive* primitive : primitives) {
        m_primitives.append(primitive);
    }

    endInsertRows();
    emit generationFinished();

    qDebug() << "Total primitives in scene:" << m_primitives.size();
}

void SceneManager::clearScene() {
    if (m_primitives.isEmpty()) return;

    beginResetModel();
    qDeleteAll(m_primitives);
    m_primitives.clear();
    m_nextId = 0;
    endResetModel();

    emit sceneCleared();
    qDebug() << "Scene cleared";
}

void SceneManager::selectPrimitive(int index) {
    if (index < 0 || index >= m_primitives.size()) {
        qWarning() << "Invalid selection index:" << index;
        return;
    }

    QVector<int> changedIndexes;

    // Находим все примитивы у которых изменилось состояние выделения
    for (int i = 0; i < m_primitives.size(); ++i) {
        bool wasSelected = m_primitives[i]->selected();
        bool shouldBeSelected = (i == index);

        if (wasSelected != shouldBeSelected) {
            m_primitives[i]->setSelected(shouldBeSelected);
            changedIndexes.append(i);
        }
    }

    // Уведомляем View об изменениях
    if (!changedIndexes.isEmpty()) {
        for (int idx : changedIndexes) {
            QModelIndex modelIndex = createIndex(idx, 0);
            emit dataChanged(modelIndex, modelIndex, {SelectedRole});
        }
        qDebug() << "Selection updated. Selected index:" << index;
    }
}

bool SceneManager::saveScene(const QString& filename) {
    QJsonArray primitivesArray;

    for (Primitive* primitive : m_primitives) {
        primitivesArray.append(primitiveToJson(primitive));
    }

    QJsonObject sceneObject;
    sceneObject["version"] = "1.0";
    sceneObject["primitivesCount"] = m_primitives.size();
    sceneObject["primitives"] = primitivesArray;

    QJsonDocument doc(sceneObject);

    QUrl url(filename);
    QString localFilePath = url.toLocalFile();

    QFile file(localFilePath);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not save scene to" << localFilePath
                   << "Error:" << file.errorString();
        return false;
    }

    file.write(doc.toJson());
    file.close();

    qDebug() << "Scene saved to" << localFilePath << "with" << m_primitives.size() << "primitives";
    return true;
}

bool SceneManager::loadScene(const QString& filename) {
    QUrl url(filename);
    QString localFilePath = url.toLocalFile();

    QFile file(localFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not load scene from" << localFilePath
                   << "Error:" << file.errorString();
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull()) {
        qWarning() << "Invalid JSON file";
        return false;
    }

    QJsonObject sceneObject = doc.object();
    QJsonArray primitivesArray = sceneObject["primitives"].toArray();

    beginResetModel();
    clearScene();

    for (const QJsonValue& value : primitivesArray) {
        Primitive* primitive = createPrimitiveFromJson(value.toObject());
        if (primitive) {
            primitive->setParent(this);
            primitive->setProperty("id", m_nextId++);
            m_primitives.append(primitive);
        }
    }

    endResetModel();

    qDebug() << "Scene loaded from" << filename << "with" << m_primitives.size() << "primitives";
    return true;
}

QJsonObject SceneManager::primitiveToJson(Primitive* primitive) const {
    QJsonObject json;
    json["type"] = primitive->type();

    QJsonObject position;
    position["x"] = primitive->position().x();
    position["y"] = primitive->position().y();
    position["z"] = primitive->position().z();
    json["position"] = position;

    QJsonObject size;
    size["width"] = primitive->size().x();
    size["height"] = primitive->size().y();
    size["depth"] = primitive->size().z();
    json["size"] = size;

    QJsonObject color;
    color["r"] = primitive->color().red();
    color["g"] = primitive->color().green();
    color["b"] = primitive->color().blue();
    color["a"] = primitive->color().alpha();
    json["color"] = color;

    json["selected"] = primitive->selected();

    return json;
}

Primitive* SceneManager::createPrimitiveFromJson(const QJsonObject& json) {
    QString type = json["type"].toString();

    QJsonObject positionObj = json["position"].toObject();
    QVector3D position(
        positionObj["x"].toDouble(),
        positionObj["y"].toDouble(),
        positionObj["z"].toDouble()
        );

    QJsonObject sizeObj = json["size"].toObject();
    QVector3D size(
        sizeObj["width"].toDouble(),
        sizeObj["height"].toDouble(),
        sizeObj["depth"].toDouble()
        );

    QJsonObject colorObj = json["color"].toObject();
    QColor color(
        colorObj["r"].toInt(),
        colorObj["g"].toInt(),
        colorObj["b"].toInt(),
        colorObj["a"].toInt()
        );

    Primitive::PrimitiveType primitiveType;
    if (type == "Sphere") primitiveType = Primitive::Sphere;
    else if (type == "Pyramid") primitiveType = Primitive::Pyramid;
    else primitiveType = Primitive::Box;

    return new Primitive(primitiveType, position, size, color);
}
