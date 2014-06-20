#include "widget.h"
#include <QApplication>
mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent),curI(0),canmove(false),speechInput_AudioInput(NULL),
      flacEncoder(NULL)
{
    this->setFixedSize(440,300);
    this->setWindowFlags(Qt::FramelessWindowHint);
    curPath = QApplication::applicationDirPath();
    curPath.chop(30);
    QNetworkAccessManager* _manager = new QNetworkAccessManager(this);
    QString hostUrl = "http://www.ip138.com/ips138.asp?ip=111.111.111.111&action=2";
    QNetworkRequest request;
    request.setUrl(QUrl(hostUrl));
    //request.setHeader();
    QNetworkReply *reply = _manager->get(request);
    QByteArray responseData;
    QEventLoop eventLoop;
    connect(_manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    responseData = reply->readAll();

    qDebug()<<"hostUrl"<<hostUrl<<" "<<responseData;
    myDemo = new SocketDemo();
    //myDemo->connectPhone(QString(responseData));

    frame_Speech = new QPushButton(this);
    connect(frame_Speech,SIGNAL(clicked()),this,SLOT(pushButton_Speech_clicked()));
    frame_Speech->resize(100,50);
    frame_Speech->move(50,110);
    frame_Speech->setStyleSheet("QPushButton {\
                                color: grey;\
                                image: url(:/images/speech_n.png) ;\
                                image-position: right center;\
                                border-image: url(:/images/button_default.png) 3 10 3 10;\
                                border-top: 3px transparent;\
                                border-bottom: 3px transparent;\
                                border-right: 10px transparent;\
                                border-left: 10px transparent;\
                            }"\
                                                     "QPushButton:pressed {\
                               image: url(:/images/speech_p.png);\
                               image-position: right center;\
                               border-image: url(:/images/button_press.png) 3 10 3 10;\
                               border-top: 3px transparent;\
                               border-bottom: 3px transparent;\
                               border-right: 10px transparent;\
                               border-left: 10px transparent;\
                           }"\
                                                    "QPushButton:hover {\
                                color:grey;\
                                image: url(:/images/speech_p.png);\
                                image-position: right center;\
                                border-image: url(:/images/button_press.png) 3 10 3 10;\
                                border-top: 3px transparent;\
                                border-bottom: 3px transparent;\
                                border-right: 10px transparent;\
                                border-left: 10px transparent;\
     }");
    qDebug()<<"11111111111"<<endl;
    cancel_btn = new QPushButton(this);
    connect(cancel_btn,SIGNAL(clicked()),this,SLOT(pushButton_SpeechCancel_clicked()));
    cancel_btn->resize(100,50);
    cancel_btn->move(50,110);
    cancel_btn->setText(QString::fromUtf8("取消录音"));
    cancel_btn->setStyleSheet("QPushButton {\
                                color: grey;\
                                border-image: url(:/images/button_default.png) 3 10 3 10;\
                                border-top: 3px transparent;\
                                border-bottom: 3px transparent;\
                                border-right: 10px transparent;\
                                border-left: 10px transparent;\
                            }"\
                                                "QPushButton:pressed {\
                               color: grey;\
                               border-image: url(:/images/button_press.png) 3 10 3 10;\
                               border-top: 3px transparent;\
                               border-bottom: 3px transparent;\
                               border-right: 10px transparent;\
                               border-left: 10px transparent;\
                           }"\
                                                "QPushButton:hover {\
                                color:grey;\
                                image-position: right center;\
                                border-image: url(:/images/button_press.png) 3 10 3 10;\
                                border-top: 3px transparent;\
                                border-bottom: 3px transparent;\
                                border-right: 10px transparent;\
                                border-left: 10px transparent;\
     }");
    cancel_btn->setEnabled(false);
    cancel_btn->hide();

    qDebug()<<"222222222222"<<endl;
    textEdit_Input = new QTextEdit(this);
    textEdit_Input->resize(180,200);
    textEdit_Input->move(230,50);
    textEdit_Input->setStyleSheet("QTextEdit {\
                                  border: 2px solid gray;\
                                  border-radius: 5px;\
                                  padding: 0 8px;\
                                  background: \"#649f12\";\
                                  selection-background-color: darkgray;\
                              }"\
                                  "QScrollBar:vertical {\
                                  border: 1px solid grey;\
                                  background: #32CC99;\
                                  width: 13px;\
                                  margin: 22px 0 22px 0;\
                              }"\
                              "QScrollBar::handle:vertical {\
                                  background: darkgray;\
                                  min-height: 20px;\
                              }"\
                              "QScrollBar::add-line:vertical {\
                                  border: 2px solid grey;\
                                  background: #32CC99;\
                                  height: 20px;\
                                  subcontrol-position: bottom;\
                                  subcontrol-origin: margin;\
                              }"\
                              "QScrollBar::sub-line:vertical {\
                                  border: 2px solid grey;\
                                  background: #32CC99;\
                                  height: 20px;\
                                  subcontrol-position: top;\
                                  subcontrol-origin: margin;\
                              }"\
                              "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {\
                                  border: 2px solid grey;\
                                  width: 3px;\
                                  height: 3px;\
                                  background: darkgray;\
                              }"\
                              "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\
                                  background: none;\
                              }");

    qDebug()<<"33333333333"<<endl;
    label_Speech_Waiting = new QPushButton(this);
    //connect(label_Speech_Waiting,SIGNAL(clicked()),this,SLOT(pushButton_SpeechCancel_clicked()));
    label_Speech_Waiting->resize(100,50);
    label_Speech_Waiting->move(50,110);
    label_Speech_Waiting->setText(QString::fromUtf8("智能识别"));
    label_Speech_Waiting->setStyleSheet("QPushButton {\
                                color: grey;\
                                border-image: url(:/images/button_default.png) 3 10 3 10;\
                                border-top: 3px transparent;\
                                border-bottom: 3px transparent;\
                                border-right: 10px transparent;\
                                border-left: 10px transparent;\
                            }"\
                                                              "QPushButton:pressed {\
                               color: grey;\
                               border-image: url(:/images/button_press.png) 3 10 3 10;\
                               border-top: 3px transparent;\
                               border-bottom: 3px transparent;\
                               border-right: 10px transparent;\
                               border-left: 10px transparent;\
                           }"\
                                                             "QPushButton:hover {\
                               color: grey;\
                               border-image: url(:/images/button_press.png) 3 10 3 10;\
                               border-top: 3px transparent;\
                               border-bottom: 3px transparent;\
                               border-right: 10px transparent;\
                               border-left: 10px transparent;\
}");
    label_Speech_Waiting->hide();
    protocol = new Protocol();
    connect(protocol,SIGNAL(Signal_SPEECH(int,QString,double)),this,SLOT(slotGoogleApiData(int,QString,double)));

    speechArea = new RenderArea(this);
    speechArea->resize(100,50);
    speechArea->move(50,200);
    speechArea->hide();

    close_btn = new QPushButton(this);
    connect(close_btn,SIGNAL(clicked()),this,SLOT(pushButton_Close_Clicked()));
    close_btn->resize(25,25);
    close_btn->move(this->width()-40,10);
    close_btn->setStyleSheet("QPushButton {\
                             color: grey;\
                             border-image: url(:/images/close_d.png) 3 10 3 10;\
                             border-top: 3px transparent;\
                             border-bottom: 3px transparent;\
                             border-right: 10px transparent;\
                             border-left: 10px transparent;\
                         }"\
                                           "QPushButton:pressed {\
                            color: grey;\
                            border-image: url(:/images/close_h.png) 3 10 3 10;\
                            border-top: 3px transparent;\
                            border-bottom: 3px transparent;\
                            border-right: 10px transparent;\
                            border-left: 10px transparent;\
                        }"\
                                          "QPushButton:hover {\
                            color: grey;\
                            border-image: url(:/images/close_p.png) 3 10 3 10;\
                            border-top: 3px transparent;\
                            border-bottom: 3px transparent;\
                            border-right: 10px transparent;\
                            border-left: 10px transparent;\
         }");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdate()));
    //timer->start(500);
}

