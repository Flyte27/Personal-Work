/**
  *A class that stores all the data for a ServiceMeasure object
  * it stores the Municipality and year as a key pair for a specific value.
  * @author Alex and Raghad. 
  */
#ifndef SERVICEMEASURE_H
#define SERVICEMEASURE_H

#include <string>
#include <QString>
#include <map>
#include <QVector>

class ServiceMeasure
{
public:

    /**
     * A  default public constructor.
     *@brief ServiceMeasure::ServiceMeasure
     */
    ServiceMeasure();

    /**
     * A public constructor
     * @param QString measureID , QString title of the measure, QString type of the measure, char priority, QString serviceArea
     * @brief ServiceMeasure::ServiceMeasure
     */
    ServiceMeasure(QString measureID, QString title, QString type, char priority, QString serviceArea);

    /**
     * A destructor.
     * @brief ServiceMeasure::~ServiceMeasure
     */
    ~ServiceMeasure();

    /**
     * A public getter: Returns the measure ID.
     * @brief ServiceMeasure::getMeasureID
     * @return QString _measureID
     */
    QString getMeasureID() const;

    /**
     * A public getter: Returns the measure proprity
     * @brief ServiceMeasure::getPriority
     * @return char _priority
     */
    char getPriority();

    /**
     * A public getter: Returns the type of the measure.
     * @brief ServiceMeasure::getType
     * @return QString _type
     */
    QString getType();


    /**
     * A public getter: Returns the measure title.
     * @brief ServiceMeasure::getTitle
     * @return QString _title
     */
    QString getTitle();

    /**
    * A public getter: Returns a vector of all possible Municipalities
    * @return QVector storing all Municipalities
    */
    QVector<QString> getMunicipalities ();

    /**
    * A public getter: Returns a vector of all possible years
    * @return QVector storing all years
    */
    QVector<int> getYears();

    /**
    * A public getter: Method used to retrieve the value of a Municipality/year
    * combination for a specific service.
    * @param valuePair: Pair passed into map to return it's value
    * @return double variable of the value of the given pair.
    */
    double getValue(std::pair<QString, int> valuePair);

    /**
     * A method that loads all the measures of a service area from the csv files.
     * @brief ServiceMeasure::loadServiceMeasure
     * @param QString The name of service area
     */
    void loadServiceMeasure(QString serviceArea);




private:

    /**
    * A private member:
    */
    QString _measureID;

    /**
    * A private member:
    */
    QString _title;

    /**
    * A private member:
    */
    char _priority;

    /**
    * A private member:
    */
    QString _type;

    /**
    * A private member: Stores the specific values which can be retrieved with a Municipality and a year
    */
    std::map<std::pair<QString, int>, double> _MunicipalityModel;

    /**
    * A private member: Stores the vector of all possible municipalities
    */
    QVector<QString> _municipalities;

    /**
    * A private member: Stores the vector of all possible years
    */
    QVector<int> _years;



};

#endif // SERVICEMEASURE_H
