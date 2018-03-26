#include "actionfuc.h"

dLabel::dLabel()
{
    initial();
}

dLabel::~dLabel()
{
    delete this;
}

int dLabel::dataGet(QString *data)
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

bool dLabel::initial()
{
    int backgroundWidth = 900;
    int backgroundHeight = 100;
    int textSize = 40;
    int textR = 255;
    int textG = 20;
    int textB = 20;
    int textA = 180;

    QFile settingFile("./setting.ini");
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

    QTextStream in(&settingFile);
    QString str = "tmp";
    QString flag;
    int digiData = 0;
    int times = 0;

    while (!str.isNull()) {
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

    // initial
    QPalette textPen;
    textPen = this->palette();
    textPen.setColor(QPalette::WindowText, QColor(textR, textG, textB, textA));
    this->setPalette(textPen);

    QFont font;
    font.setPixelSize(textSize);
    this->setFont(font);
    this->setAlignment(Qt::AlignCenter);

    this->resize(backgroundWidth, backgroundHeight);

    return true;
}

void dLabel::openSubFile()
{
    // get sub file name
    QString subFileName;
    if(languageSet==0)
        subFileName = QFileDialog::getOpenFileName(this, "Open Subtitle File", "E:\\\\", "(*.txt)");
    else if(languageSet==1)
        subFileName = QFileDialog::getOpenFileName(this, "打开字幕文件", "E:\\\\", "(*.txt)");
    if(subFileName.isNull())
        return;

    // get the file
    QFile subFile(subFileName);
    if(subFile.size() == 0)
    {
        // if file is empty, exit
        fileHas = false;
        if(languageSet==0)
            this->setText("Empty File!");
        else if(languageSet==1)
            this->setText("字幕文件为空!");
        return;
    }
    else{
        fileHas = true;
    }
    subFile.open(QIODevice::ReadOnly | QIODevice::Text);

    // read sub file string
    QTextStream in(&subFile);
    QString line = in.readLine();
    substrList.clear();
    while (!line.isNull()) {
        // check, to filter empty line
        if(line.size()!=0)
            substrList.append(line);
        line = in.readLine();
    }

    subFile.close();

    // check, if has no lines will exit
    subListLength = substrList.size();
    if(subListLength == 0){
        fileHas = false;
        return;
    }

    // initial label and sub list line point
    this->setText(substrList[0]);
    if(subListLength == 0)
        fileHas = false;
    else {
        subLinePoint = 0;
    }

    return;
}

void dLabel::lastSub()
{
    if(subLinePoint!=0 && fileHas)
    {
        subLinePoint--;
        this->setText(substrList[subLinePoint]);
    }

    return;
}

void dLabel::nextSub()
{
    if(subLinePoint<subListLength-1 && fileHas)
    {
        subLinePoint++;
        this->setText(substrList[subLinePoint]);
    }

    return;
}

// move hotkey slots
void dLabel::moveUp()
{
    this->move(this->x(), this->y()-4);
    return;
}

void dLabel::moveDown()
{
    this->move(this->x(), this->y()+4);
    return;
}

void dLabel::moveLeft()
{
    this->move(this->x()-4, this->y());
    return;
}

void dLabel::moveRight()
{
    this->move(this->x()+4, this->y());
    return;
}