mainWidget::~mainWidget()
{

}
// Init Speech Input
void mainWidget::initializeAudioInput()
{
    speechInput_AudioFormat.setSampleRate(16000);
    speechInput_AudioFormat.setChannelCount(1);
    speechInput_AudioFormat.setSampleSize(16);
    speechInput_AudioFormat.setSampleType(QAudioFormat::SignedInt);
    speechInput_AudioFormat.setByteOrder(QAudioFormat::LittleEndian);
    speechInput_AudioFormat.setCodec("audio/pcm");

    speechInput_AudioDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
    bool isSupport = speechInput_AudioDeviceInfo.isFormatSupported(speechInput_AudioFormat);
    qDebug()<<"isSupport "<<isSupport<<curPath;
    if(!isSupport)
    {
        speechInput_AudioFormat = speechInput_AudioDeviceInfo.nearestFormat(speechInput_AudioFormat);
    }

    curName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    curName.append(".wav");
    speechInput_AudioInfo  = new AudioInfo(speechInput_AudioFormat, this, curPath+"/lib/"+curName);
    connect(speechInput_AudioInfo, SIGNAL(update()), SLOT(refreshRender()));
    qDebug()<<"isSupport======= "<<speechInput_AudioInfo->errorString();
    createAudioInput();
}

