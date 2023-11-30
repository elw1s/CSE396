#include "ImageUploader.h"

ImageUploader::ImageUploader(QWidget* parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setSpacing(2); // Set spacing between buttons to 0
    topLayout->setAlignment(Qt::AlignLeft); // Align buttons to the left

    mainLayout->setContentsMargins(25, 75, 75, 20); // Adjust the margins as needed

    QSize iconSize(35, 35);

    QIcon addFileIcon(QString::fromStdString(SELECT_FILE_ICON)); // Replace ":/icons/white_pen_icon.png" with your actual icon path
    addFileButton = createStyledButton(addFileIcon, iconSize, "#FFFFFF", "#767676", this);
    connect(addFileButton, &QPushButton::clicked, this, &ImageUploader::onAddFile);
    topLayout->addWidget(addFileButton);

    QIcon cropIcon(QString::fromStdString(CROP_ICON)); // Replace ":/icons/white_pen_icon.png" with your actual icon path
    cropButton = createStyledButton(cropIcon, iconSize, "#FFFFFF", "#767676", this);
    connect(cropButton, &QPushButton::clicked, this, &ImageUploader::onCropButtonClicked);
    topLayout->addWidget(cropButton);

    QIcon rotateIcon(QString::fromStdString(ROTATE_ICON)); // Replace ":/icons/white_pen_icon.png" with your actual icon path
    rotateButton = createStyledButton(rotateIcon, iconSize, "#FFFFFF", "#767676", this);
    //connect(cropButton, &QPushButton::clicked, this, &ImageUploader::onCropButtonClicked);
    topLayout->addWidget(rotateButton);

    m_graphicsView = new QGraphicsView(this);
    m_clippedLabel = new QLabel(this);

    mainLayout->addLayout(topLayout);
    //mainLayout->addWidget(m_graphicsView);
    //mainLayout->addWidget(m_clippedLabel);

    saveButton = new QPushButton("Draw", this);
    saveButton->setObjectName("saveButton"); // Set object name to apply specific styles
    saveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    saveButton->setFixedWidth(200);
    saveButton->setFixedHeight(50); // Set the fixed height
    saveButton->setStyleSheet("QPushButton#saveButton {"
                              "    background-color: #33C2FF;"
                              "    color: #424242;"
                              "    font-family: Abel;"
                              "    font-size: 12px;"
                              "    border: 1px solid #767676;"
                              "    margin-right: 0px;"
                              "    margin-bottom: 5px;"
                              "}"
                              "QPushButton#saveButton:hover {"
                              "    background-color: #57D5FF;" // Change color on hover if desired
                              "}");

    connect(saveButton, &QPushButton::clicked, this, &ImageUploader::saveImage);

    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addWidget(m_graphicsView);
    bottomLayout->addWidget(m_clippedLabel);
    bottomLayout->addWidget(saveButton);
    bottomLayout->setAlignment(saveButton, Qt::AlignRight); // Align the saveButton to the right within bottomLayout
    mainLayout->addLayout(bottomLayout);
    //mainLayout->addWidget(saveButton);
    mainLayout->setAlignment(Qt::AlignRight | Qt::AlignTop); // Align the layout to the top-right
    m_graphicsView->setBackgroundBrush(QColor("#D9D9D9"));  // Set the background color
    //mainLayout->setContentsMargins(25, 75, 75, 20); // Set the margins
    m_clipScene = new CropFeature(this);
    m_dragdropScene = new DragDropScene(this);
    m_graphicsView->setScene(m_dragdropScene);

    connect(m_clipScene, &CropFeature::clippedImage, this, &ImageUploader::onClippedImage);
    connect(m_dragdropScene, &DragDropScene::imageDropped, this, &ImageUploader::onImageDropped);
}

void ImageUploader::onAddFile()
{
    addFileButton->setStyleSheet("border: 1px solid #33C2FF; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    cropButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    rotateButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image File", QString(), tr("Image (*.png *.jpg *.jpeg)"));
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        m_clipScene->setImage(pixmap);
        QGraphicsScene *currentScene = m_graphicsView->scene();
        m_graphicsView->setScene(m_clipScene);
        delete currentScene;
    }
    addFileButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    cropButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    rotateButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
}

void ImageUploader::onClippedImage(const QPixmap& pixmap)
{
    m_clipScene->setImage(pixmap);
}

void ImageUploader::onCropButtonClicked()
{
    cropButton->setStyleSheet("border: 1px solid #33C2FF; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    addFileButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    rotateButton->setStyleSheet("border: 1px solid #767676; background-color: rgba(28, 28, 28, 0); color: #FFFFFF;");
    m_clipScene->setCropEnabled(!m_clipScene->isCropEnabled());
}

void ImageUploader::onImageDropped() {
    QGraphicsScene *currentScene = m_graphicsView->scene(); // Assuming m_graphicsView points to m_dragdropScene
    QPixmap droppedPixmap = m_dragdropScene->getDroppedPixmap();
    qDebug() << "onImageDropped!";
    // Use the obtained pixmap as needed in m_clipScene (CropFeature)
    if (!droppedPixmap.isNull()) {
        qDebug() << "Dropped image is not null";
        m_clipScene->setImage(droppedPixmap); // Set the dropped pixmap in m_clipScene
        qDebug() << "Image is set in m_clipScene";
        m_graphicsView->setScene(m_clipScene); // Set m_graphicsView to display m_clipScene
        qDebug() << "scene is changed!";
        delete currentScene; // Delete the old scene
        qDebug() << "the other scene is removed!";
    }
}

QPushButton* ImageUploader::createStyledButton(const QIcon &icon, const QSize &size, const QString &textColor,
                                            const QString &borderColor, QWidget *parent) {
    QPushButton *button = new QPushButton(parent);
    button->setIcon(icon);
    button->setIconSize(size);
    button->setFixedSize(size);

    // Initial button style
    button->setStyleSheet(QString("border: 1px solid %1; background-color: rgba(28, 28, 28, 0); color: %2;")
                              .arg(borderColor)
                              .arg(textColor));

    // Change border color when button is pressed
    button->setProperty("borderColor", borderColor); // Store original border color
    connect(button, &QPushButton::pressed, [=]() {
        button->setStyleSheet(QString("border: 1px solid #33C2FF; background-color: rgba(28, 28, 28, 0); color: %1;")
                                  .arg(textColor));
    });

    // Revert to original border color when button is released
    connect(button, &QPushButton::released, [=]() {
        QString storedBorderColor = button->property("borderColor").toString();
        button->setStyleSheet(QString("border: 1px solid %1; background-color: rgba(28, 28, 28, 0); color: %2;")
                                  .arg(storedBorderColor)
                                  .arg(textColor));
    });

    return button;
}

void ImageUploader::saveImage() {

    QPixmap croppedPixmap = m_clipScene->getCroppedImage();
    if (!croppedPixmap.isNull()) {
        QString folderPath = "./tmp"; // Relative path to the 'xz' folder from the application's location
        QString filePath = folderPath + "/image.jpg"; // Path to the 'xz' folder with the desired file name

        croppedPixmap.toImage().save(filePath, "JPG");
    } else {
        QMessageBox::warning(this, "Warning", "No cropped image available to save!");
    }

}
