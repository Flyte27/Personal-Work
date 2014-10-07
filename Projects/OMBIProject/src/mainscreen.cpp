//////////////////// MainScreen ////////////////////

/**
 *  MainScreen class is the main service selection
 *  screen for the OMBI project.
 *  @author Vince Marchese
 **/

#include "mainscreen.h"
#include "ui_mainscreen.h"
#include "mainwindow.h"
#include <QString>
#include <iostream>

/**
 * @brief   Constructor initializes MainScreen and it's UI.
 * @param   parent    A pointer to the QWidget parent.
 */
MainScreen::MainScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainScreen) {

    ui->setupUi(this);
    _serviceSelection = ui->comboBox;
    _displayButton = ui->display_button;
    this->setWindowTitle("OMBI");

    // Add OMBI logo from Resources.
    QString logo(":/images/ombi-logo.png");
    QPixmap logo_img(logo);
    ui->logo->setPixmap(logo_img);

    // Add fire icon from Resources.
    _fireButton = ui->fire_button;
    QIcon fire_icon(":/images/icons/fire.png");
    _fireButton->setIcon(fire_icon);
    _fireButton->setIconSize(_fireButton->size());

    // Add library icon from Resources.
    _libraryButton = ui->library_button;
    QIcon library_icon(":/images/icons/library.png");
    _libraryButton->setIcon(library_icon);
    _libraryButton->setIconSize(_libraryButton->size());

    // Add parks icon from Resources.
    _parksButton = ui->parks_button;
    QIcon parks_icon(":/images/icons/parks.png");
    _parksButton->setIcon(parks_icon);
    _parksButton->setIconSize(_parksButton->size());

    // Add police icon from Resources.
    _policeButton = ui->police_button;
    QIcon police_icon(":/images/icons/police.png");
    _policeButton->setIcon(police_icon);
    _policeButton->setIconSize(_policeButton->size());

    // Add waste icon from Resources.
    _wasteButton = ui->waste_button;
    QIcon waste_icon(":/images/icons/waste.png");
    _wasteButton->setIcon(waste_icon);
    _wasteButton->setIconSize(_wasteButton->size());

    // Add water icon from Resources.
    _waterButton = ui->water_button;
    QIcon water_icon(":/images/icons/water.png");
    _waterButton->setIcon(water_icon);
    _waterButton->setIconSize(_waterButton->size());

    // Get pointers to radio buttons.
    _tableRadio = ui->table_radio;
    _barRadio = ui->bar_radio;
    _lineRadio = ui->line_radio;

    // Start municipalities slideshow.
    QMovie *movie = new QMovie(":/animations/muni-slideshow.gif");
    ui->slideshow->setMovie(movie);
    movie->start();
}

/**
 * @brief   Destructor.
 */
MainScreen::~MainScreen() {
    delete ui;
}

/**
 * @brief   Gets the Service Area Selector.
 * @return  A pointer to the QComboBox.
 */
QComboBox* MainScreen::getServiceSelector() {
    return _serviceSelection;
}

/**
 * @brief   Gets the Fire Service Area Button.
 * @return  A pointer to the QPushButton.
 */
QPushButton* MainScreen::getFireButton() {
    return _fireButton;
}

/**
 * @brief   Gets the Library Service Area Button.
 * @return  A pointer to the QPushButton.
 */
QPushButton* MainScreen::getLibraryButton() {
    return _libraryButton;
}

/**
 * @brief   Gets the Parks Service Area Button.
 * @return  A pointer to the QPushButton.
 */
QPushButton* MainScreen::getParksButton() {
    return _parksButton;
}

/**
 * @brief   Gets the Police Service Area Button.
 * @return  A pointer to the QPushButton.
 */
QPushButton* MainScreen::getPoliceButton() {
    return _policeButton;
}

/**
 * @brief   Gets the Waste Service Area Button.
 * @return  A pointer to the QPushButton.
 */
QPushButton* MainScreen::getWasteButton() {
    return _wasteButton;
}

/**
 * @brief   Gets the Water Service Area Button.
 * @return  A pointer to the QPushButton.
 */
QPushButton* MainScreen::getWaterButton() {
    return _waterButton;
}

/**
 * @brief   Gets the Table Button.
 * @return  A pointer to the QRadioButton.
 */
QRadioButton* MainScreen::getTableRadio() {
    return _tableRadio;
}

/**
 * @brief   Gets the Bar Chart Button.
 * @return  A pointer to the QRadioButton.
 */
QRadioButton* MainScreen::getBarRadio() {
    return _barRadio;
}

/**
 * @brief   Gets the Line Graph Button.
 * @return  A pointer to the QRadioButton.
 */
QRadioButton* MainScreen::getLineRadio() {
    return _lineRadio;
}

/**
 * @brief   Gets the Display Buttton.
 * @return  A pointer to the QPushButton.
 */
QPushButton* MainScreen::getDisplayButton() {
    return _displayButton;
}

/**
 * @brief   Gets the Display Chart Type.
 * @return  1 if Bar Chart, 2 if Line Graph, 3 if Table.
 */
int MainScreen::getSelectedRadio() {
    if (_barRadio->isChecked())
        return 1;
    else if (_lineRadio->isChecked())
        return 2;
    else
        return 3;
}

//////////////////// End of file ////////////////////
