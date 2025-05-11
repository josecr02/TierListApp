#pragma once

#include "Tier.h"
#include <QPainter>
#include <QPaintEvent>
#include <QFrame>
#include <QPalette>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDebug>


Tier::Tier(const QString& tierName, const QColor& tierColor, 
    QWidget* parent) : QWidget{parent}, tierName{tierName}, tierColor{tierColor} {
        setMinimumHeight(80);
}
    
QString Tier::getTierName() const{
    return tierName;
}
QColor Tier::getTierColor() const{
    return tierColor;
}

// Draws the colored background and name inside
void Tier::paintEvent(QPaintEvent* event) 
{
    QPainter painter(this);

    // background
    painter.fillRect(rect(), tierColor);

    // text
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 20));
    painter.drawText(rect(), Qt::AlignCenter, tierName);
}


// *************** ROWS ****************

TierRow::TierRow(const QString& name, const QColor& color, QWidget* parent)
: QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);

    // create the Tier Name Label
    tierLabel = new QLineEdit(name, this);
    tierLabel->setFixedWidth(140);
    tierLabel->setAlignment(Qt::AlignCenter);

    tierLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    QString style = QString(R"(
        font-weight: bold;
        font-size: 17pt;
        color: black;
        background-color: %1;
        border: none;
        padding: 10px;
    )").arg(color.name());

    // tierLabel->setStyleSheet(R"(
    // font-weight: bold;
    // font-size: 20pt;
    // color: black;
    // )");

    tierLabel->setStyleSheet(style);

    // can instead of doing the palette with background-color

    // QFont font = tierLabel->font();
    // font.setPointSize(20);
    // font.setBold(true);
    // tierLabel->setFont(font);

    // set background color (row)
    QPalette tierPalette = tierLabel->palette();
    tierPalette.setColor(QPalette::Window, color);
    tierLabel->setAutoFillBackground(true);
    tierLabel->setPalette(tierPalette);

    // create drop area
    dropArea = new DropFrame(this);
    dropArea->setFrameShape(QFrame::StyledPanel);
    dropArea->setMinimumHeight(80);
    dropArea->setAcceptDrops(true);

    dropLayout = new QHBoxLayout(dropArea);
    dropArea->setLayout(dropLayout);

    // add widgets
    mainLayout->addWidget(tierLabel);
    mainLayout->addWidget(dropArea);

    setLayout(mainLayout);

    QPalette bgPalette = palette();
    bgPalette.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(bgPalette);
}

void TierRow::dragEnterEvent(QDragEnterEvent* event){
    if (event->mimeData()->hasText()){
        event->acceptProposedAction();
    }
}

void TierRow::dropEvent(QDropEvent* event){
    qDebug() << "dropping item!";
    QString imagePath = event->mimeData()->text();

    TierItem* item = new TierItem(imagePath, this);
    dropLayout->addWidget(item);

    event->acceptProposedAction();
}



// ******************* ITEMS **********************
TierItem::TierItem(const QString& imagePath, QWidget* parent)
    : QLabel{parent}, imagePath(imagePath)
{
    QPixmap pix(imagePath);
    setPixmap(pix.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setFixedSize(80, 80);
    setStyleSheet("border: 1px solid black;");
    
}

QString TierItem::getImagePath() const {
    return imagePath;
}

void TierItem::mousePressEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton){
        QMimeData* mimeData = new QMimeData;
        mimeData->setText(imagePath);

        // create drag object
        QDrag* drag = new QDrag(this);
        drag->setMimeData(mimeData);

        // show pixmap of item while dragging
        drag->setPixmap(*pixmap());
        drag->setHotSpot(event->pos());

        Qt::DropAction result = drag->exec(Qt::MoveAction);

        // start drag
        //drag->exec(Qt::MoveAction);

        if (result == Qt::MoveAction){
            this->deleteLater();
        }
    }
}

DropFrame::DropFrame(QWidget* parent) : QFrame(parent)
{
    setAcceptDrops(true);
    layout = new QHBoxLayout(this);
    setLayout(layout);
}

QHBoxLayout* DropFrame::getLayout()
{
    return layout;
}

void DropFrame::dragEnterEvent(QDragEnterEvent* event){
    qDebug() << "dragEnterEvent: DropFrame";
    if (event->mimeData()->hasText())
        event->acceptProposedAction();
}

void DropFrame::dropEvent(QDropEvent* event){
    qDebug() << "dropEvent: DropFrame";
    QString imagePath = event->mimeData()->text();
    layout->addWidget(new TierItem(imagePath, this));
    //event->acceptProposedAction();
    event->setDropAction(Qt::MoveAction);
    event->accept();
}