// Create Audio Input
void mainWidget::createAudioInput()
{
    speechInput_AudioInput = new QAudioInput(speechInput_AudioDeviceInfo, speechInput_AudioFormat, this);
    //connect(speechInput_AudioInput,SIGNAL(stateChanged(QAudio::State)),this,SLOT(slotStateChanged(QAudio::State)));
    connect(speechInput_AudioInput,SIGNAL(notify()),this,SLOT(slotNotify()));
    qDebug()<<"createAudioInput";
    if(speechInput_AudioInput != NULL)
    {
        qDebug()<<"createAudioInput333";
        speechInput_AudioInfo->start();
        qDebug()<<"createAudioInput1111";
        speechInput_AudioInput->start(speechInput_AudioInfo);
        qDebug()<<"createAudioInput222";
    }
}

// Stop Audio Input
void mainWidget::stopAudioInput()
{
    speechInput_AudioInput->stop();
    speechInput_AudioInfo->stop();

    speechInput_AudioInput->deleteLater();
    speechInput_AudioInput = NULL;
}

// Refresh Render
void mainWidget::refreshRender()
{
            speechArea->setLevel(speechInput_AudioInfo->level());
            speechArea->repaint();
}

void mainWidget::slotGoogleApiData(int, QString str, double)
{
    if(str != ""){
        label_Speech_Waiting->hide();
        textEdit_Input->setText(str);
    }else{
        label_Speech_Waiting->hide();
    }
    label_Speech_Waiting->setEnabled(true);
    frame_Speech->show();
    speechArea->hide();
}
/*
 * QAudio::ActiveState	0	Audio data is being processed, this state is set after start() is called and while audio data is available to be processed.
QAudio::SuspendedState	1	The audio device is in a suspended state, this state will only be entered after suspend() is called.
QAudio::StoppedState	2	The audio device is closed, not processing any audio data
QAudio::IdleState
*/
void mainWidget::slotStateChanged(QAudio::State status)
{
    //qDebug()<<"slotStateChanged "<<status<<speechInput_AudioInput->error();
    switch (status) {
    case QAudio::SuspendedState:
        break;
    case QAudio::ActiveState:
        //QTimer::singleShot(5000,this,SLOT(pushButton_SpeechCancel_clicked()));
        break;
    case QAudio::StoppedState:
        if (speechInput_AudioInput->error() != QAudio::NoError)
        {
            textEdit_Input->setText(QString::fromUtf8("音频设备未安装"));
        } else
        {
            //QTimer::singleShot(5000,this,SLOT(pushButton_SpeechCancel_clicked()));
        }
        break;
    default:
        break;
    }
    speechInput_AudioInput->error();
}

