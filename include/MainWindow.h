#pragma once
#include <QMainWindow>
#include <QPalette>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include "Tier.h"
#include "TierSettingsDialog.h"

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
    void onSaveTemplate();
    void onLoadTemplate();

private:
    QVBoxLayout* mainLayout;
    QHBoxLayout* sourceItemsLayout;
    //QWidget* sourceItemsContainer;  // the widget inside the scroll area
    DropFrame* sourceItemsContainer;

    //QVBoxLayout* mainLayout;
    QVector<TierRow*> tierRows;

    QHBoxLayout* templateButtonLayout;
    QPushButton* loadButton ;

    void insertRowRelativeTo(TierRow* referenceRow, bool above);
    void loadTemplate(const QString& path);
    void saveTemplate(const QString& name);
};

