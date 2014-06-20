#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
class Protocol : public QObject
{
    Q_OBJECT
    // This enumeration follows the values described here:
    // http://www.w3.org/2005/Incubator/htmlspeech/2010/10/google-api-draft.html#speech-input-error
    enum SpeechInputError {
      // There was no error.
      SPEECH_INPUT_ERROR_NONE = 0,
      // The user or a script aborted speech input.
      SPEECH_INPUT_ERROR_ABORTED,
      // There was an error with recording audio.
      SPEECH_INPUT_ERROR_AUDIO,
      // There was a network error.
      SPEECH_INPUT_ERROR_NETWORK,
      // No speech heard before timeout.
      SPEECH_INPUT_ERROR_NO_SPEECH,
      // Speech was heard, but could not be interpreted.
      SPEECH_INPUT_ERROR_NO_MATCH,
      // There was an error in the speech recognition grammar.
      SPEECH_INPUT_ERROR_BAD_GRAMMAR,
    };
public:
    explicit Protocol(QObject *parent = 0);
    QNetworkAccessManager NetworkMGR;
    QNetworkReply* Nt_SPEECH;

    void Request_SPEECH(QByteArray & audioData);
signals:
    void Signal_SPEECH(int,QString,double);
public slots:
    void Read_SPEECH();
};

#endif // PROTOCOL_H
