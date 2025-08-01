#pragma once

#include <QQuickFramebufferObject>

class SimpleOsgEarthItem : public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(QString earthFile READ earthFile WRITE setEarthFile NOTIFY earthFileChanged)

public:
    explicit SimpleOsgEarthItem(QQuickItem* parent = nullptr);
    Renderer* createRenderer() const override;

    QString earthFile() const { return m_earthFile; }
    void setEarthFile(const QString& file);

signals:
    void earthFileChanged();

private:
    QString m_earthFile;
};
