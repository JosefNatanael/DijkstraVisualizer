#ifndef VERTEX_H
#define VERTEX_H

#include <QLabel>
#include <QPainter>

class Vertex : public QLabel
{
    Q_OBJECT
public:
    explicit Vertex(int xPos, int yPos, QWidget *parent = nullptr);

    void setPos(QPoint);
    QPoint getPos() const;

signals:

protected slots:
    void paintEvent(QPaintEvent *event) override;

private:
    int dummy = 5;
    int xPos;
    int yPos;
};

#endif // VERTEX_H
