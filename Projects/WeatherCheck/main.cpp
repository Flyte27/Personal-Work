/*Created by Alex Payne
 *main.cpp is a c++ file used to initiate the application
 *October 7, 2013
*/
#include "mainwindow.h"
#include <QApplication>
/*Name: main
 *Description: Main method, runs on application startup
 *Parameter Descriptions: Integer of number of arguments, and array of characters holding arguments
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
