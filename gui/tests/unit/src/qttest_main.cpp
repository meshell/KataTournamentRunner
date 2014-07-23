#include <gtest/gtest.h>

#include <QApplication>


int main(int argc, char **argv)
{
    QApplication application{argc, argv};

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
