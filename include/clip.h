
#ifndef CUSTOM_CLIP_H
    #define CUSTOM_CLIP_H
    #include <QString>
    #include "utils.h"
    #include <iostream>
    #include <QJsonDocument>
    #include <QCryptographicHash>
    class Clip {
        private:
            QString m_value;
            QString m_format;
            QString m_hash;
            qint64 m_timestamp;
        public:
            // friend QString getMd5Hash(const QString&);
            Clip():m_value(""),m_format(""),m_hash(""),m_timestamp(0){}
            Clip(const QString& value,const QString& format,qint64 timestamp)
                :m_value(value)
                ,m_format(format),m_timestamp(timestamp){
                    QString temp = value + format + QString::number(timestamp);
                    QCryptographicHash qh(QCryptographicHash::Md5);
                    qh.addData(temp.toStdString().c_str(),16);
                    m_hash = qh.result().toHex();
                }

            Clip(const Clip& clip):m_value(clip.value()),m_format(clip.format()),
                        m_hash(clip.hash()),m_timestamp(clip.timestamp()){}
            
            QString value() const{return m_value;}
            QString format() const {return m_format;}
            qint64 timestamp() const {return m_timestamp;}
            QString hash() const {return m_hash;}
            

            QString toString() const{
                return "\"timestamp\": " + QString::number(m_timestamp)
                        + ", \"value\": \"" + m_value + "\""
                        + ", \"format\": \"" + m_format + "\""
                        + ", \"hash\": \"" + m_hash + "\"";
            }

            friend std::ostream& operator<<(std::ostream& cout,const Clip& c)
            {
                cout << c.toString().toStdString() << std::endl; 
                return cout;
            }

            QString toJson() const
            {
                // return QJsonDocument::fromJson(this->toString().toUtf8()).toJson(QJsonDocument::Compact);
                return "{" + this->toString() + "}";
            }

    };
#endif
