#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include <QVector3D>
#include <QColor>
#include <QList>
#include "Primitive.h"
#include "PrimitiveFactory.h"
#include "ThreadManager.h"


class SceneManager : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        TypeRole = Qt::UserRole + 1,
        PositionRole,
        SizeRole,
        ColorRole,
        SelectedRole,
        IdRole
    };

    explicit SceneManager(QObject* parent = nullptr);
    ~SceneManager();

    // QAbstractListModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addPrimitives(const QString& type, int count,
                                   float width, float height, float depth);
    Q_INVOKABLE void clearScene();
    Q_INVOKABLE void selectPrimitive(int index);
    Q_INVOKABLE bool saveScene(const QString& filename);
    Q_INVOKABLE bool loadScene(const QString& filename);

    Q_INVOKABLE int getPrimitiveCount() const { return m_primitives.size(); }

signals:
    void sceneCleared();
    void generationStarted();
    void generationFinished();

private slots:
    void handlePrimitivesGenerated(const QList<Primitive*>& primitives);

private:
    QList<Primitive*> m_primitives;
    ThreadManager* m_threadManager;
    int m_nextId = 0;

    Primitive* createPrimitiveFromJson(const QJsonObject& json);
    QJsonObject primitiveToJson(Primitive* primitive) const;
};

#endif // SCENEMANAGER_H
