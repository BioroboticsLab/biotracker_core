#include "biotracker/zmq/ZmqInfoFile.h"

#include <QFile>
#include <QTextStream>
#include "biotracker/Exceptions.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

ZmqInfoFile::ZmqInfoFile(const boost::filesystem::path &path) {
    if (!boost::filesystem::exists(path)) {
        throw file_not_found("Could not find file " + path.string());
    }

    QFile infoFile(QString::fromStdString(path.string()));

    if (!infoFile.open(QIODevice::ReadOnly)) {
        throw device_open_error("Could not open file " + path.string());
    }

    int c = 0;
    QString name = QString::fromStdString(path.stem().string());
    QString path_folder = QString::fromStdString(path.string());
    const int last = path_folder.length() - 1;
    const int first = last - (int)path.filename().string().length();
    path_folder = path_folder.remove(first, last);

    QString program;
    QStringList arguments;

    QTextStream in(&infoFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.startsWith("#")) {
            if (c == 0) {
                program = line;
                c += 1;
            } else if (c == 1) {
                arguments << path_folder + "/" + line;
                c+= 1;
                break;
            }
        }
    }
    if (c != 2) {
        throw device_open_error("Malformed *.zmq file: " + path.string());
    }

    m_name = name;
    m_program = program;
    m_arguments = arguments;

}



}
}
}
