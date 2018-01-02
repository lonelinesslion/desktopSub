#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QPixmap>
#include <QDesktopServices>
#include <QUrl>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    void initSetting();
    ~Widget();

private:
    bool midButtonFlag = false;

    Ui::Widget *ui;
    QPoint relativePos;
    QLabel *textLabel;
    QAction *openAction;
    QAction *aboutAction;
    QAction *exitAction;
    QStringList subText;
    uint subLineNum;
    uint subLineLength;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void showNextSub();

public slots:
    void exitEvent();
    void openSubFile();
    void aboutHelp();
};

#endif // WIDGET_H
