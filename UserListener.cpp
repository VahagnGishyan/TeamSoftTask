
#include "UserListener.hpp"

namespace TeamSoftTask
{
	IUserCommand::~IUserCommand() {}
	TaskList::Command IUserCommand::GetCommand() const 
	{
		return (m_command);
	}
	void IUserCommand::SetCommand(const TaskList::Command& command)
	{
		m_command = command;
	}

	UCommandName::UCommandName(const TaskList::Command& command)
	{
		m_command = command;
	}
	Task::Name UCommandName::GetName() const
	{
		return (m_name);
	}
	void UCommandName::SetName(const Task::Name& name)
	{
		m_name = name;
	}

	UCommandAdd::UCommandAdd() : UCommandName(TaskList::Command::Add) {}
	void UCommandAdd::AddAttribute(const std::string& att)
	{
		m_attributes.push_back(att);
	}
	Attributes& UCommandAdd::GetAttributes()
	{
		return (m_attributes);
	}
	const Attributes& UCommandAdd::GetAttributes() const
	{
		return (m_attributes);
	}

	/////////////////////////////////////////////////////////////////////////////

	UCommandSelect::UCommandSelect()
	{
		m_command = TaskList::Command::Select;
	}

	void UCommandSelect::AddCondition(TaskList::Condition&& cond)
	{
		m_query.push_back(std::move(cond));
	}

	TaskList::QueryForSelect& UCommandSelect::GetQuery()
	{
		return (m_query);
	}

	const TaskList::QueryForSelect& UCommandSelect::GetQuery() const
	{
		return (m_query);
	}

	/////////////////////////////////////////////////////////////////////////////

	UserListener::UserListener(std::unique_ptr<TaskList> tasklist) : m_tasklist(std::move(tasklist)) {}
	UserListener::~UserListener() {}

	void UserListener::StartListening()
	{
		std::cout << "For stop working enter \"" << InputForStopListening() << "\"" << std::endl << std::endl;
	}

	std::string UserListener::InputForStopListening()
	{
		return ("q");
	}

	std::string UserListener::ReadInput()
	{
		std::string line = "";
		while ( line == "")
		{
			std::getline(std::cin >> std::ws, line);
			//std::getline(std::cin, line);
		}
		return (line);
	}

	const std::unique_ptr<IUserCommand> UserListener::ParseInput(std::string line)
	{
		std::string delimiter = " ";
		std::string token = line.substr(0, line.find(delimiter));
		line.erase(0, line.find(delimiter) + delimiter.length());

		if (token == "add")
		{
			return (ParseAdd(line));
		}
		else if (token == "done")
		{
			return (ParseDone(line));
		}
		else if (token == "update")
		{
			return (ParseUpdate(line));
		}
		else if (token == "delete")
		{
			return (ParseDelete(line));
		}
		else if (token == "select")
		{
			line.erase(0, line.find('*') + 1);
			return (ParseSelect(line));
		}
		else
		{
			throw std::invalid_argument("UserListener.cpp, ParseInput(const std::string& line), first token { " +
				token + ", size is " + std::to_string(token.size()) + " } is unknown");
		}
	}

	void UserListener::Do(const std::unique_ptr<IUserCommand>& command)
	{
		switch (command->GetCommand())
		{
		case TaskList::Command::Add		: Add(dynamic_cast<const UCommandAdd&>(*command));	break;
		case TaskList::Command::Done	: Done(dynamic_cast<const UCommandName&>(*command));	break;
		case TaskList::Command::Update	: Update(dynamic_cast<const UCommandName&>(*command));	break;
		case TaskList::Command::Delete	: Delete(dynamic_cast<const UCommandName&>(*command));	break;
		case TaskList::Command::Select	: Select(dynamic_cast<const UCommandSelect&>(*command)); break;
		default: throw std::invalid_argument("UserListener.cpp, Do(const std::UserCommand& command), unknown command"); 
		}
	}

	const std::string UserListener::HandleException(const std::exception &err)
	{
		return (InputForStopListening());
	}

	void UserListener::StopListening()
	{
	}

	/////////////////////////////////////////////////////////////////////////////

	static void SetAttributes(UCommandAdd& command, const std::string& line, int istok, int ietok)
	{
		while ((size_t)istok < line.size())
		{
			istok = line.find('\"', istok) + 1;
			ietok = line.find('\"', istok);
			command.AddAttribute({ &line[istok], &line[ietok] });
			istok = ietok + 1;
		}
	}

	static void SetAttributes(UCommandName& command, const std::string& line)
	{

		int istok = 0;
		int ietok = 0;

		istok = line.find('\"', istok) + 1;
		ietok = line.find('\"', istok);
		command.SetName({ &line[istok], &line[ietok] });
		istok = ietok + 1;

		if (command.GetCommand() == TaskList::Command::Add)
		{
			SetAttributes((dynamic_cast<UCommandAdd&>(command)), line, istok, ietok);
		}
	}

