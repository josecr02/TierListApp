#include "MainWindow.h"
#include "Tier.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QInputDialog>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Tier List Creator");

    resize(1000, 750);

    // set the central widget
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    mainLayout = new QVBoxLayout(central);
    central->setLayout(mainLayout);

    // mainLayout->addWidget(new TierRow("S Tier", QColor(255, 67, 89), this)); // gold color
    // mainLayout->addWidget(new TierRow("A Tier", QColor(255, 152, 67), this)); // silver
    // mainLayout->addWidget(new TierRow("B Tier", QColor(250, 255, 64), this)); // bronze
    // mainLayout->addWidget(new TierRow("C Tier", QColor(96, 237, 75), this)); // light blue
    // mainLayout->addWidget(new TierRow("D Tier", QColor(99, 254, 248), this)); // light salmon

    // generate default rows
    for (int i = 0; i < 5; ++i){
        TierRow* row = new TierRow("S Tier", QColor(255, 67, 89), this);
        mainLayout->addWidget(row);
        tierRows.append(row);
        connect(row, &TierRow::moveUp, this, &MainWindow::onRowMoveUp);
        connect(row, &TierRow::moveDown, this, &MainWindow::onRowMoveDown);
        connect(row, &TierRow::openSettings, this, &MainWindow::onOpenRowSettings);
    }
    



    // Button to load images
    QPushButton* loadButton = new QPushButton("Load Images", this);
    mainLayout->addWidget(loadButton);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadImages);


    // Add template buttons 
    QPushButton* loadTemplateBtn = new QPushButton("Choose from Template", this);
    QPushButton* saveTemplateBtn = new QPushButton("Save as Template", this);

    QHBoxLayout* templateButtonLayout = new QHBoxLayout();
    templateButtonLayout->addWidget(loadTemplateBtn);
    templateButtonLayout->addWidget(saveTemplateBtn);

    mainLayout->addLayout(templateButtonLayout);

    // now we connect the buttons
    connect(loadTemplateBtn, &QPushButton::clicked, this, &MainWindow::onLoadTemplate);
    connect(saveTemplateBtn, &QPushButton::clicked, this, &MainWindow::onSaveTemplate);

// ⬇️ Source item container
    // sourceItemsContainer = new QWidget(this);
    // sourceItemsLayout = new QHBoxLayout(sourceItemsContainer);
    // sourceItemsContainer->setLayout(sourceItemsLayout);


    // also a drop frame, to make it act like a bank!
    sourceItemsContainer = new DropFrame(this);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(sourceItemsContainer);
    scrollArea->setMinimumHeight(120);
    scrollArea->setFrameShape(QFrame::NoFrame);
    mainLayout->addWidget(scrollArea);

    // KEEP changing from here, STEP 2 chatgpt

    

   // layout->addWidget(new TierItem("resources/burger.png", this));
   // layout->addWidget(new TierItem("resources/pizza.jpg", this));
    //layout->addWidget(new TierItem("resources/paint.png", this));
    // layout->addWidget(new TierItem("🍟", this));

    
    // // Container for source images
    // sourceItemsLayout = new QHBoxLayout();
    // QWidget* sourceWidget = new QWidget(this);
    // sourceWidget->setLayout(sourceItemsLayout);

    // Optional: scroll area if you want it to be scrollable
    // QScrollArea* scrollArea = new QScrollArea(this);
    // scrollArea->setWidget(sourceWidget);
    // scrollArea->setWidgetResizable(true);
    // scrollArea->setMinimumHeight(100);
    // mainLayout->addWidget(scrollArea);

    

    QPalette bgPalette = palette();
    bgPalette.setColor(QPalette::Window, Qt::gray);
    setAutoFillBackground(true);
    setPalette(bgPalette);



    // TO-DO: Deconstructor for the central and layout?
}

void MainWindow::loadImages()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "Select Images",
        QDir::homePath(),
        "Images (*.png *.jpg *.jpeg *.bmp *.gif)"
    );

    for (const QString& path : files){
        TierItem* item = new TierItem(path, sourceItemsContainer);
        sourceItemsContainer->getLayout()->addWidget(item);
        // sourceItemsLayout->addWidget(item);
    }
}


