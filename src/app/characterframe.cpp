#include "characterframe.h"
#include "ui_characterframe.h"

CharacterFrame::CharacterFrame(MainWindow *mainWindow, QWidget *parent) :
    PlotlineAppFrame(mainWindow, parent),
    ui(new Ui::CharacterFrame)
{
    ui->setupUi(this);
    mModel = new CharacterModel(mMainWindow->novel());
    ui->characterList->setModel(mModel);

    // Set up the list view for drag/drop re-ordering.
    ui->characterList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->characterList->setDragDropMode(QAbstractItemView::InternalMove);
    ui->characterList->setDragEnabled(true);
    ui->characterList->viewport()->setAcceptDrops(true);
    ui->characterList->setAcceptDrops(true);
    ui->characterList->setDropIndicatorShown(true);

    // connect signals
    connect(this, SIGNAL(characterListModified()), this,
            SLOT(onCharacterListModified()));
    connect(this, SIGNAL(characterModified()), this,
            SLOT(onCharacterModified()));
}

CharacterFrame::~CharacterFrame()
{
    delete mModel;
    delete ui;
}

void CharacterFrame::onNovelLoad()
{
    delete mModel;
    mModel = new CharacterModel(mainWindow()->novel());
    ui->characterList->setModel(mModel);
    if (mModel->rowCount() > 0)
        ui->characterList->setCurrentIndex(mModel->lastRow());
    on_characterList_activated(ui->characterList->currentIndex());
}

void CharacterFrame::onNovelNew()
{
    while (mModel->rowCount() > 0) mModel->removeRow(0);
    ui->characterName->clear();
    ui->characterLabel->clear();
    ui->characterNickname->clear();
    clearCharacterHeadshot();
    ui->deleteCharacter->setDisabled(true);
    ui->archiveCharacter->setDisabled(true);
    ui->characterAttrList->setDisabled(true);
    ui->scrollAreaCharDetails->setDisabled(true);
}

void CharacterFrame::on_addCharacter_clicked()
{
    QModelIndex index = ui->characterList->currentIndex();
    mModel->insertRows(index.row()+1, 1);
    index = mModel->index(index.row()+1, 0);
    ui->characterList->setCurrentIndex(index);
    emit ui->characterList->activated(index);
}

void CharacterFrame::onCharacterListModified()
{
    ui->scrollAreaCharDetails->setEnabled(mModel->rowCount() > 0);
    emit novelModified();
}

void CharacterFrame::onCharacterModified()
{
//    on_characterList_activated(ui->characterList->currentIndex());
    emit novelModified();
}

void CharacterFrame::on_characterList_activated(const QModelIndex &index)
{
    ui->scrollAreaCharDetails->setDisabled(true);
    ui->deleteCharacter->setDisabled(true);
    ui->archiveCharacter->setDisabled(true);
    clearCharacterHeadshot();
    if (!index.isValid()) {
        qDebug() << "character - none selected";
        return;
    }

    ui->characterAttrList->setEnabled(true);
    ui->scrollAreaWidgetContents->setEnabled(true);
    ui->scrollAreaCharDetails->setEnabled(true);
    ui->deleteCharacter->setEnabled(true);
    ui->archiveCharacter->setEnabled(true);

    setCharacterHeadshot();

    QString name = mModel->data(index, CharacterModel::NameRole).toString(),
            label = mModel->data(index, CharacterModel::LabelRole).toString(),
            nickname = mModel->data(index, CharacterModel::NicknameRole).toString();
    QColor color = mModel->data(index, CharacterModel::ColorRole)
            .value<QColor>();

    ui->characterName->blockSignals(true);
    ui->characterName->setText(name);
    ui->characterName->blockSignals(false);
    ui->characterLabel->blockSignals(true);
    ui->characterLabel->setText(label);
    ui->characterLabel->blockSignals(false);
    ui->characterNickname->blockSignals(true);
    ui->characterNickname->setText(nickname);
    ui->characterNickname->blockSignals(false);

    setCharacterHeadshot();

    setButtonColor(color);

}

