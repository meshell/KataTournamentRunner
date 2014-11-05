#ifndef TOURNAMENT_RUNNER_GUI_ABOUTDIALOG_H
#define TOURNAMENT_RUNNER_GUI_ABOUTDIALOG_H

#include <memory>

#include <QtWidgets/QDialog>

namespace Ui
{
class AboutDialog;
}


namespace TournamentRunnerGUI
{

/**
 * @ingroup GUI
 *
 * @brief The AboutDialog class
 */
class AboutDialog :
    public QDialog
{
    Q_OBJECT

public:
    /**
     * @ingroup GUI
     * @brief About Dialog constructor
     * @param version Programm version
     * @param minor_version Minor version
     * @param patch_version Patch version
     * @param parent Parent Widget
     */
    AboutDialog (const QString& version,
                 const QString& minor_version,
                 const QString& patch_version,
                 QWidget* parent = 0);

private:
    std::shared_ptr<Ui::AboutDialog> ui_{};
};

} // namespace TournamentRunnerGUI

#endif // TOURNAMENT_RUNNER_GUI_ABOUTDIALOG_H
