#pragma once
#include <QMainWindow>
#include <QPalette>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include "Tier.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void loadImages();
    void onRowMoveUp(TierRow* row);
    void onRowMoveDown(TierRow* row);
    void onOpenRowSettings(TierRow* row);

private:
    QVBoxLayout* mainLayout;
    QHBoxLayout* sourceItemsLayout;
    //QWidget* sourceItemsContainer;  // the widget inside the scroll area
    DropFrame* sourceItemsContainer;

    //QVBoxLayout* mainLayout;
    QVector<TierRow*> tierRows;
};