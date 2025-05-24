#pragma once

#include <QDialog>
#include <QColor>
#include <QLineEdit>
#include <QPushButton>
#include <QVector>

class TierSettingsDialog : public QDialog {
    // we want this to be known as a dialog pop-up!!!, is-a relation
    Q_OBJECT

private:
    QLineEdit* nameEdit;
    QColor selectedColor;
    QVector<QPushButton*> colorButtons;


public:
    TierSettingsDialog(const QString& currentName, 
                    const QColor& currentColor, 
                    QWidget* parent = nullptr);
    QString getNewName() const;
    QColor getSelectedColor() const;

signals: // like events
    void deleteRow();
    void clearImages();
    void addRowAbove();
    void addRowBelow();
    void colorChanged(const QColor&);

private slots:
    void onColorSelected();
    void emitDelete();
    void emitClear();
    void emitAddAbove();
    void emitAddBelow();

};