#include "ScrollHandle.h"
#include <QMouseEvent>

ScrollHandle::ScrollHandle(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(30);  // Adjust as needed
    setStyleSheet("background-color: #888;");  // Visible handle
    setCursor(Qt::SizeVerCursor);  // Vertical drag indicator
}

void ScrollHandle::mousePressEvent(QMouseEvent* event)
{
    lastPos = event->pos();
}

void ScrollHandle::mouseMoveEvent(QMouseEvent* event)
{
    int dy = event->pos().y() - lastPos.y();
    emit dragDelta(dy);
    lastPos = event->pos();
}