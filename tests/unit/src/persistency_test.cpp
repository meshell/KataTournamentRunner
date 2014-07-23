#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "tournament_runner/tournament.h"
#include "tournament_runner/persistency.h"
#include "tournament_runner/os_helper.h"

using ::TournamentRunner::TournamentData;
using ::TournamentRunner::Persistency;
using boost::filesystem::path;

namespace
{

class The_Persistency_class :
    public ::testing::Test
{
public:
    void SetUp() override;
    void TearDown() override;

protected:
    boost::filesystem::path profile_file_;
    boost::filesystem::path profile_file_in_folder_;
    boost::filesystem::path default_profile_;
};

void The_Persistency_class::SetUp()
{
    profile_file_ = boost::filesystem::temp_directory_path();

    profile_file_ /= "test_profile.ktm";

    profile_file_in_folder_ = boost::filesystem::temp_directory_path();

    profile_file_in_folder_ /= "KTM";
    profile_file_in_folder_ /= "test_profile.ktm";

    auto default_profile_path = TournamentRunner::OS::get_home_folder();
    if (default_profile_path.empty())
    {
        default_profile_path = "./";
    }

    default_profile_ = path(default_profile_path);
    default_profile_ /= path(".ktmtest");
    default_profile_ /= path("default.ktm");

}


void The_Persistency_class::TearDown()
{
    if (boost::filesystem::exists(profile_file_))
    {
        boost::filesystem::remove(profile_file_);
    }

    if (boost::filesystem::exists(profile_file_in_folder_))
    {
        boost::filesystem::remove_all(profile_file_in_folder_.parent_path());
    }

    if (boost::filesystem::exists(default_profile_))
    {
        boost::filesystem::remove_all(default_profile_.parent_path());
    }
}


TEST_F(The_Persistency_class, should_create_a_file_at_the_given_location_when_saving_a_profile)
{
    auto testee = Persistency{};
    auto tournament = TournamentRunner::Tournament{};


    testee.save_profile(tournament, profile_file_.string());

    ASSERT_TRUE(boost::filesystem::exists(profile_file_));
    ASSERT_TRUE(boost::filesystem::is_regular_file(profile_file_));
}

TEST_F(The_Persistency_class, should_throw_an_exception_if_the_file_can_not_be_saved)
{
    auto testee = Persistency{};
    auto tournament = TournamentRunner::Tournament{};

    const auto profile_file_without_permission = std::string{"/test_profile.ktm"};

    ASSERT_THROW(testee.save_profile(tournament, profile_file_without_permission), std::ios_base::failure);
}

TEST_F(The_Persistency_class, should_throw_an_exception_if_the_profile_file_does_not_exist_when_loading_a_profile)
{
    Persistency testee{};

    auto non_existing_profile_file =  boost::filesystem::temp_directory_path();

    non_existing_profile_file /= "non_existing_profile.ktm";

    ASSERT_THROW(testee.load_profile(non_existing_profile_file.string()), std::ios_base::failure);
}


TEST_F(The_Persistency_class, should_be_able_to_load_a_previously_saved_profile)
{
    Persistency testee{};
    TournamentRunner::Tournament tournament{TournamentData{}
                                            .with_name("SM")
                                            .on_date("2013-06-20")
                                            .at_location("Kriens")
                                            };

    testee.save_profile(tournament, profile_file_.string());


    auto new_tournament = testee.load_profile(profile_file_.string());

    ASSERT_EQ(tournament.name(), new_tournament.name());
}


TEST_F(The_Persistency_class, should_create_the_profile_root_folder_if_it_does_not_exist)
{
    Persistency testee{};
    TournamentRunner::Tournament tournament{};

    testee.save_profile(tournament, profile_file_in_folder_.string());

    ASSERT_TRUE(boost::filesystem::exists(profile_file_in_folder_.parent_path()));
}

TEST_F(The_Persistency_class, should_save_a_previously_loaded_profile_to_the_same_file)
{
    Persistency testee{};
    TournamentRunner::Tournament tournament{TournamentData{}
                                            .with_name("SM")
                                            .on_date("2013-06-20")
                                            .at_location("Kriens")
                                           };


    testee.save_profile(tournament, profile_file_.string());

    testee.load_profile(profile_file_.string());

    boost::filesystem::remove(profile_file_);

    ASSERT_FALSE(boost::filesystem::exists(profile_file_));

    testee.save_profile(tournament);

    ASSERT_TRUE(boost::filesystem::exists(profile_file_));
}

TEST_F(The_Persistency_class, should_load_the_default_profile_when_no_path_is_given)
{
    Persistency testee{default_profile_.string()};
    TournamentRunner::Tournament tournament{TournamentData{}
                                            .with_name("SM")
                                            .on_date("2013-06-20")
                                            .at_location("Kriens")
                                           };


    testee.save_profile(tournament, default_profile_.string());

    const auto new_tournament = testee.load_profile();

    ASSERT_EQ(tournament.name(), new_tournament.name());
}

} //namespace
