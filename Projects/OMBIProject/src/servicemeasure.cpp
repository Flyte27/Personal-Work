/**
  *A class that stores all the data for a ServiceMeasure object
  * it stores the Municipality and year as a key pair for a specific value.
  * @author Alex and Raghad.
  */

#include "servicemeasure.h"
#include "dirent.h"

#include <dirent.h>
#include <stdio.h>
#include <string>
#include <utility>
#include <QString>
#include <fstream>
#include <iostream>
#include <sstream>
#include <QDir>

using namespace std;

/**
 * A  default public constructor.
 *@brief ServiceMeasure::ServiceMeasure
 */
ServiceMeasure::ServiceMeasure()
{

}

/**
 * A public constructor
 * @param QString measureID , QString title of the measure, QString type of the measure, char priority, QString serviceArea
 * @brief ServiceMeasure::ServiceMeasure
 */
ServiceMeasure::ServiceMeasure(QString measureID, QString title, QString type, char priority, QString serviceArea)
{
    _measureID = measureID;
    _title = title;
    _type = type;
    if (type.compare(QString::fromStdString("percent")) == 0){
        _title.append(QString::fromStdString("\n(%)"));
    }
    else if (type.compare(QString::fromStdString("dollar")) == 0){
        _title.append(QString::fromStdString("\n($)"));
    }
    _priority = priority;
    loadServiceMeasure(serviceArea);
}
/**
 * A destructor.
 * @brief ServiceMeasure::~ServiceMeasure
 */
ServiceMeasure::~ServiceMeasure()
{

}

/**
 * A public getter: Returns the measure ID.
 * @brief ServiceMeasure::getMeasureID
 * @return QString _measureID
 */
QString ServiceMeasure::getMeasureID() const
{
    return _measureID;
}

/**
 * A public getter: Returns the measure title.
 * @brief ServiceMeasure::getTitle
 * @return QString _title
 */
QString ServiceMeasure::getTitle()
{
    return _title;

}

/**
 * A public getter: Returns the measure proprity
 * @brief ServiceMeasure::getPriority
 * @return char _priority
 */
char ServiceMeasure::getPriority()
{
    return _priority;
}

/**
 * A public getter: Returns the type of the measure.
 * @brief ServiceMeasure::getType
 * @return QString _type
 */
QString ServiceMeasure::getType()
{
    return _type;
}

/**
* A public getter: Returns a vector of all possible Municipalities
* @brief ServiceMeasure::getMunicipalities
* @return QVector storing all Municipalities
*/
QVector<QString> ServiceMeasure::getMunicipalities()
{
    return this->_municipalities;

}

/**
* A public getter: Returns a vector of all possible years
* @return QVector storing all years
*/
QVector<int> ServiceMeasure::getYears()
{
    return this->_years;
}

/**
* A public getter: Method used to retrieve the value of a Municipality/year
* combination for a specific service.
* @param valuePair: Pair passed into map to return it's value
* @return double variable of the value of the given pair.
*/
double ServiceMeasure::getValue(std::pair<QString, int> valuePair)
{
    return _MunicipalityModel[valuePair];

}
/**
 * A method that loads all the measures of a service area from the csv files.
 * @brief ServiceMeasure::loadServiceMeasure
 * @param QString The name of service area
 */
void ServiceMeasure::loadServiceMeasure(QString serviceArea)
{
    QDir tempDir;
    string tempStr = tempDir.absolutePath().toUtf8().constData();
    tempStr = tempStr.substr(0, tempStr.length() - 24) + "src/OMBI_csvDB/" + serviceArea.toUtf8().constData() + "/" + _measureID.toUtf8().constData() + ".csv";
    const char *fileName;
    fileName = tempStr.c_str();
    ifstream dataFile (fileName);
    string current;

    if (dataFile.good()){
        getline(dataFile, current, '\n');
        current = current.substr(13, current.length()-1);
        string year;
        while (current.length() >= 4){
            year = current.substr(0, 4);
            if (current.length() > 4){
                current = current.substr(5, current.length()-1);
            }
            else if (current.length() == 4){
                current = "";
            }
            _years << atoi(year.c_str()); //Gets each recorded year from the CSV
        }
    }
    while (dataFile.good())
    {
        getline (dataFile, current, '\n');
        string currentCity;
        stringstream ss(current);
        getline (ss, currentCity, ',');
        if (currentCity.compare("") != 0)
        _municipalities << QString::fromStdString(currentCity);
        for (int i = 0; i < _years.count(); i++){
            if (ss.str().length() > 0){
                getline (ss, current, ',');
                _MunicipalityModel[make_pair(QString::fromStdString(currentCity), _years.at(i))] = atof(current.c_str());
            }
            else{
                _MunicipalityModel[make_pair(QString::fromStdString(currentCity), _years.at(i))] = 0;
            }
        }
    }
    dataFile.close();
}
// end of the class.
