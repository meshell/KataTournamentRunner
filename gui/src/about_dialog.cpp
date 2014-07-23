#include "tournament_runner_gui/about_dialog.h"
#include "ui_about_dialog.h"

#include <QString>

namespace TournamentRunnerGUI
{

AboutDialog::AboutDialog (const QString& version,
                          const QString& minor_version,
                          const QString& patch_version,
                          QWidget* parent) :
    QDialog{parent},
    ui_{std::make_shared<Ui::AboutDialog>()}
{
    ui_->setupUi(this);

    auto version_string = version;
    if (!minor_version.isEmpty())
    {
        version_string += "." + minor_version;
    }
    if (!patch_version.isEmpty())
    {
        version_string += "." + patch_version;
    }
    ui_->version_->setText(ui_->version_->text().arg(version_string));
}

} // namespace TournamentRunnerGUI
