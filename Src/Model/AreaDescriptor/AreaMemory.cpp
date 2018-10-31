#include "AreaMemory.h"

#include "Utility/misc.h"
#include <qvector.h>
#include <fstream>

/*
TODO: This is still rather prototype'ish.
- The functions are not namespaces, the whole thing is non-OO
- Checking for defaults is icky
- Workflow is hard to see through

*/
#include <qmap.h>

using namespace BioTrackerUtilsMisc; //split
namespace AreaMemory{

    QMap<QString, QVector<QString>> map;

    QVector<QString> getVertices(QString file, QString areaFile, bool useLast) {
        if (map.size() == 0) {
            std::ifstream o;
            o.open(areaFile.toStdString(), std::ifstream::in);
            if (o.is_open()) {
                while (!o.eof()) {
                    std::string line = "";
                    getline(o, line);
                    std::vector<std::string> strs;
                    split(line, strs, '#');
                    if (strs.size() >= 5) {
                        map[strs[0].c_str()] = QVector<QString>{
                            strs[1].c_str(),
                            strs[2].c_str(),
                            strs[3].c_str(),
                            strs[4].c_str() };
                    }
                }
            }
        }

        std::string f = file.toStdString();
        if (map.contains(file))
            return map.value(file);
        else {
            if (useLast && map.contains("Last")) {
                return map.value("Last");
            }else
                return DEFAULT_PAIR;
        }
    }

    void setVertices(QString file, QVector<QString> values, QString areaFile) {
        if (file.size() <= 1) {
            return;
        }

        map[file] = values;
        std::ofstream o;
        o.open(areaFile.toStdString(), std::ofstream::out);

        if (o.is_open()) {
            QMapIterator<QString, QVector<QString>> i(map);
            while (i.hasNext()) {
                i.next();
                o << i.key().toStdString() << "#" << i.value()[0].toStdString()
                    << "#" << i.value()[1].toStdString()
                    << "#" << i.value()[2].toStdString()
                    << "#" << i.value()[3].toStdString() << std::endl;
            }
        }
    }

    std::vector<QPoint> toQPointVector(QString vertices) {
        std::vector<std::string> strs;
        std::vector<QPoint> points;
        std::string s = vertices.toStdString();
        split(s, strs, ';');
        for (int i = 0; i < strs.size(); i++) {
            std::vector<std::string> pt;
            split(strs[i], pt, ',');
            int x = atoi(pt[0].c_str());
            int y = atoi(pt[1].c_str());
            points.push_back(QPoint(x, y));
        }
        return points;
    }

}