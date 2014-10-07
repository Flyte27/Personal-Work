/**
 * An OMBIData class. Retrieves services and corresponding organizational data
 * and service measures from the OMBI .csv database.
 * @author Alex and Raghad
 */

#ifndef OMBIDATA_H
#define OMBIDATA_H

#include "servicearea.h"
#include "municipalityinfo.h"
#include <string>
#include <QString>
#include <map>
#include <QVector>


class OMBIData {
  public:
    /**
     * Constructor.
     * @brief   Reads through the .csv files of the OMBI database and loads in the
     *          service names and service objects.
     */
    OMBIData();

    /**
     * Destructor.
     * @brief   Deletes OMBIData object from the system.
     */
    ~OMBIData();

    /**
     * @brief   Gets service areas and their information.
     * @return  A QVector of ServiceArea objects.
     */
    QVector<ServiceArea> getServiceAreas();

    /**
     * @breif   Gets service area names.
     * @return  A QVector of service area names.
     */
    QVector<QString> getServiceAreaNames();

private:

    /**
     * A vector of ServiceArea objects.
     */
    QVector<ServiceArea> _serviceAreas;

    /**
     * A vector of service area names.
     */
    QVector<QString> _serviceAreaNames;

};

#endif // OMBIDATA_H
