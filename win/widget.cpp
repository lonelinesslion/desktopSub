#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("desktopSubSet"));

    // connect btn to action
    connect(ui->saveBtn, SIGNAL(clicked(bool)), this, SLOT(trigSave()));
    connect(ui->saveBtn, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->CancelBtn, SIGNAL(clicked(bool)), this, SLOT(close()));

    // connect text setting action
    connect(ui->textSizeNum, SIGNAL(valueChanged(int)), this, SLOT(textSizeChange()));
    connect(ui->textColorRNum, SIGNAL(valueChanged(int)), this, SLOT(textRChange()));
    connect(ui->textColorGNum, SIGNAL(valueChanged(int)), this, SLOT(textGChange()));
    connect(ui->textColorBNum, SIGNAL(valueChanged(int)), this, SLOT(textBChange()));
    connect(ui->textColorANum, SIGNAL(valueChanged(int)), this, SLOT(textAChange()));

    // lock the dialog size
    this->setFixedSize(this->width(), this->height());

    // set combobox
    ui->languageSwitch->addItem(tr("English"));
    ui->languageSwitch->addItem(tr("Chinese"));
    connect(ui->languageSwitch, SIGNAL(currentIndexChanged(int)), this, SLOT(languageChange()));

    // initial data show
    initial();

    // rePaint the label "T"
    rePaintText();
}

Widget::~Widget()
{
    delete ui;
}

int Widget::dataGet(QString *data)
{
    int reData = 255;
    if(data->count("\"")!=2)
        return reData;
    else
    {
        QString digiData;
        digiData = data->section("\"", 1, 1);
        bool ok;
        reData = digiData.toInt(&ok, 10);
        return reData;
    }
}

void Widget::initial()
{
    // read setting data
    QFile settingFile("./setting.ini");
    // if setting file is not exist
    if(!settingFile.exists())
    {
        settingFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&settingFile);
        out << "BW\"900\"\nBH\"100\"\nTS\"40\"\n";
        out << "TR\"255\"\nTG\"20\"\nTB\"20\"\nTA\"180\"\n";
        out << "LN\"0\"\n";
        settingFile.close();
    }
    settingFile.open(QIODevice::ReadOnly | QIODevice::Text);

    // open and read data
    QTextStream in(&settingFile);
    QString str = "tmp";
    QString flag;
    int digiData = 0;
    int times = 0;

    // filter the data
    while(!str.isNull()) {
        in >> str;
        flag = str.left(2);
        digiData = dataGet(&str);
        if(flag=="BW")
            backgroundWidth = digiData;
        else if(flag=="BH")
            backgroundHeight = digiData;
        else if(flag=="TS")
            textSize = digiData;
        else if(flag=="TR")
            textR = digiData;
        else if(flag=="TG")
            textG = digiData;
        else if(flag=="TB")
            textB = digiData;
        else if(flag=="TA")
            textA = digiData;
        else if(flag=="LN")
            languageSet = digiData;
        times++;
    }

    settingFile.close();

    // initial setting data for QLineEdit
    QString tmpStr;
    ui->backWidth->setText(tmpStr.setNum(backgroundWidth));
    ui->backHeight->setText(tmpStr.setNum(backgroundHeight));
    ui->textSize->setText(tmpStr.setNum(textSize));
    ui->textColorR->setText(tmpStr.setNum(textR));
    ui->textColorG->setText(tmpStr.setNum(textG));
    ui->textColorB->setText(tmpStr.setNum(textB));
    ui->textColorA->setText(tmpStr.setNum(textA));

    ui->textSizeNum->setValue(textSize);
    ui->textColorRNum->setValue(textR);
    ui->textColorGNum->setValue(textG);
    ui->textColorBNum->setValue(textB);
    ui->textColorANum->setValue(textA);

    if(languageSet==0)
        ui->languageSwitch->setCurrentIndex(0);
    else if (languageSet==1)
        ui->languageSwitch->setCurrentIndex(1);

    languageChange();
}

void Widget::trigSave()
{
    // save setting
    QFile settingFile("./setting.ini");
    if(settingFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&settingFile);
        out << "BW\"";
        out << ui->backWidth->text();
        out << "\"\nBH\"";
        out << ui->backHeight->text();
        out << "\"\nTS\"";
        out << ui->textSize->text();
        out << "\"\nTR\"";
        out << ui->textColorR->text();
        out << "\"\nTG\"";
        out << ui->textColorG->text();
        out << "\"\nTB\"";
        out << ui->textColorB->text();
        out << "\"\nTA\"";
        out << ui->textColorA->text();
        out << "\"\nLN\"";
        if(ui->languageSwitch->currentText()=="English")
            out << "0\"\n";
        else if(ui->languageSwitch->currentText()=="Chinese")
            out << "1\"\n";
        settingFile.close();
    }
    else {
        return;
    }

    saveSignal();
    return;
}

void Widget::rePaintText()
{
    QPalette textPen;
    textPen = ui->textColor->palette();
    textPen.setColor(QPalette::WindowText, QColor(textR, textG, textB, textA));
    ui->textColor->setPalette(textPen);
    QFont font;
    font.setPixelSize(textSize);
    ui->textColor->setFont(font);
    ui->textColor->setAlignment(Qt::AlignCenter);
    return;
}

void Widget::closeEvent(QWidget *parent)
{
    closeSignal();
    return;
}

// text setting change slots
void Widget::textSizeChange()
{
    QString tmpNum;
    textSize = ui->textSizeNum->value();
    tmpNum.setNum(textSize);
    ui->textSize->setText(tmpNum);

    rePaintText();
    return;
}

void Widget::textRChange()
{
    QString tmpNum;
    textR = ui->textColorRNum->value();
    tmpNum.setNum(textR);
    ui->textColorR->setText(tmpNum);

    rePaintText();
    return;
}

void Widget::textGChange()
{
    QString tmpNum;
    textG = ui->textColorGNum->value();
    tmpNum.setNum(textG);
    ui->textColorG->setText(tmpNum);

    rePaintText();
    return;
}

void Widget::textBChange()
{
    QString tmpNum;
    textB = ui->textColorBNum->value();
    tmpNum.setNum(textB);
    ui->textColorB->setText(tmpNum);

    rePaintText();
    return;
}

void Widget::textAChange()
{
    QString tmpNum;
    textA = ui->textColorANum->value();
    tmpNum.setNum(textA);
    ui->textColorA->setText(tmpNum);

    rePaintText();
    return;
}

void Widget::languageChange()
{
    QString languageFile;

    if(ui->languageSwitch->currentText()=="English"){
       languageFile = qApp->applicationDirPath() + QString("/en.qm");
       languageSet = 0;
    }
    else if(ui->languageSwitch->currentText()=="Chinese"){
       languageFile = qApp->applicationDirPath() + QString("/ch.qm");
       languageSet = 1;
    }

    if(!QFile(languageFile).exists())
        return;

    // if language file is exists
    QTranslator translator;
    translator.load(languageFile);

    qApp->installTranslator(&translator);

    ui->retranslateUi(this);

    return;
}