void mainWidget::slotUpdate()
{
    curI +=0.1;
    qDebug()<<"slotUpdate "<<curI;
    if(curI >1.0)
    {
        timer->stop();
        return;
    }
    if(speechArea != NULL)
    {
        qreal i = qrand()%10;
        qreal ii = i/10;
        qDebug()<<"slotUpdate I "<<curI;
        speechArea->setLevel(curI);
        speechArea->repaint();
    }
}

void mainWidget::slotNotify()
{
   // qDebug()<<" slotNotify "<<speechInput_AudioInput->error();
}

void mainWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap = QPixmap(":/images/main_bg.png").scaled(this->size());
    painter.drawPixmap(this->rect(), pixmap);
}

void mainWidget::mousePressEvent(QMouseEvent *e)
{
    if( (e->pos().x()>= 10) && (e->pos().y()<=45) && (e->pos().x()<=this->x()-40))
    {
        canmove = true;
    }
    else
    {
        canmove = false;
        e->accept();
        return;
    }
    oldPos = e->pos();
    e->accept();
}

void mainWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(canmove)
    {
        //qDebug()<<this->pos()<<"\n"<<this->y();
        move(e->globalPos() - oldPos);
    }
    e->accept();
}

// Send Request
void mainWidget::flacEncoderFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug()<<"flacEncoderFinished "<<exitStatus<<"\n"<<exitCode;
    if (exitStatus == QProcess::NormalExit)
    {
        QByteArray flacData = flacEncoder->readAll();
        protocol->Request_SPEECH(flacData);
    }
    //label_Speech_Waiting->hide();
    //frame_Speech->hide();
    flacEncoder->deleteLater();
    flacEncoder = NULL;
}

// Speech Input Button - Start Speech
void mainWidget::pushButton_Speech_clicked()
{
    initializeAudioInput();
    qDebug()<<"pushButton_Speech_clicked";
    textEdit_Input->setReadOnly(true);
    //label_Speech_Waiting->show();
    frame_Speech->hide();
    speechArea->show();
    cancel_btn->show();
    cancel_btn->setEnabled(true);


    //QTimer::singleShot(5000,this,SLOT(pushButton_SpeechCancel_clicked()));
}

void mainWidget::pushButton_Close_Clicked()
{
    if(speechInput_AudioInput != NULL)
    {
        stopAudioInput();
    }
    qApp->quit();
}

// Speech Input Button - Stop Speech
// Call flacEncoder
void mainWidget::pushButton_SpeechCancel_clicked()
{
    frame_Speech->hide();
    cancel_btn->hide();
    speechArea->hide();
    label_Speech_Waiting->show();
    label_Speech_Waiting->setEnabled(false);
    stopAudioInput();
    //delete speechInput_AudioInput;

    //label_Speech_Waiting->setText(QString::fromUtf8("识别中..."));
    //QString program = "./lib/flac.exe";
    /*
         *      flac.exe -c --totally-silent -f -8 test.wav
         */
    QStringList arguments;
#ifdef Q_OS_WIN
    QString program = "./lib/flac.exe";
#else Q_OS_MAC
    QString program = "/usr/local/bin/flac";
#endif
    arguments << "-c" << "-f" << "-8" << curPath +"/lib/"+curName;
    flacEncoder = new QProcess(this);
    connect(flacEncoder,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(flacEncoderFinished(int, QProcess::ExitStatus)));

    flacEncoder->start(program, arguments);
    qDebug()<<"arguments "<<program<<" "<<arguments;
}
//github address https://github.com/taf2/speech2text.git
