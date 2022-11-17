
////////////////////////////////////////////////////////////

#include "TaskList.hpp"
#include "UserListener.hpp"

////////////////////////////////////////////////////////////

namespace TeamSoftTask
{
	class ULForFastTestingWithLog : public UserListenerLog
	{
	public:
		ULForFastTestingWithLog(std::unique_ptr<TaskList> tasklist) : UserListenerLog(std::move(tasklist)) {}

		void StartListening()
		{
			UserListenerLog::StartListening();
			std::cout << "Do you want add some tasks for fast testing ? y/n : ";
			char symbol = 'a';
			std::cin >> symbol;
			while (symbol != 'y' && symbol != 'n')
			{
				std::cout << "Please input 'y' or 'n' : ";
				std::cin >> symbol;
			}
			if (symbol == 'y')
			{
				Do(ParseInput("add \"some-name-0\" \"some-desc-0\" \"3-5-5 19:05\" \"cat1\""));
				Do(ParseInput("add \"some-name-1\" \"some-desc-1\" \"9993-9-5 19:05\" \"cat1\""));
				Do(ParseInput("add \"some-name-2\" \"some-desc-2\" \"2563-12-5 19:05\" \"cat1\""));
				Do(ParseInput("add \"some-name-3\" \"some-desc-3\" \"2013-10-5 19:05\" \"cat1\""));
				Do(ParseInput("add \"some-name-4\" \"some-desc-4\" \"23-1-5 19:05\" \"cat1\""));
				Do(ParseInput("add \"some-name-5\" \"some-desc-5\" \"0-0-5 19:05\" \"cat1\""));

				Do(ParseInput("add \"any-name-0\" \"any-desc-5\" \"400-0-5 19:05\" \"cat0\""));
				Do(ParseInput("add \"any-name-1\" \"any-desc-2\" \"0-0-5 19:05\" \"cat1\""));
				Do(ParseInput("add \"any-name-2\" \"any-desc-2\" \"0-0-5 19:05\" \"cat2\""));
			}
		}

		const std::string HandleException(const std::exception& err) override
		{
			std::cout << "throw exception, message is " << err.what() << std::endl;
			std::cout << "do you want continue ? y/n : ";
			char symbol;
			std::cin >> symbol;
			while (symbol != 'y' && symbol != 'n')
			{
				std::cout << "Please input 'y' or 'n' : ";
				std::cin >> symbol;
			}
			if (symbol == 'y')
			{
				return("");
			}
			return (InputForStopListening());
		}
	};

	void Main()
	{
		std::unique_ptr<UserListener> listener = std::make_unique<ULForFastTestingWithLog>(std::move(std::make_unique<TaskList>()));

		listener->StartListening();
		const std::string exit = listener->InputForStopListening(); // input this, for stop working

		std::string input = "";
		while (input != exit)
		{
			try
			{
				if (input != "")
				{
					std::unique_ptr<IUserCommand> command = listener->ParseInput(input);
					listener->Do(command);
				}
				input = listener->ReadInput();
			}
			catch (std::exception& err)
			{
				input = listener->HandleException(err);
			}
		}

		listener->StopListening();
	}
}

////////////////////////////////////////////////////////////

// I don't like to spoil the global space
int main()
{
	try
	{
		TeamSoftTask::Logging("Start main()");
		TeamSoftTask::Main();
		TeamSoftTask::Logging("Close main()");
	}
	catch (std::exception& err)
	{
		TeamSoftTask::Logging(std::string("Main.cpp, main() catch exception, type is std::exception, message is ") + err.what());
	}
	catch (...)
	{
		TeamSoftTask::Logging(std::string("Main.cpp, main() catch unkown type exception"));
	}
}

////////////////////////////////////////////////////////////
