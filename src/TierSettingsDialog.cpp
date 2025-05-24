#include "TierSettingsDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QColor>

TierSettingsDialog::TierSettingsDialog(const QString& currentName, const QColor& currentColor, QWidget* parent)
    : QDialog(parent), selectedColor(currentColor)
{
    setWindowTitle("Tier Settings");
    setModal(true);
    setMinimumWidth(400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Color picker row
    QHBoxLayout* colorLayout = new QHBoxLayout();
    QVector<QColor> colors = {
        QColor("#f87171"), QColor("#fbbf24"), QColor("#facc15"), QColor("#a3e635"),
        QColor("#34d399"), QColor("#22d3ee"), QColor("#60a5fa"), QColor("#c084fc"),
        QColor("#e879f9"), QColor("#9ca3af"), QColor("#6b7280"), QColor("#f3f4f6")
    };

    for (const QColor& color : colors) {
        QPushButton* btn = new QPushButton(this);
        btn->setFixedSize(24, 24);
        btn->setStyleSheet(QString("border-radius: 12px; background-color: %1;").arg(color.name()));
        btn->setProperty("color", color.name());
        connect(btn, &QPushButton::clicked, this, &TierSettingsDialog::onColorSelected);
        colorButtons.append(btn);
        colorLayout->addWidget(btn);
    }

    mainLayout->addLayout(colorLayout);

    // Row name editor
    nameEdit = new QLineEdit(currentName, this);
    mainLayout->addWidget(nameEdit);

    // Row action buttons
    QGridLayout* grid = new QGridLayout();

    QPushButton* deleteBtn = new QPushButton("Delete Row", this);
    QPushButton* clearBtn = new QPushButton("Clear Row Images", this);
    QPushButton* aboveBtn = new QPushButton("Add a Row Above", this);
    QPushButton* belowBtn = new QPushButton("Add a Row Below", this);

    connect(deleteBtn, &QPushButton::clicked, this, &TierSettingsDialog::emitDelete);
    connect(clearBtn, &QPushButton::clicked, this, &TierSettingsDialog::emitClear);
    connect(aboveBtn, &QPushButton::clicked, this, &TierSettingsDialog::emitAddAbove);
    connect(belowBtn, &QPushButton::clicked, this, &TierSettingsDialog::emitAddBelow);

    grid->addWidget(deleteBtn, 0, 0);
    grid->addWidget(clearBtn, 0, 1);
    grid->addWidget(aboveBtn, 1, 0);
    grid->addWidget(belowBtn, 1, 1);

    mainLayout->addLayout(grid);
}

void TierSettingsDialog::onColorSelected()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn) {
        selectedColor = QColor(btn->property("color").toString());
        emit colorChanged(selectedColor);
    }
}

QString TierSettingsDialog::getNewName() const {
    return nameEdit->text();
}

QColor TierSettingsDialog::getSelectedColor() const {
    return selectedColor;
}

void TierSettingsDialog::emitDelete() { emit deleteRow(); accept(); }
void TierSettingsDialog::emitClear()  { emit clearImages(); accept(); }
void TierSettingsDialog::emitAddAbove() { emit addRowAbove(); accept(); }
void TierSettingsDialog::emitAddBelow() { emit addRowBelow(); accept(); }