void CharacterFrame::setCharacterHeadshot()
{
    QGraphicsScene *scene = new QGraphicsScene();
    int h = ui->characterHeadshot->geometry().height(),
            w = ui->characterHeadshot->geometry().width();

    QModelIndex index = ui->characterList->currentIndex();
    if (!index.isValid())
        return;
    QVariant headshotData = mModel->data(index, CharacterModel::HeadshotRole);

    QImage headshot = headshotData.value<QImage>();
    if (headshot.width() > headshot.height())
        headshot = headshot.scaledToWidth(w-2);
    else
        headshot = headshot.scaledToHeight(h-2);
    scene->addPixmap(QPixmap::fromImage(headshot));
    ui->characterHeadshot->setScene(scene);
}

void CharacterFrame::clearCharacterHeadshot()
{
    if (!ui->characterHeadshot->scene()) return;
    QList<QGraphicsItem *> items = ui->characterHeadshot->scene()->items();
    for (QGraphicsItem *item : items)
        ui->characterHeadshot->scene()->removeItem(item);
}

void CharacterFrame::setButtonColor(const QColor &color)
{
    if (!color.isValid())
        clearButtonColor();
    QImage image = QImage(40, 20, QImage::Format_RGB32);
    image.fill(color);
    ui->characterColor->setIcon(QIcon(QPixmap::fromImage(image)));
}

void CharacterFrame::clearButtonColor()
{
    QString filename = "qrc:/images/images/edit-clear-locationbar-rtl.png";
    ui->characterColor->setIcon(QIcon(QPixmap(filename)));
}

void CharacterFrame::on_characterList_clicked(const QModelIndex &index)
{
    on_characterList_activated(index);
}

void CharacterFrame::on_characterName_textChanged(const QString &arg1)
{
    QModelIndex index = ui->characterList->currentIndex();
    mModel->setData(index, arg1, Qt::DisplayRole);
    ui->characterLabel->blockSignals(true);
    ui->characterLabel->setText(Character::generateLabel(arg1));
    ui->characterLabel->blockSignals(false);
    emit mModel->dataChanged(index, index);
    emit characterModified();
}

void CharacterFrame::on_characterColor_clicked()
{
    QColor current = mModel->data(ui->characterList->currentIndex(),
                                  CharacterModel::ColorRole).value<QColor>();
    QColor color = QColorDialog::getColor(current);
    mModel->setData(ui->characterList->currentIndex(),
                    color, CharacterModel::ColorRole);
}

void CharacterFrame::on_chooseHeadshot_clicked()
{
    QString headshotFile = QFileDialog::getOpenFileName(this,
                                                tr("Select Headshot"),
                                                QDir::homePath(),
                                                tr("Image (*.jpg *.png *.JPEG *.jpeg *.gif *.tif)"));
    if (headshotFile.isEmpty())
        return;
    QImage img = QImage(headshotFile);
    QVariant variant = img;
    mModel->setData(ui->characterList->currentIndex(), variant,
                    CharacterModel::HeadshotRole);
    emit characterModified();
}

void CharacterFrame::on_characterLabel_textChanged(const QString &arg1)
{
    mModel->setData(ui->characterList->currentIndex(),
                    arg1, CharacterModel::LabelRole);
    emit characterModified();
}

void CharacterFrame::on_archiveCharacter_clicked()
{

}

void CharacterFrame::on_deleteCharacter_clicked()
{
    QModelIndex index = ui->characterList->currentIndex();
    QString name = mModel->data(index, CharacterModel::NameRole).toString();
    QString title = tr("Delete Character?"),
            text = QString("Are you sure you want to delete \"")
                + name + QString("?\"\nArchiving is a safer action."),
            b0t = tr("Archive"),
            b1t = tr("Cancel"),
            b2t = tr("Delete");
    const int dflt = 0;
    int res = QMessageBox::warning(this, title, text, b0t, b1t, b2t, dflt);
    if (res == 0){
        // TODO: archive.
        return;
    } if (res == 1) {
        // cancel
        return;
    } if (res == 2) {
        mModel->removeRows(index.row(), 1);
        // Set the index to the previous. and activate it!
//        index = mModel->index(index.row()-1, 0);
        emit ui->characterList->activated(index);
        emit novelModified();
    }
}
