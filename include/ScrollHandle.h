#pragma once
#include <QWidget>

class ScrollHandle : public QWidget
{
    Q_OBJECT

public:
    explicit ScrollHandle(QWidget* parent = nullptr);

signals:
    void dragDelta(int dy);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QPoint lastPos;
};
