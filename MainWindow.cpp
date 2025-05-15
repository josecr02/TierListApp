#include "MainWindow.h"
#include "Tier.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QWidget>



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
      //  row->clearImages();  // you'll need to implement this in TierRow
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