#pragma once

#include <QWidget>
#include <QColor>
#include <QString>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>
#include <QHBoxLayout>
#include <QFrame>
#include <QPixmap>
#include <QLineEdit>
#include <QPushButton>

class Tier : public QWidget 
{
    Q_OBJECT

    QString tierName;
    QColor tierColor;

public:
    Tier(const QString& tierName, const QColor& tierColor, 
        QWidget* parent = nullptr);
    
    QString getTierName() const;
    QColor getTierColor() const;

protected:
    // Draws the colored background and name inside
    void paintEvent(QPaintEvent* event) override;
};

class DropFrame : public QFrame
{
    Q_OBJECT

public:
    DropFrame(QWidget* parent = nullptr);
    QHBoxLayout* getLayout();

signals:
    void itemDropped(const QString& imagePath);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    QHBoxLayout* layout;
};

class TierRow : public QWidget
{
    Q_OBJECT

    QString name;
    QColor color;
    QLineEdit* tierLabel;
    QFrame* dropArea;
    QHBoxLayout* mainLayout;
    QHBoxLayout* dropLayout; // layout inside dropArea

    QPushButton* settingsButton;
    QPushButton* upButton;
    QPushButton* downButton;

public:
    TierRow(const QString& name, const QColor& color, QWidget* parent = nullptr);

    QString getTierName(){
        return name;
    }

    QColor getTierColor(){
        return color;
    }

    void setTierName(const QString& newName){
        name = newName;
    }

    void setTierColor(const QColor& newColor){
        color = newColor;
        QString style = QString(R"(
            font-weight: bold;
            font-size: 20pt;
            color: black;
            background-color: %1;
            border: none;
            padding: 10px;
        )").arg(color.name());
    
        tierLabel->setStyleSheet(style);
    }

    void clearImages();



signals:
    void moveUp(TierRow*);
    void moveDown(TierRow*);
    void openSettings(TierRow*);

private slots:
    void onSettingsClicked();
    void onMoveUpClicked();
    void onMoveDownClicked();


protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

};

class TierItem : public QLabel
{
    Q_OBJECT

    QString imagePath;

public:
    TierItem(const QString& imagePath, QWidget* parent = nullptr);
    QString getImagePath() const;


protected:
    void mousePressEvent(QMouseEvent* event) override;
};
