#include <gtest/gtest.h>

#include <boost/log/core.hpp>

int main(int argc, char** argv)
{
    boost::log::core::get()->set_logging_enabled(false);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}