/*
 * Based on Qt Example
 * PCM2WAV is not mine, I found it in Google and modified it.
 */

#include "speechInput.h"

#include <QtEndian>
#include <QDebug>
#include <QPainter>

WavPcmFile::WavPcmFile(const QString & name, const QAudioFormat & format_, QObject *parent_)
        : QFile(name, parent_), format(format_)
{
}

bool WavPcmFile::hasSupportedFormat()
{
        return (format.sampleSize() == 8
                && format.sampleType() == QAudioFormat::UnSignedInt)
                || (format.sampleSize() > 8
                && format.sampleType() == QAudioFormat::SignedInt
                && format.byteOrder() == QAudioFormat::LittleEndian);
}

bool WavPcmFile::open()
{
        if (!hasSupportedFormat()) {
                setErrorString("Wav PCM supports only 8-bit unsigned samples "
                        "or 16-bit (or more) signed samples (in little endian)");
                return false;
        } else {
                if (!QFile::open(ReadWrite | Truncate))
                        return false;
                writeHeader();
                return true;
        }
}

void WavPcmFile::writeHeader()
{
        QDataStream out(this);
        out.setByteOrder(QDataStream::LittleEndian);

        // RIFF chunk
        out.writeRawData("RIFF", 4);
        out << quint32(0); // Placeholder for the RIFF chunk size (filled by close())
        out.writeRawData("WAVE", 4);

        // Format description chunk
        out.writeRawData("fmt ", 4);
        out << quint32(16); // "fmt " chunk size (always 16 for PCM)
        out << quint16(1);  // data format (1 => PCM)
        out << quint16(format.channelCount());
        out << quint32(format.sampleRate());
        out << quint32(format.sampleRate() * format.channelCount()
                * format.sampleSize() / 8 ); // bytes per second
        out << quint16(format.channelCount() * format.sampleSize() / 8); // Block align
        out << quint16(format.sampleSize()); // Significant Bits Per Sample

        // Data chunk
        out.writeRawData("data", 4);
        out << quint32(0);  // Placeholder for the data chunk size (filled by close())

        Q_ASSERT(pos() == 44); // Must be 44 for WAV PCM
}

void WavPcmFile::close()
{
        // Fill the header size placeholders
        quint32 fileSize = size();

        QDataStream out(this);
        // RIFF chunk size
        seek(4);
        out << quint32(fileSize - 8);

        // data chunk size
        seek(40);
        out << quint32(fileSize - 44);

        QFile::close();
}

AudioInfo::AudioInfo(const QAudioFormat &format, QObject *parent, const QString &filename)
        :   QIODevice(parent)
        ,   m_format(format)
        ,   m_maxAmplitude(0)
        ,   m_level(0.0)

{
        switch (m_format.sampleSize()) {
        case 8:
                switch (m_format.sampleType()) {
                case QAudioFormat::UnSignedInt:
                        m_maxAmplitude = 255;
                        break;
                case QAudioFormat::SignedInt:
                        m_maxAmplitude = 127;
                        break;
                default:
                        break;
                }
                break;
        case 16:
                switch (m_format.sampleType()) {
                case QAudioFormat::UnSignedInt:
                        m_maxAmplitude = 65535;
                        break;
                case QAudioFormat::SignedInt:
                        m_maxAmplitude = 32767;
                        break;
                default:
                        break;
                }
                break;
        default:
                break;
        }

        m_file = new WavPcmFile(filename,format,this);

}

AudioInfo::~AudioInfo()
{
}

void AudioInfo::start()
{
        m_file->open();
        open(QIODevice::WriteOnly);
}

void AudioInfo::stop()
{
        close();
        m_file->close();
}

qint64 AudioInfo::readData(char *data, qint64 maxlen)
{
        Q_UNUSED(data)
                Q_UNUSED(maxlen)

                return 0;
}

qint64 AudioInfo::writeData(const char *data, qint64 len)
{
        if (m_maxAmplitude) {
                Q_ASSERT(m_format.sampleSize() % 8 == 0);
                const int channelBytes = m_format.sampleSize() / 8;
                const int sampleBytes = m_format.channelCount() * channelBytes;
                Q_ASSERT(len % sampleBytes == 0);
                const int numSamples = len / sampleBytes;

                quint16 maxValue = 0;
                const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

                for (int i = 0; i < numSamples; ++i) {
                        for(int j = 0; j < m_format.channelCount(); ++j) {
                                quint16 value = 0;

                                if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                                        value = *reinterpret_cast<const quint8*>(ptr);
                                } else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::SignedInt) {
                                        value = qAbs(*reinterpret_cast<const qint8*>(ptr));
                                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                                        if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                                                value = qFromLittleEndian<quint16>(ptr);
                                        else
                                                value = qFromBigEndian<quint16>(ptr);
                                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::SignedInt) {
                                        if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                                                value = qAbs(qFromLittleEndian<qint16>(ptr));
                                        else
                                                value = qAbs(qFromBigEndian<qint16>(ptr));
                                }

                                maxValue = qMax(value, maxValue);
                                ptr += channelBytes;
                        }
                }

                maxValue = qMin(maxValue, m_maxAmplitude);
                m_level = qreal(maxValue) / m_maxAmplitude;
        }

        m_file->write(data,len);

        emit update();
        return len;
}


RenderArea::RenderArea(QWidget *parent)
        : QPushButton(parent)
{
        setBackgroundRole(QPalette::Base);
        setAutoFillBackground(true);

        m_level = 0;
        setMinimumHeight(30);
        setMinimumWidth(80);

}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
        QPainter painter(this);
        QPixmap pixmap = QPixmap(":/images/button_default.png").scaled(this->size());
        painter.drawPixmap(this->rect(), pixmap);

//        painter.setPen(Qt::black);
//        painter.drawRect(QRect(painter.viewport().left(),
//                painter.viewport().top(),
//                painter.viewport().right()-20,
//                painter.viewport().bottom()-20));
        if (m_level == 0.0)
                return;
        painter.setPen(Qt::darkGray);
        int pos = ((painter.viewport().right()-20)-(painter.viewport().left()+11))*m_level;
        for (int i = 0; i < 10; ++i) {
                int x1 = painter.viewport().left()+11;
                int y1 = painter.viewport().top()+10+i;
                int x2 = painter.viewport().left()+20+pos;
                int y2 = painter.viewport().top()+10+i;
                if (x2 < painter.viewport().left()+10)
                        x2 = painter.viewport().left()+10;

                painter.drawLine(QPoint(x1+10, y1+10),QPoint(x2+10, y2+10));
        }
}

void RenderArea::setLevel(qreal value)
{
        m_level = value;
        repaint();
}
