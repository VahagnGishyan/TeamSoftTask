
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "UserListener.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace TeamSoftTask;

///////////////////////////////////////////////////////////////////////////////////////////////////////

class TaskListLog : public TaskList
{
    void PrintSelectedTasks(const Selected &stasks) const override
    {
        if (stasks.empty())
        {
            Logging("selected tasks is empty");
        }

        for (auto &&task : stasks)
        {
            Logging((*task).ToString());
        }
        Logging("");
    }
};

TEST(TeamSoft_UserListener, Core)
{
    {
        std::unique_ptr<UserListener> listener = std::make_unique<UserListenerLog>(std::move(std::make_unique<TaskListLog>()));
        std::string input = "add \"some-name\" \"some-desc\" \"2013-08-5 19:05\" \"cat1\"";
        auto command = listener->ParseInput(input);
        ASSERT_TRUE(command->GetCommand() == TaskList::Command::Add);
        listener->Do(command);
    }
    {
        std::unique_ptr<UserListener> listener = std::make_unique<UserListenerLog>(std::move(std::make_unique<TaskListLog>()));

        listener->StartListening();
        const std::string exit = listener->InputForStopListening(); // input this, for stop working

        // temp
        // input = listener->ReadInput();
        listener->Do(listener->ParseInput("add \"some-name-0\" \"some-desc-0\" \"3-5-5 19:05\" \"cat1\""));
        listener->Do(listener->ParseInput("add \"some-name-1\" \"some-desc-1\" \"9993-9-5 19:05\" \"cat1\""));
        listener->Do(listener->ParseInput("add \"some-name-2\" \"some-desc-2\" \"2563-12-5 19:05\" \"cat1\""));
        listener->Do(listener->ParseInput("add \"some-name-3\" \"some-desc-3\" \"2013-10-5 19:05\" \"cat1\""));
        listener->Do(listener->ParseInput("add \"some-name-4\" \"some-desc-4\" \"23-1-5 19:05\" \"cat1\""));
        listener->Do(listener->ParseInput("add \"some-name-5\" \"some-desc-5\" \"0-0-5 19:05\" \"cat1\""));

        listener->Do(listener->ParseInput("add \"any-name-0\" \"any-desc-5\" \"400-0-5 19:05\" \"cat0\""));
        listener->Do(listener->ParseInput("add \"any-name-1\" \"any-desc-2\" \"0-0-5 19:05\" \"cat1\""));
        listener->Do(listener->ParseInput("add \"any-name-2\" \"any-desc-2\" \"0-0-5 19:05\" \"cat2\""));

        listener->Do(listener->ParseInput("done \"some-name-4\""));
        listener->Do(listener->ParseInput("delete \"some-name-3\""));
        listener->Do(listener->ParseInput("delete \"some-name-4\""));

        listener->Do(listener->ParseInput("select *   "));

        listener->Do(listener->ParseInput("done \"any-name-0\""));
        listener->Do(listener->ParseInput("done \"some-name-1\""));

        listener->Do(listener->ParseInput("select * where { status = \"ON\" and status = \"OFF\" }"));
        listener->Do(listener->ParseInput("select * where { name like \"some-name\"  and description like \"some-\" }"));
        listener->Do(listener->ParseInput("select * where { date > \"2010-0-0 00:00\" }"));
        listener->Do(listener->ParseInput("select * where { date < \"2010-0-0 00:00\" }"));

        listener->Do(listener->ParseInput("select * where { date < \"1000-0-0 00:00\" and status = \"OFF\" and name like \"any-name\" and category like \"t0\" }"));
        listener->StopListening();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
