/**
* @author: Alex Payne, Raghad Alahmadi
* Data class used to store each service area and its measurements,
* along with service area names.
*/
#include "servicearea.h"
#include <fstream>
#include <QDir>
#include <sstream>
#include <iostream>
#include "dirent.h"

using namespace std;

/**
* A constructor.
* @brief Instantiates the Service Area object, for QVector use
*/
ServiceArea::ServiceArea(){

}

/**
* A constructor.
* @param serviceAreaName a string representing the name to be given to the service area object
* @brief Instantiates the Service Area object with a given service
* area name. Also generates measurements for the service area.
*/
ServiceArea::ServiceArea(QString serviceAreaName)
{
    _serviceName = serviceAreaName;

    //Retrieves current pathname to the build file, in order to get project directory
    QDir tempDir;
    string tempStr = tempDir.absolutePath().toUtf8().constData();
    tempStr = tempStr.substr(0, tempStr.length() - 24) + "src/OMBI_csvDB/" + serviceAreaName.toUtf8().constData();

    //Reads the directories in the data folder of the project directory
    DIR *dir = opendir(tempStr.c_str());
    struct dirent *entry = readdir(dir);

    char abrevChars[10];
    strncpy(abrevChars, entry->d_name, 4);
    stringstream ss;
    string abrevString;
    ss << abrevChars;
    ss >> abrevString;
    abrevString = abrevString.substr(0,4);

    //Reads the AvailableMeasures file to determine values for each service measure
    tempStr = tempDir.absolutePath().toUtf8().constData();
    tempStr = tempStr.substr(0, tempStr.length() - 24) + "src/OMBI_csvDB/" + _serviceName.toUtf8().constData() + "/" + abrevString + "_AvailableMeasures.csv";
    const char *fileName;
    fileName = tempStr.c_str();
    ifstream dataFile (fileName);
    string current;

    //Loops through each line of the AvailableMeasures file, getting measureID, title, type and priority for each file of the service area
    getline(dataFile, current, '\n');
    for (int j = 0; dataFile.good(); j++){
        getline(dataFile, current, '\n');
        if (current.compare("") != 0){
            stringstream ss(current);
            vector<string> currentInfo;
            for (int i = 0; i < 4; i++){
                if (i == 1){
                    getline(ss, tempStr, '"');
                    getline(ss, tempStr, '"');
                    if (tempStr.length() > 55){
                        char tempChar = 'a';
                        for (unsigned int i = tempStr.length()/2; tempChar != ' ' && i < tempStr.length(); ++i){
                            tempChar = tempStr.at(i);
                            if (tempChar == ' ') currentInfo.push_back(tempStr.substr(0, i) + "\n" + tempStr.substr(i, tempStr.length()-1));
                        }
                    }
                    else{
                        currentInfo.push_back(tempStr);
                    }
                    getline (ss, tempStr, ',');
                }
                else {
                    getline(ss, tempStr, ',');
                    if (tempStr.length() > 55){
                        char tempChar = 'a';
                        for (unsigned int i = tempStr.length()/2; tempChar != ' ' && i < tempStr.length(); ++i){
                            tempChar = tempStr.at(i);
                            if (tempChar == ' ') currentInfo.push_back(tempStr.substr(0, i) + "\n" + tempStr.substr(i, tempStr.length()-1));
                        }
                    }
                    else{
                        currentInfo.push_back(tempStr);
                    }
                }
            }
            ServiceMeasure temp(QString::fromStdString(currentInfo.at(0)), QString::fromStdString(currentInfo.at(1)), QString::fromStdString(currentInfo.at(2)), currentInfo.at(3)[0], serviceAreaName);
            _serviceMeasures.push_back(temp);
        }
    }
}

/**
* A deconstructor.
* @brief Deconstructs service area object and reallocates memory
*/
ServiceArea::~ServiceArea()
{

}

/**
* A public getter method.
* @brief Retrieves the name of the ServiceArea
* @return Name in QString format
*/
QString ServiceArea::getServiceAreaName() const
{
    return _serviceName;
}

/**
* A public getter method
* @brief Gets a ServiceMeasure object that matches measure ID to the given string.
* @param ID QString variable to be compared to all ID's of Service Measures
* @return The matched ServiceMeasure object otherwise the first object
*/
ServiceMeasure ServiceArea::getServiceMeasure(QString ID) const
{
    for (int i = 0; i < _serviceMeasures.size(); ++i)
    {
        if (_serviceMeasures.at(i).getMeasureID().compare(ID) == 0){
            return _serviceMeasures.at(i);
        }
    }
    return _serviceMeasures.at(0);
}

/**
 * A Public getter method
 * @brief Gets a vector of all measure ID's in a selected service area.
 * @return QVector of QStrings containing all measure ID
 */
QVector<QString> ServiceArea::getServiceMeasuresIDs()
{
    QVector<QString> ServiceMeasuresIDs;

    for (QVector<ServiceMeasure>::iterator it = _serviceMeasures.begin(); it != _serviceMeasures.end(); ++it)
    {
        ServiceMeasuresIDs.push_back(it->getMeasureID());
    }

    return ServiceMeasuresIDs;
}

/**
 * A Public getter method
 * @brief Gets a vector of all measure titles in a selected service area.
 * @return QVector of QStrings containing all measure titles
 */
QVector<QString> ServiceArea::getServiceMeasuresTitles()
{
    QVector<QString> ServiceMeasuresTitles;

    for (QVector<ServiceMeasure>::iterator it = _serviceMeasures.begin(); it != _serviceMeasures.end(); ++it)
    {
        ServiceMeasuresTitles.push_back(it->getTitle());
    }

    return ServiceMeasuresTitles;
}
