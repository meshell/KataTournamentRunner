#include "tournament_runner_gui/mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStackedWidget>

#include "tournament_runner/persistency.h"

#include "tournament_runner_gui/about_dialog.h"
#include "tournament_runner_gui/new_tournament_dialog.h"
#include "tournament_runner_gui/mainframe.h"
#include "tournament_runner_gui/finalframe.h"
#include "tournament_runner_gui/kataround_overview_frame.h"


namespace TournamentRunnerGUI
{

MainWindow::MainWindow (QDialog& about_dialog,
                        TournamentRunner::IPersistency& persistency_handler,
                        std::shared_ptr<TournamentRunner::Tournament> tournament,
                        QWidget* parent) :
    QMainWindow{parent},
    about_dialog_(about_dialog),
    persistency_handler_(persistency_handler),
    ui_{std::make_shared<Ui::MainWindow>()},
    tournament_{tournament}
{
    ui_->setupUi(this);
    set_central_widget();
}

void MainWindow::on_actionAbout_triggered ()
{
    about_dialog_.show();
}

void MainWindow::on_actionNewTournament_triggered ()
{
    auto* tournament_dialog =  new NewTournamentDialog(this);

    connect(tournament_dialog, SIGNAL(new_tournament(TournamentRunner::TournamentData)),
            this, SLOT(create_tournament_slot(TournamentRunner::TournamentData)));

    tournament_dialog->show();
}

void MainWindow::on_actionSave_triggered ()
{
    persistency_handler_.save_profile(*tournament_);
}

void MainWindow::on_actionSaveAs_triggered ()
{
    auto file_dialog = create_file_dialog(QFileDialog::AcceptSave);
    connect(file_dialog, &QFileDialog::fileSelected, [this](QString filename)
    {
        this->persistency_handler_.save_profile(*(this->tournament_), filename.toStdString());
    });
    file_dialog->show();
}

void MainWindow::on_actionOpen_triggered ()
{
    auto file_dialog = create_file_dialog(QFileDialog::AcceptOpen);
    connect(file_dialog, &QFileDialog::fileSelected, [this](QString filename)
    {
        auto tournament = this->persistency_handler_.load_profile(filename.toStdString());
        this->tournament_ = std::make_shared<TournamentRunner::Tournament>(tournament);
        this->set_central_widget();
    });

    file_dialog->show();
}

void MainWindow::create_tournament_slot (TournamentRunner::TournamentData tournament)
{
    tournament_ = std::make_shared<TournamentRunner::Tournament>(tournament);
    set_central_widget();
}

void MainWindow::start_kata_round_slot(uint8_t round)
{
    if (round_frame_widget_index.find(round) == std::end(round_frame_widget_index))
    {
        auto* kata_start_widget = new KataRoundOverviewFrame(*tournament_, round, this);
        connect(kata_start_widget, SIGNAL(next_kata_tournament(uint8_t)),
                this, SLOT(next_kata_round_slot(uint8_t)));
        connect(kata_start_widget, SIGNAL(tournament_finished()),
                this, SLOT(tournament_finished_slot()));

        const auto kata_start_widget_page_index = main_widget_->addWidget(kata_start_widget);

        round_frame_widget_index[round] = kata_start_widget_page_index;
    }

    main_widget_->setCurrentIndex(round_frame_widget_index.at(round));
}

void MainWindow::next_kata_round_slot(uint8_t round)
{
    start_kata_round_slot(round);
    tournament_->start_next_kata_round();
}

void MainWindow::tournament_finished_slot()
{
    auto* final_page = new FinalFrame(*tournament_, this);
    const auto final_page_index = main_widget_->addWidget(final_page);
    main_widget_->setCurrentIndex(final_page_index);
}

QFileDialog* MainWindow::create_file_dialog (QFileDialog::AcceptMode mode)
{
    auto* file_dialog = new QFileDialog(this);
    file_dialog->setAcceptMode(mode);
    set_file_dialog_filter(file_dialog);
    return file_dialog;
}

void MainWindow::set_file_dialog_filter (QFileDialog* file_dialog)
{
    auto name_filters = file_dialog->nameFilters();
    name_filters.push_front(QString{"*.ktr"});
    file_dialog->setNameFilters(name_filters);
}

void MainWindow::set_central_widget ()
{
    if (tournament_ != nullptr)
    {
        main_widget_ = new QStackedWidget();
        auto* tournament_main_widget = new MainFrame(*tournament_, this);
        connect(ui_->actionAddParticipant, SIGNAL(triggered()),
                tournament_main_widget, SLOT(on_AddParticipantButton_clicked()));
        connect(ui_->actionEditTournament, SIGNAL(triggered()),
                tournament_main_widget, SLOT(on_EditTournamentButton_clicked()));
        connect(tournament_main_widget, SIGNAL(start_kata_tournament(uint8_t)),
                this, SLOT(start_kata_round_slot(uint8_t)));

        main_widget_->addWidget(tournament_main_widget);

        setCentralWidget(main_widget_);
        ui_->menuEdit->setEnabled(true);
    }
}


} // namespace TournamentRunnerGUI
