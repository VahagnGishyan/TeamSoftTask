
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "TaskList.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace TeamSoftTask;

///////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(TeamSoft_TaskList, Task_Ctor)
{
    {
        Task obj;
        ASSERT_TRUE(obj.GetStatus() == Task::Status::Off);
    }
    {
        std::string tname = "task-name";
        std::string tdescription = "task-description";
        std::string tdatestr = "2002-12-08 08:34";
        std::string tcategory = "category";
        Task obj(tname, tdescription, tdatestr, tcategory);
        ASSERT_TRUE(obj.GetStatus() == Task::Status::On);
        ASSERT_TRUE(obj.GetName() == tname);
        ASSERT_TRUE(obj.GetDescription() == tdescription);
        //ASSERT_TRUE(Task::Date::ConvertToString(obj.GetDate()) == tdatestr);
        ASSERT_TRUE(obj.GetCategory() == tcategory);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
