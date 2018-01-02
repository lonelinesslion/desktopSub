#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // some initial set to create a transparent widget
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    ui->label->setGeometry(0, 0, 1920, 1080);

    // no topbar
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->show();

    // three action for rightButton
    openAction = new QAction(tr("打开"), this);
    aboutAction = new QAction(tr("关于"), this);
    exitAction = new QAction(tr("退出"), this);

    addAction(openAction);
    addAction(aboutAction);
    addAction(exitAction);

    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(exitEvent()));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openSubFile()));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(aboutHelp()));

    // use this bool var to mark if it is midButton event
    midButtonFlag = false;

    // initial function
    initSetting();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    // it will show the next line text when click leftButton
    if(event->button()==Qt::LeftButton)
    {
        showNextSub();
    }
    // remember the position when click midButton
    if(event->button()==Qt::MidButton)
    {
        relativePos = this->pos() - event->globalPos();
        midButtonFlag = true;
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    // if this move is trigger by midButton, move the widget
    if(midButtonFlag == true)
    {
        this->move(event->globalPos() + relativePos);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    // if midButton release, release the moving widget
    if(event->button()==Qt::MidButton)
    {
        midButtonFlag = false;
    }
}

void Widget::exitEvent()
{
    this->close();
}

// open the sub file
void Widget::openSubFile()
{
    QString subFileName = QFileDialog::getOpenFileName(this, tr("Open Subtitle File"), "./", "setting file (*.txt)");
    if(subFileName.isNull())
        return;
    // get the file
    QFile subFile(subFileName);
    if(subFile.size() == 0)
    {
        ui->textLabel->setText(tr("文件内容为空！"));
        return;
    }
    subFile.open(QIODevice::ReadOnly | QIODevice::Text);

    // initial the subText QStringList
    subText.clear();

    // get the sub files text and give to subText list
    QTextStream subTextStream(&subFile);
    QString tmpText = "a";
    int i = -1;
    while (!tmpText.isNull()) {
        subTextStream >> tmpText;
        subText << tmpText;
        i++;
    }
    // it will end with ""
    subText.removeLast();

    subFile.close();

    subLineLength = i;
    ui->textLabel->setText(subText[0]);
    subLineNum = 1;
}

void Widget::aboutHelp()
{
    QString message = tr("许可：GPL v2\n");
    message += tr("版本：v0.1\n");
    message += tr("注意：如果配置文件错误，可以将其删除后再次打开程序重新生成\n");
    message += tr("注意：当前版本框体最大为 1920 * 1080\n");
    message += tr("设置：界面、字体的设置请修改程序目录下的 setting.txt\n");
    message += tr("操作方法：打开字幕文件后，左键点击将显示下一行文字\n");
    QMessageBox::about(this, tr("关于"), message);
}

void Widget::initSetting()
{
    // get the setting file's setting
    QFile settingFile("./setting.txt");
    if(!settingFile.exists())
    {
        settingFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&settingFile);
        out << "backWidth=600\nbackHeight=60\nbackR=100\nbackG=100\nbackB=100\nbackA=150\n";
        out << "textR=255\ntextG=10\ntextB=20\ntextSize=20\ntextWeight=60";
        settingFile.close();
    }
    settingFile.open(QIODevice::ReadOnly | QIODevice::Text);

    // get every line's digital
    QRegExp rex("(\\d+)");
    QString endStr;

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int labWidth = endStr.toInt();
    if(labWidth==0) {labWidth=400;}

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int labHeight = endStr.toInt();
    if(labWidth==0) {labWidth=100;}

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int labR = endStr.toInt();

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int labG = endStr.toInt();

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int labB = endStr.toInt();

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int labA = endStr.toInt();

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int textR = endStr.toInt();

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int textG = endStr.toInt();

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int textB = endStr.toInt();

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int textSize = endStr.toInt();

    rex.indexIn(settingFile.readLine());
    endStr = rex.cap();
    int textWeight = endStr.toInt();

    settingFile.close();

    // set label background color
    QPixmap pixw(ui->label->size());
    pixw.fill(QColor(labR, labG, labB, labA));
    ui->label->setPixmap(pixw);

    // set flow window's size
    this->resize(labWidth, labHeight);
    // set the text label size same as flow window
    ui->textLabel->setGeometry(0, 0, labWidth, labHeight);

    // set the text font
    QPalette textPen;
    textPen = ui->textLabel->palette();
    textPen.setColor(QPalette::WindowText, QColor(textR, textG, textB));
    ui->textLabel->setPalette(textPen);

    QFont textFont;
    textFont = ui->textLabel->font();
    textFont.setPointSize(textSize);
    textFont.setWeight(textWeight);
    ui->textLabel->setFont(textFont);

    ui->textLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    ui->textLabel->setText(tr("请打开字幕文件"));
}

void Widget::showNextSub()
{
    if(subText.isEmpty())
        return;
    if(subLineNum < subLineLength)
        ui->textLabel->setText(subText[subLineNum++]);
    else
        ui->textLabel->setText(tr("结束！"));
}