// basic functionality, have to change that SWAPS
void MainWindow::onRowMoveUp(TierRow* row) {
    int index = mainLayout->indexOf(row);
    if (index > 0) {
        tierRows.swapItemsAt(index, index - 1);
        mainLayout->removeWidget(row);
        mainLayout->insertWidget(index - 1, row);
    }
}

void MainWindow::onRowMoveDown(TierRow* row) {
    int index = tierRows.indexOf(row);
    if (index < tierRows.size() - 1) {
        tierRows.swapItemsAt(index, index + 1);
        mainLayout->removeWidget(row);
        mainLayout->insertWidget(index + 1, row);
    }
}

void MainWindow::onOpenRowSettings(TierRow* row) {
    TierSettingsDialog* dialog = new TierSettingsDialog(
        row->getTierName(),
        row->getTierColor(),
        this
    );

    connect(dialog, &TierSettingsDialog::colorChanged, row, &TierRow::setTierColor);
    
    connect(dialog, &TierSettingsDialog::deleteRow, this, [this, row]() {
        mainLayout->removeWidget(row);
        tierRows.removeOne(row);
        row->deleteLater();
    });
    
    connect(dialog, &TierSettingsDialog::clearImages, this, [row]() {
        qDebug() << "Clearing images for row:" << row->getTierName();
        row->clearImages();  // you'll need to implement this in TierRow
    });
    
    connect(dialog, &TierSettingsDialog::addRowAbove, this, [this, row]() {
       insertRowRelativeTo(row, /*above=*/true);
    });
    
    connect(dialog, &TierSettingsDialog::addRowBelow, this, [this, row]() {
       insertRowRelativeTo(row, /*above=*/false);
    });
    
    if (dialog->exec() == QDialog::Accepted) {
        row->setTierName(dialog->getNewName());
        row->setTierColor(dialog->getSelectedColor());
    }
}

void MainWindow::insertRowRelativeTo(TierRow* referenceRow, bool above){
    int index = tierRows.indexOf(referenceRow);
    if (index == -1) return;

    int insertIndex = above ? index : index + 1;

    TierRow* newRow = new TierRow("New Tier", QColor("#dddddd"), this);
    mainLayout->insertWidget(insertIndex, newRow);
    tierRows.insert(insertIndex, newRow);

    // Connect signals just like when creating rows initially
    connect(newRow, &TierRow::moveUp, this, &MainWindow::onRowMoveUp);
    connect(newRow, &TierRow::moveDown, this, &MainWindow::onRowMoveDown);
    connect(newRow, &TierRow::openSettings, this, &MainWindow::onOpenRowSettings);
}



// template stuff

void MainWindow::onLoadTemplate()
{
    QDir().mkpath("templates");

    QStringList templates;
    QDir dir("templates");
    for (const QFileInfo& info : dir.entryInfoList(QStringList() << "*.json", QDir::Files)) {
        templates.append(info.baseName());
    }

    QString selected = QInputDialog::getItem(this, "Choose Template", 
        "Select a template:", templates, 0, false);

    if (!selected.isEmpty()) {
        loadTemplate("templates/" + selected + ".json");
    }
}


void MainWindow::onSaveTemplate()
{
    QString name = QInputDialog::getText(this, "Save Template", "Enter a template name:");
    if (name.isEmpty()) return;

    saveTemplate(name);
}

void MainWindow::loadTemplate(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray array = doc.array();

    // Clear bank row
    QLayout* layout = sourceItemsContainer->getLayout();
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget())
            widget->deleteLater();
        delete item;
    }

    for (const QJsonValue& val : array) {
        QString imagePath = val.toString();
        TierItem* item = new TierItem(imagePath, sourceItemsContainer);
        layout->addWidget(item);
    }
}


void MainWindow::saveTemplate(const QString& name)
{
    QJsonArray images;
    QLayout* layout = sourceItemsContainer->getLayout();

    for (int i = 0; i < layout->count(); ++i) {
        TierItem* item = qobject_cast<TierItem*>(layout->itemAt(i)->widget());
        if (item) {
            images.append(item->getImagePath());  // assumes TierItem has getImagePath()
        }
    }

    QDir().mkpath("templates");

    QFile out("templates/" + name + ".json");
    if (out.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(images);
        out.write(doc.toJson());
        out.close();
    }
}