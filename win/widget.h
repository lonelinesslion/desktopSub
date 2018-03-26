#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QPalette>
#include <QFont>
#include <QTranslator>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void rePaintText();

private:
    Ui::Widget *ui;

    int dataGet(QString *data);
    void initial();

    int backgroundWidth = 900;
    int backgroundHeight = 100;
    int textSize = 40;
    int textR = 255;
    int textG = 20;
    int textB = 20;
    int textA = 180;
    int languageSet = 0;

    void closeEvent(QWidget *parent = 0);

public slots:
    void trigSave();

    void textSizeChange();
    void textRChange();
    void textGChange();
    void textBChange();
    void textAChange();
    void languageChange();

signals:
    void saveSignal();
    void closeSignal();
};

#endif // WIDGET_H
