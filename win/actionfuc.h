#ifndef ACTIONFUC_H
#define ACTIONFUC_H

#endif // ACTIONFUC_H

#include <QWidget>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QTextStream>
#include <QThread>

class dLabel : public QLabel
{
    Q_OBJECT
public:
    dLabel();
    ~dLabel();

    QStringList substrList;
    int subListLength = 0;
    int subLinePoint = 0;
    bool fileHas = false;

    int languageSet = 0;

    // get the setting data from setting file "./setting.ini"
    int dataGet(QString *data);

    // initial label as setting file, will call dataGet()
    bool initial();

public slots:
    void openSubFile();

    void lastSub();

    void nextSub();

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
};

