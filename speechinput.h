/*
 * Based on Qt Example
 * PCM2WAV is not mine, I found it in Google and modified it.
 */

#ifndef SPEECHINPUT
#define SPEECHINPUT

#include <QPixmap>
#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QByteArray>
//#include <Phonon/AudioOutput>
#include <QtMultimedia>
#include <QIODevice>
#include <QFile>

class WavPcmFile : public QFile {
public:
        WavPcmFile(const QString & name, const QAudioFormat & format, QObject *parent = 0);
        bool open();
        void close();

private:
        void writeHeader();
        bool hasSupportedFormat();
        QAudioFormat format;
};

class AudioInfo : public QIODevice
{
        Q_OBJECT
public:
        AudioInfo(const QAudioFormat &format, QObject *parent, const QString &filename = "./data/tmp/speechInput.wav");
        ~AudioInfo();

        void start();
        void stop();

        qreal level() const { return m_level; }

        qint64 readData(char *data, qint64 maxlen);
        qint64 writeData(const char *data, qint64 len);

private:
        const QAudioFormat m_format;
        quint16 m_maxAmplitude;
        qreal m_level; // 0.0 <= m_level <= 1.0

        WavPcmFile * m_file;

signals:
        void update();
};


class RenderArea : public QPushButton
{
        Q_OBJECT

public:
        RenderArea(QWidget *parent = 0);

        void setLevel(qreal value);

protected:
        void paintEvent(QPaintEvent *event);

private:
        qreal m_level;
        QPixmap m_pixmap;
};

#endif
