#ifndef SAVETHREAD_H
#define SAVETHREAD_H

#include <QtCore>

class MainWindow;
class Novel;

class SaveThread : public QThread
{
    Q_OBJECT

public:
    SaveThread(MainWindow *mainWindow, Novel *novel,
               const QString &openedFile = QString());

protected:
    void run();

private:
    MainWindow *mMainWindow;
    Novel *mNovel;
    QString mOpenedFile;
};

#include "mainwindow.h"
#include "novel.h"

#endif // SAVETHREAD_H
