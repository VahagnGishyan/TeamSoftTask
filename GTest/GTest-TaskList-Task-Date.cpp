
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "TaskList.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace TeamSoftTask;

///////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(TeamSoft_TaskList, Date_True)
{
    std::list<std::string> dates;
    
    dates.push_back("0-0-0 00:00");
    dates.push_back("2002-12-08 08:34");
    dates.push_back("0-12-08 11:59");
    dates.push_back("1-12-31 12:00");
    dates.push_back("295-12-31 12:00");
    dates.push_back("365-12-31 12:05");
    dates.push_back("2002-12-31 16:00");
    dates.push_back("2002-12-31 12:00");
    dates.push_back("2002-12-07 12:00");
    dates.push_back("22-12-31 12:00");
    dates.push_back("20-12-18 12:00");
    dates.push_back("1-12-31 12:00");
    dates.push_back("2-5-29 12:00");
    dates.push_back("2013-08-5 19:05");
    dates.push_back("2013-8-05 19:05");
    dates.push_back("2013-10-05 19:05");
    dates.push_back("2013-09-05 19:05");
    dates.push_back("2013-12-05 19:05");
    dates.push_back("2013-0-05 19:05");
    dates.push_back("2013-10-0 19:05");
    dates.push_back("2013-10-5 19:05");
    dates.push_back("2013-10-06 19:05");
    dates.push_back("2013-10-16 19:05");

    for (const auto &data : dates)
    {
        bool result = Task::Date::IsStringDataFormat(data);
        if (result == false)
        {
            std::cout << "for date " << data << " regex return false, expect true" << std::endl;
        }
        EXPECT_TRUE(result);
    }
}

TEST(TeamSoft_TaskList, Date_False)
{
    std::list<std::string> dates;

    dates.push_back("00-0-0 00:00");
    dates.push_back("000-0-0 00:00");
    dates.push_back("0000-0-0 00:00");
    dates.push_back("00000-0-0 00:00");
    dates.push_back("0-0-0 000:000");
    dates.push_back("0-0-0 0:0");
    dates.push_back("-2002-12-08 08:34");
    dates.push_back("07-12-08 14:59");
    dates.push_back("0-0-0 00:69");
    dates.push_back("295-12-41 12:00");
    dates.push_back("365-12-31 -8:05");
    dates.push_back("20099-12-31 76:00");
    dates.push_back("-12-31 12:00");
    dates.push_back("2002--07 12:00");
    dates.push_back("2013-14-05 19:05");
    dates.push_back("22-12- 12:00");
    dates.push_back("20-12-18 :00");
    dates.push_back("2002-13-31 12:00");
    dates.push_back("2002-13-31 12:");
    dates.push_back("-- :");

    for (const auto &data : dates)
    {
        bool result = Task::Date::IsStringDataFormat(data);
        if (result == true)
        {
            std::cout << "for date " << data << " regex return true, expect false" << std::endl;
        }
        EXPECT_FALSE(result);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