	/////////////////////////////////////////////////////////////////////////////

	void UserListener::Add(const UCommandAdd& ucammand)
	{
		const std::string& name			= ucammand.GetName();
		Attributes::const_iterator itr	= ucammand.GetAttributes().begin();
		const std::string& description	= *(itr++); 
		const std::string& datestr		= *(itr++);
		const std::string& category		= *(itr++);
		m_tasklist->Add(name, description, datestr, category);
	}

	void UserListener::Done(const UCommandName& ucammand)
	{
		m_tasklist->Done(ucammand.GetName());
	}
	void UserListener::Update(const UCommandName& ucammand)
	{
		m_tasklist->Delete(ucammand.GetName());
		std::unique_ptr<UCommandAdd> command = std::make_unique<UCommandAdd>();
		std::string line;
		std::getline(std::cin, line);
		SetAttributes(*command, line);
		Add(*command);
	}
	void UserListener::Delete(const UCommandName& ucammand)
	{
		m_tasklist->Delete(ucammand.GetName());
	}

	/////////////////////////////////////////////////////////////////////////////

	void UserListener::Select(const UCommandSelect& ucommmand)
	{
		m_tasklist->Select(ucommmand.GetQuery());
	}

	/////////////////////////////////////////////////////////////////////////////

	std::unique_ptr<IUserCommand> UserListener::ParseAdd(const std::string& line)
	{
		// add name description date category => add "some name" "description" "datestr" "category" 

		std::unique_ptr<UCommandAdd> command = std::make_unique<UCommandAdd>();
		SetAttributes(*command, line);
		if (command->GetAttributes().size() != 3)
		{
			throw std::invalid_argument("UserListener.cpp, ParseAdd(std::string& line), command.m_attributes.size must be 4");
		}

		return (command);
	}

	std::unique_ptr<IUserCommand> UserListener::ParseDone(const std::string& line)
	{
		// done name => done "some name"
		std::unique_ptr<UCommandName> command = std::make_unique<UCommandName>(TaskList::Command::Done);
		SetAttributes(*command, line);
		return (command);
	}

	std::unique_ptr<IUserCommand> UserListener::ParseUpdate(const std::string& line)
	{
		// update name
		std::unique_ptr<UCommandName> command = std::make_unique<UCommandName>(TaskList::Command::Update);
		SetAttributes(*command, line);
		return (command);
	}

	std::unique_ptr<IUserCommand> UserListener::ParseDelete(const std::string& line)
	{
		// delete name
		std::unique_ptr<UCommandName> command = std::make_unique<UCommandName>(TaskList::Command::Delete);
		SetAttributes(*command, line);
		return (command);
	}
	
	/////////////////////////////////////////////////////////////////////////////

	static void SplitStringQuery(const std::string& strQuery, std::list<std::string>& conditionStringList)
	{
		int index = strQuery.find("where");
		if (index <= 0)
		{
			return;
		}

		std::string delimiter = " and ";

		index = strQuery.find("{", index);
		const int end = strQuery.find("}", index);
		
		if ((index == -1) || (end == -1))
		{
			throw std::invalid_argument("UserListener.cpp, SplitStringQuery, condition must open and close whith {}");
		}
		index += 1;

		int next = index;

		while (index < end)
		{
			next = strQuery.find(delimiter, index);
			if (next == -1) 
			{
				conditionStringList.push_back({ &strQuery[index], &strQuery[end] });
				break;
			}

			conditionStringList.push_back({ &strQuery[index], &strQuery[next] });
			index = next + delimiter.size();
		}
	}

	static void ParseTextCondition(const std::string& condstring, int start, TaskList::TextCondition& condition)
	{
		// {=} or {like} "some-text"
		const std::string eq = "=";
		const std::string like = "like";

		int from = condstring.find('\"', start) + 1;
		int to = condstring.find('\"', from);
		std::string value(&condstring[from], &condstring[to]);

		int index = condstring.find(eq, start);
		if (index >= 0)
		{
			condition.SetCondition(value, TaskList::TextConditionType::Equals);
		}
		index = condstring.find(like, start);
		if (index >= 0)
		{
			condition.SetCondition(value, TaskList::TextConditionType::Like);
		}
	}

	static TaskList::Condition ParseNameCondition(const std::string& condstring, int from)
	{
		std::unique_ptr<TaskList::NameCondition> cond = std::make_unique<TaskList::NameCondition>();
		ParseTextCondition(condstring, from, *cond);
		return (cond);
	}

	static TaskList::Condition ParseDescriptionCondition(const std::string& condstring, int from)
	{
		std::unique_ptr<TaskList::DescriptionCondition> cond = std::make_unique<TaskList::DescriptionCondition>();
		ParseTextCondition(condstring, from, *cond);
		return (cond);
	}

