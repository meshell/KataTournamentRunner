#include <memory>

#include <QApplication>
#include <QtCore/QTranslator>
#include <QLibraryInfo>

#include "tournament_runner/kata_tournament_runner_config.h"
#include "tournament_runner/persistency.h"
#include "tournament_runner/os_helper.h"

#include "tournament_runner_gui/mainwindow.h"
#include "tournament_runner_gui/about_dialog.h"

using ::TournamentRunnerGUI::MainWindow;
using ::TournamentRunnerGUI::AboutDialog;
using TournamentRunner::Tournament;

int main (int argc, char* argv[])
{
    auto default_profile_path = TournamentRunner::OS::get_home_folder();
    if (default_profile_path.empty())
    {
        default_profile_path = "./";
    }

    default_profile_path.append("/.ktr/default.ktr");

    QApplication::setStyle("windowsvista");

    QApplication main_application{argc, argv};

    QTranslator qtTranslator{};

    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    main_application.installTranslator(&qtTranslator);

    QTranslator translator{};
    translator.load("KTR_" + QLocale::system().name(), "translations");
    main_application.installTranslator(&translator);

    const auto version = QString::fromStdString(TournamentRunner::version_major);
    const auto minor_version = QString::fromStdString(TournamentRunner::version_minor);
    const auto patch_version = QString::fromStdString(TournamentRunner::version_patch);
    AboutDialog about_dialog{version, minor_version, patch_version};

    std::shared_ptr<Tournament> tournament{};
    TournamentRunner::Persistency persistency_handler{default_profile_path};
    try
    {
        // load default profile
        tournament = std::make_shared<Tournament>(persistency_handler.load_profile());
    }
    catch (const std::ios_base::failure&)
    {
        // do nothing it is absolutely ok if file doesn't exist yet
    }

    MainWindow main_window{about_dialog, persistency_handler, std::move(tournament)};
    main_window.show();

    return main_application.exec();
}
