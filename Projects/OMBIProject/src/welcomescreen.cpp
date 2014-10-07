//////////////////// WelcomeScreen ////////////////////

/**
 *  WelcomeScreen class is the welcome screen for
 *  the OMBI project.
 *  @author Vince Marchese
 **/

#include "welcomescreen.h"
#include "ui_welcomescreen.h"
#include "mainwindow.h"
#include <QString>
#include <iostream>

/**
 * @brief   Constructor initializes WelcomeScreen and it's UI.
 * @param   parent    A pointer to the QWidget parent.
 */
WelcomeScreen::WelcomeScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeScreen) {

    ui->setupUi(this);
    _start = ui->start_button;
    this->setWindowTitle("OMBI");

    ui->how_to->setVisible(false);
    connect(ui->info_button, SIGNAL(clicked()), this, SLOT(toggleInfo()));

    // Testing variable, true or false (default).
    bool testing = false;

    // Add OMBI logo from Resources.
    QString logo(":/images/ombi-logo.png");
    QPixmap img(logo);
    ui->logo->setPixmap(img);

    // Start municipalities slideshow.
    QMovie *movie = new QMovie(":/animations/muni-slideshow.gif");
    ui->slideshow->setMovie(movie);
    movie->start();

    // **Automated Testing Only**
    if (testing) {
        /* Test #1 - OMBI Logo */
        if(img.isNull())
            std::cout << "Test #1 - Logo:           Fail" << std::endl;
        else
            std::cout << "Test #1 - Logo:           Pass" << std::endl;

        /* Test #2 - OMBI Description */
        if(ui->line9->text().isEmpty())
            std::cout << "Test #2 - Description:    Fail" << std::endl;
        else
            std::cout << "Test #2 - Description:    Pass" << std::endl;
    }

}

/**
 * @brief   Destructor.
 */
WelcomeScreen::~WelcomeScreen() {
    delete ui;
}

/**
 * @brief   Gets the Start Button.
 * @return  A pointer to the QPushButton.
 */
QPushButton* WelcomeScreen::getStartButton() {
    return _start;
}

/**
 * @brief   Toggles visibility of About OMBI/How to Get Started.
 */
void WelcomeScreen::toggleInfo() {
    if (ui->about_OMBI->isVisible()) {
        ui->about_OMBI->setVisible(false);
        ui->how_to->setVisible(true);
        ui->info_button->setText("About OMBI");
    }
    else {
        ui->about_OMBI->setVisible(true);
        ui->how_to->setVisible(false);
        ui->info_button->setText("How to Get Started");
    }
}

//////////////////// End of file ////////////////////
