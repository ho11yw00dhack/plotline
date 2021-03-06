#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QItemSelectionModel>
#include <QWidgetList>
#include <QToolBar>
#include <QCheckBox>
#include "savethread.h"

#include "novel.h"
#include "characteritemmodel.h"
#include "plotlineitemmodel.h"
#include "plotlineitemdelegate.h"
#include "plotlinedialog.h"
#include "preferencesdialog.h"
#include "revisiondialog.h"

#include "novelframe.h"
#include "characterframe.h"
#include "plotframe.h"
#include "sceneframe.h"
#include "chaptersframe.h"

class PlotlineAppFrame;
class CharacterFrame;
class PlotFrame;
class SceneFrame;
class ChaptersFrame;
class NovelFrame;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static const int TENSE_PAST = 0,
        TENSE_PRESENT = 1,
        TENSE_FUTURE = 2,
        TENSE_OTHER = 3,

        POV_1_1 = 0,
        POV_2_1 = 1,
        POV_3_1 = 2,
        POV_1_P = 3,
        POV_2_P = 4,
        POV_3_P = 5,
        POV_OTHER = 6;

    Novel *novel() const;
    void setNovel(Novel *novel);
    void openNovel(const QString &path);

    void openTab(const int index);

    virtual void closeEvent(QCloseEvent *event);

signals:
    void saveNovel();
    void novelChanged();
    void novelLoaded();
    void novelNew();

public slots:

    void show();

private slots:

    void onVersionWarningChecked(bool checked);

    void onNovelModified();
    void onSaveNovel();
    void onSaveNovel_started();
    void onSaveNovel_finished();
    void onNovelNew();
    void onNovelLoaded();
    void onHideDistractions();
    void onShowDistractions();

    void on_actionPreferences_triggered();
    void on_actionNovelNew_triggered();
    void on_actionNovelOpen_triggered();
    void on_actionNovelSave_triggered();
    void on_actionNovelRevisions_triggered();
    void on_actionNovelExport_triggered();
    void on_actionNovelBind_triggered();
    void on_actionNovelSaveAs_triggered();
    void on_actionNovelClose_triggered();

protected:

    void mouseMoveEvent(QMouseEvent *event);

private:

    static const QStringList frameTitles;
    static const QString ShowWarning;

    void showWarningMessage();

    // Frames
    NovelFrame *mNovelFrame;
    CharacterFrame *mCharacterFrame;
    PlotFrame *mPlotFrame;
    SceneFrame *mSceneFrame;
    ChaptersFrame *mChapterFrame;
    QList<PlotlineAppFrame *> frames;

    // Window & Dialogs
    Ui::MainWindow *ui;
    PreferencesDialog *mPrefDialog;

    Novel *mNovel;
    bool mIsSaved;
    QString mOpenedFile;
    QWidgetList mDistractions;

    // widgets
    QCheckBox *mShowVersionWarning;

    // Threads
    SaveThread *mSaveThread;

    void disconnectAll();
    void connectAll();

};

#endif // MAINWINDOW_H
