#include "protocol.h"

Protocol::Protocol(QObject *parent) :
    QObject(parent),Nt_SPEECH(NULL)
{
}
void Protocol::Request_SPEECH(QByteArray & audioData)
{
        qDebug()<<"audioData "<<audioData.length();
        if (!Nt_SPEECH)
        {
                QNetworkRequest request;
                QString speechAPI = "http://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=zh-CN&maxresults=1";
                request.setUrl(speechAPI);

                request.setRawHeader("User-Agent", "Mozilla/5.0");
                request.setRawHeader("Content-Type", "audio/x-flac; rate=16000");

                //qDebug(audioData);

                Nt_SPEECH = NetworkMGR.post(request, audioData);
                connect(Nt_SPEECH, SIGNAL(readyRead()), this, SLOT(Read_SPEECH()));
        }
}

//*
// *{
//    "status":0,    /* 结果代码 */
//    "id":"c421dee91abe31d9b8457f2a80ebca91-1",    /* 识别编号 */
//    "hypotheses":    /* 假设，即结果 */
//    [
//        {
//            "utterance":"下午好",    /* 话语 */
//            "confidence":0.2507637    /* 信心，即准确度 */
//        },
//        {
//            "utterance":"午好",    /* 话语 */
//            "confidence":0.2507637    /* 信心，即准确度 */
//        }
//    ]
//}
//
void Protocol::Read_SPEECH()
{
        QString content = QString::fromUtf8( Nt_SPEECH->readAll() );
        qDebug()<<"content: "<<content;
        emit Signal_SPEECH(0,content,0);

        disconnect(Nt_SPEECH, SIGNAL(readyRead()), this, SLOT(Read_SPEECH()));
        //delete Nt_SPEECH;
        Nt_SPEECH = NULL;
}
