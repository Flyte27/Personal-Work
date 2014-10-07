#ifndef SERVICEAREA_H
#define SERVICEAREA_H

#include "servicemeasure.h"
#include <string>
#include <QString>
#include <map>
#include <QVector>

class ServiceArea {

 public:
    /**
     * @brief A public constructor
     */
    ServiceArea();
    /**
     * @brief A public constructor
     * @param Service area name.
     */
    ServiceArea(QString serviceAreaName);

    /**
     * @brief A destructor
     */
    ~ServiceArea();

    /**
     * @brief Gets the service area name.
     */
    QString getServiceAreaName() const;

    /**
     * @brief Gets the ServiceMeasure object.
     */
    ServiceMeasure getServiceMeasure(QString name) const;

    /**
     * @brief Gets the service measure ID's.'
     */
    QVector<QString> getServiceMeasuresIDs();

    /**
     * @brief Gets the service measure titles.
     */
    QVector<QString> getServiceMeasuresTitles();

 private:
    
    /**
     * @brief String containing the service area name.
     */
    QString _serviceName;

    /**
     * @brief A vector of ServiceMeasure objects.
     */
    QVector<ServiceMeasure> _serviceMeasures;


};

#endif // SERVICEAREA_H
