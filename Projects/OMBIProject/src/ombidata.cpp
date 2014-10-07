/**
 * An OMBIData class. Retrieves services and corresponding organizational data
 * and service measures from the OMBI .csv database.
 * @author Alex and Raghad
 */

#include "ombidata.h"
#include <dirent.h>
#include <stdio.h>
#include <string>
#include <utility>
#include <QVector>
#include <QString>
#include <fstream>
#include <iostream>
#include <sstream>
#include <QDir>

using namespace std;

/**
 * Constructor.
 * @brief   Reads through the .csv files of the OMBI database and loads in the
 *          service names and service objects.
 */
OMBIData::OMBIData()
{
    QDir tempDir;

    // Fetch the pathname to the OMBI .csv files.
    string tempStr = tempDir.absolutePath().toUtf8().constData();
    tempStr = tempStr.substr(0, tempStr.length() - 24) + "src/OMBI_csvDB/";


    DIR *dir = opendir(tempStr.c_str());

    struct dirent *entry = readdir(dir);

    // Read through the directory and extract all service names and their measures until end of directory is reached.
    while (entry != NULL)
    {
        if (entry->d_name[0] != '.'){
            // Add service name.
            _serviceAreaNames.push_back(entry->d_name);
            ServiceArea tempArea(entry->d_name);
            // Add .csv files under that service.
            _serviceAreas.push_back(tempArea);
        }

        // Read next service file.
        entry = readdir(dir);
    }
    closedir(dir);
}

/**
 * Destructor.
 * @brief   Deletes OMBIData object from the system.
 */
OMBIData::~OMBIData()
{

}

/**
 * @brief   Gets service area names.
 * @return  A QVector of service names.
 */
QVector<QString> OMBIData::getServiceAreaNames()
{
    return _serviceAreaNames;
}

/**
 * @brief   Gets service areas and their information.
 * @return  A QVector of ServiceArea objects.
 */
QVector<ServiceArea>  OMBIData::getServiceAreas()
{
    return _serviceAreas;
}