	static TaskList::Condition ParseCategoryCondition(const std::string& condstring, int from)
	{
		std::unique_ptr<TaskList::CategoryCondition> cond = std::make_unique<TaskList::CategoryCondition>();
		ParseTextCondition(condstring, from, *cond);
		return (cond);
	}

	static TaskList::Condition ParseStateCondition(const std::string& condstring, int from)
	{
		// state = "{value}"
		// {value} is ON or OFF
		const std::string on = "\"ON\"";
		const std::string off = "\"OFF\"";

		int index = condstring.find('=', from);
		if (index != -1)
		{
			std::unique_ptr<TaskList::StatusCondition> cond = std::make_unique < TaskList::StatusCondition>();
			index = condstring.find(on, from);
			if (index >= 0)
			{
				cond->SetStatus(TaskList::Status::On);
				return cond;
			}
			index = condstring.find(off, from);
			if (index >= 0)
			{
				cond->SetStatus(TaskList::Status::Off);
				return cond;
			}
		}

		throw std::invalid_argument("UserListener.cpp, ParseStateCondition, status condition must have {=}, {\"ON\"}, or {\"OFF\"}");
	}

	static TaskList::Condition ParseDateCondition(const std::string& condstring, int start)
	{
		std::list<std::pair<std::string, TaskList::DateConditionType>> list
		{
			{ " = ", TaskList::DateConditionType::Equals },
			{ " > ", TaskList::DateConditionType::Greater },
			{ " < ", TaskList::DateConditionType::Less },
			{ " >= ", TaskList::DateConditionType::GreaterEq },
			{ " <=", TaskList::DateConditionType::LessEq },
		};

		int index = -1;
		TaskList::DateConditionType type;
		for (auto&& element : list)
		{
			index = condstring.find(element.first, start);
			if (index != -1)
			{
				type = element.second;
				index += element.first.size();
				break;
			}
		}

		if (index == -1)
		{
			throw std::invalid_argument("UserListener.cpp, ParseDateCondition, date condition must have operator, {=}, {>}, {<}, {=}, {>=}, {<=}");
		}

		int from = condstring.find('\"', index) + 1;
		int to = condstring.find('\"', from);
		std::string value(&condstring[from], &condstring[to]);

		std::unique_ptr<TaskList::DateCondition> cond = std::make_unique <TaskList::DateCondition>();
		cond->SetCondition(value, type);
		return (cond);
	}

	static TaskList::Condition ParseCondition(const std::string& condstring)
	{
		// first token
		int from = condstring.find_first_not_of(' ');
		int to = condstring.find(' ', from);

		std::string token(&condstring[from], &condstring[to]);

		if (token == "name")
		{
			return (ParseNameCondition(condstring, from));
		}
		if (token == "description")
		{
			return (ParseDescriptionCondition(condstring, from));
		}
		if (token == "category")
		{
			return (ParseCategoryCondition(condstring, from));
		}
		if (token == "status")
		{
			return (ParseStateCondition(condstring, from));
		}
		if (token == "date")
		{
			return (ParseDateCondition(condstring, from));
		}

		return nullptr;
	}

	std::unique_ptr<IUserCommand> UserListener::ParseSelect(const std::string& line)
	{
		// select * where{ predicate } 
		// example = > predicate - date < "2020-12-12 00:00" and category = "cat1" and status = "on" and description like "text"
		// < , <= , =, >= , >
		// like is substr
		// select * 

		std::unique_ptr<UCommandSelect> command = std::make_unique<UCommandSelect>();
		std::list<std::string> condStrList;
		SplitStringQuery(line, condStrList);
		for (auto&& condition : condStrList)
		{
			command->AddCondition(ParseCondition(condition));
		}
		return (command);
	}

	/////////////////////////////////////////////////////////////////////////////

	UserListenerLog::UserListenerLog(std::unique_ptr<TaskList> tasklist) : UserListener(std::move(tasklist)) {}

	void UserListenerLog::StartListening()
	{
		Logging("Call UserListenerLog::StartListening();");
		UserListener::StartListening();
	}
	std::string UserListenerLog::InputForStopListening()
	{
		Logging("Call UserListenerLog::InputForStopListening();");
		return (UserListener::InputForStopListening());
	}
	std::string UserListenerLog::ReadInput()
	{
		std::string value = UserListener::ReadInput();
		Logging("Call UserListenerLog::ReadInput();, input is { " + value + " } ");
		return (value);
	}
	const std::unique_ptr<IUserCommand> UserListenerLog::ParseInput(std::string line)
	{
		Logging("Call UserListenerLog::ParseInput();, line is { " + line + " } ");
		return (UserListener::ParseInput(line));
	}
	void UserListenerLog::Do(const std::unique_ptr<IUserCommand>& command)
	{
		Logging("Call UserListenerLog::Do();");
		return (UserListener::Do(command));
	}
	void UserListenerLog::StopListening()
	{
		Logging("Call UserListenerLog::StopListening();");
		UserListener::StopListening();
	}
}
