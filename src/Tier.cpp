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
#include <QIcon>


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
: QWidget(parent), name{name}
{
    setAcceptDrops(true);
    mainLayout = new QHBoxLayout(this);

    // create the Tier Name Label
    tierLabel = new QLineEdit(name, this);
    tierLabel->setFixedWidth(140);
    tierLabel->setAlignment(Qt::AlignCenter);

    tierLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    // QString style = QString(R"(
    //     font-weight: bold;
    //     font-size: 17pt;
    //     color: black;
    //     background-color: %1;
    //     border: none;
    //     padding: 10px;
    // )").arg(color.name());

    // // tierLabel->setStyleSheet(R"(
    // // font-weight: bold;
    // // font-size: 20pt;
    // // color: black;
    // // )");

    setTierColor(color);

    // tierLabel->setStyleSheet(style);

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
    dropArea = new QFrame(this);
    dropArea->setFrameShape(QFrame::StyledPanel);
    dropArea->setMinimumHeight(80);
    dropArea->setAcceptDrops(true);
    dropArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    dropLayout = new QHBoxLayout(dropArea);
    dropLayout->setAlignment(Qt::AlignLeft);  // 💥 This is the fix
    dropArea->setLayout(dropLayout);
    dropLayout->setSpacing(0);
    dropLayout->setContentsMargins(0, 0, 0, 0);  

    // connect signal from DropFrame
    // connect(dropArea, &DropFrame::itemDropped, this, [this](const QString& imagePath) {
    // dropLayout->addWidget(new TierItem(imagePath, this)); });

    QIcon settingsIcon("resources/settings.png");
    QIcon upArrowIcon("resources/upArrow.png");
    QIcon downArrowIcon("resources/downArrow.png");


    // The configuration and arrows
    settingsButton = new QPushButton(this);
    settingsButton->setIcon(settingsIcon);
    settingsButton->setIconSize(QSize(20, 20));
    connect(settingsButton, &QPushButton::clicked, this, &TierRow::onSettingsClicked);

    upButton = new QPushButton(this);
    upButton->setIcon(upArrowIcon);
    upButton->setIconSize(QSize(20, 20));
    connect(upButton, &QPushButton::clicked, this, &TierRow::onMoveUpClicked);

    downButton = new QPushButton(this);
    downButton->setIcon(downArrowIcon);
    downButton->setIconSize(QSize(20, 20));
    connect(downButton, &QPushButton::clicked, this, &TierRow::onMoveDownClicked);

    // Style buttons (optional)
    settingsButton->setFixedSize(30, 30);
    upButton->setFixedSize(30, 30);
    downButton->setFixedSize(30, 30);



    // add widgets
    mainLayout->addWidget(tierLabel);
    mainLayout->addWidget(dropArea);

    QVBoxLayout* controlLayout = new QVBoxLayout();
    controlLayout->addWidget(settingsButton);
    controlLayout->addWidget(upButton);
    controlLayout->addWidget(downButton);
    mainLayout->addLayout(controlLayout);

    setLayout(mainLayout);

    QPalette bgPalette = palette();
    bgPalette.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(bgPalette);
}

void TierRow::onSettingsClicked() {
    emit openSettings(this); // emits the signal, cool!!!
}

void TierRow::onMoveUpClicked() {
    emit moveUp(this);
}

void TierRow::onMoveDownClicked() {
    emit moveDown(this);
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
    //dropLayout->insertWidget(0, item); // nope, this puts the element in fist posn to left

    event->acceptProposedAction();
}

void TierRow::clearImages()
{
    int count = dropLayout->count();

    qDebug() << "Clearing" << count << "items from dropLayout";

    for (int i = count - 1; i >= 0; --i) {
        QLayoutItem* item = dropLayout->takeAt(i);
        if (!item) continue;

        QWidget* widget = item->widget();
        if (widget) {
            qDebug() << "Deleting widget at position" << i;
            widget->hide();        // Optional: visually hide before delete
            widget->deleteLater(); // Safe Qt deletion
        }

        delete item;
    }
}





// ******************* ITEMS **********************
TierItem::TierItem(const QString& imagePath, QWidget* parent)
    : QLabel{parent}, imagePath(imagePath)
{
    QPixmap pix(imagePath);
    setPixmap(pix.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(80, 80);
    setStyleSheet(R"(
        background-color: lightgray;
        border: 1px solid black;
        margin: 0px;
        padding: 0px;
    )");
    
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
    //setLayout(new QHBoxLayout(this));
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
    emit itemDropped(imagePath);
    layout->addWidget(new TierItem(imagePath, this));
    // layout->insertWidget(0, new TierItem(imagePath, this)); // nope
    event->acceptProposedAction();
    event->setDropAction(Qt::MoveAction);
    //event->accept();
}