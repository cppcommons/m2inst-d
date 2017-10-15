#include <QtCore>
#include <QtNetwork>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QNetworkAccessManager v_nam;

    //QString v_url_str = "https://raw.githubusercontent.com/cyginst/cyginst-v1/master/cyginst.bat";
    QUrl v_url("https://raw.githubusercontent.com/cyginst/cyginst-v1/master/cyginst.bat");
    QNetworkRequest v_req(v_url);
    QNetworkReply *v_reply = v_nam.get(v_req);
    while(v_reply->isRunning()) {
        qDebug() << "running";
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        QThread::msleep(10);
    }
    QByteArray v_bytes = v_reply->readAll();
    qDebug() << v_bytes.size();

    //return a.exec();
    return 0;
}
