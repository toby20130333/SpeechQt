#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>
#include <QTimer>
#include <QtMultimedia>
#include <QPaintEvent>
#include <QNetworkAccessManager>
#include "protocol.h"
#include "speechinput.h"
#include "socketdemo.h"
class mainWidget : public QWidget
{
    Q_OBJECT

public:
    mainWidget(QWidget *parent = 0);
    ~mainWidget();
    void initializeAudioInput();
    void createAudioInput();
    void stopAudioInput();
    QProcess* flacEncoder;
    Protocol* protocol;
    QAudioFormat speechInput_AudioFormat;
    QAudioDeviceInfo speechInput_AudioDeviceInfo;
    QAudioInput* speechInput_AudioInput;
    AudioInfo* speechInput_AudioInfo;

    SocketDemo* myDemo;

    RenderArea* speechArea;
    QTimer *timer;
    QString curName;
    QPushButton* frame_Speech;
    QTextEdit* textEdit_Input;
    QPushButton* label_Speech_Waiting;
    QPushButton* cancel_btn;
    QPushButton* close_btn;
    qreal curI;
    bool canmove;
    QPoint oldPos;
    QString curPath;
public slots:
    void pushButton_SpeechCancel_clicked();
    void pushButton_Speech_clicked();
    void pushButton_Close_Clicked();
    void flacEncoderFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void refreshRender();
    void slotGoogleApiData(int,QString,double);
    void slotStateChanged(QAudio::State);
    void slotUpdate();
    void slotNotify();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
};

#endif // WIDGET_H
