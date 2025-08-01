#ifndef OSGEARTHITEM2_H
#define OSGEARTHITEM2_H

#include <QQuickFramebufferObject>

class OsgEarthItem2 : public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(QString earthFile READ earthFile WRITE setEarthFile NOTIFY earthFileChanged)

public:
    explicit OsgEarthItem2(QQuickItem* parent = nullptr);
    Renderer* createRenderer() const override;

    QString earthFile() const { return m_earthFile; }
    void setEarthFile(const QString& file);

signals:
    void earthFileChanged();

private:
    QString m_earthFile;
};

#endif // OSGEARTHITEM2_H
