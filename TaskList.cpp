
#include "TaskList.hpp"

namespace TeamSoftTask
{
	////////////////////////////////////////////////////////////

	void Logging(const std::string& message)
	{
		auto lOpenLogFile = []()
		{
			std::ofstream file;
			file.open(MacroLogFileName);
			if (!file)
			{
				throw std::logic_error("dfdfve");
			}
			return file;
		};
		static std::ofstream file = lOpenLogFile();
		file << message << std::endl;
	}

	////////////////////////////////////////////////////////////

	const Task::DateStr Task::Date::ConvertToString(const Task::Date& date)
	{
		std::string value = "";
		value.reserve(16); // size of "1111-11-11 11:11"
		
		value += std::to_string(date.m_year);
		value += '-';
		value += std::to_string(date.m_month);
		value += '-';
		value += std::to_string(date.m_day);
		value += ' ';

		auto addzero = [](std::string& value, const int idate)
		{
			if ((idate / 10) == 0)
			{
				value += '0';
			}
			value += std::to_string(idate);
		};
		
		addzero(value, date.m_hour);
		value += ':';
		addzero(value, date.m_day);

		return (value);
	}

	const bool Task::Date::IsStringDataFormat(const DateStr& dataAsString)
	{
		static const std::regex regex(R"((0|[1-9][0-9]{0,3})-([0-9]|0[0-9]|1[0-2])-([0-9]|[0-2][0-9]|3[0-1]) ([0-1][0-9]|2[0-4]):([0-5][0-9]|60))");
		return (std::regex_match(dataAsString.data(), regex));
	}

	const Task::Date Task::Date::ConvertToDate(const DateStr& line)
	{
		if (!IsStringDataFormat(line))
		{
			throw std::invalid_argument("std::string value { " + line + " } does not match the data format ");
		}

		int iyear_start = 0;
		int iyear_end = line.find('-') - 1;

		int imonth_start = iyear_end + 2;
		int imonth_end = line.find('-', imonth_start) - 1;

		int iday_start = imonth_end + 2;
		int iday_end = line.find(' ', iday_start) - 1;

		int ihour_start = iday_end + 2;
		int ihour_end = line.find(':', ihour_start) - 1;

		int iminute_start = ihour_end + 2;
		int iminute_end = line.size() - 1;

		std::string year(&line[iyear_start], &line[iyear_end + 1]);
		std::string month(&line[imonth_start], &line[imonth_end + 1]);
		std::string day(&line[iday_start], &line[iday_end + 1]);
		std::string hour(&line[ihour_start], &line[ihour_end + 1]);
		std::string minute(&line[iminute_start], &line[iminute_end + 1]);

		Date date;
		date.m_year = std::stoi(year);
		date.m_month = std::stoi(month);
		date.m_day = std::stoi(day);
		date.m_hour = std::stoi(hour);
		date.m_minute = std::stoi(minute);

		return (date);
	}

	////////////////////////////////////////////////////////////

	bool operator==(const Task::Date& lhs, const Task::Date& rhs)
	{
		return ((lhs.m_minute == rhs.m_minute)
			&& (lhs.m_hour == rhs.m_hour)
			&& (lhs.m_day == rhs.m_day)
			&& (lhs.m_month == rhs.m_month)
			&& (lhs.m_year == rhs.m_year));
	}
	bool operator!=(const Task::Date& lhs, const Task::Date& rhs)
	{
		return (!(lhs == rhs));
	}
	bool operator>(const Task::Date& lhs, const Task::Date& rhs)
	{
		if (lhs.m_year > rhs.m_year)
		{
			return(true);
		}
		if (lhs.m_year < rhs.m_year) 
		{ 
			return(false);
		}
		// ==

		if (lhs.m_month > rhs.m_month)
		{
			return(true);
		}
		if (lhs.m_month < rhs.m_month)
		{
			return(false);
		}
		// ==

		if (lhs.m_day > rhs.m_day)
		{
			return(true);
		}
		if (lhs.m_day < rhs.m_day)
		{
			return(false);
		}
		// ==

		if (lhs.m_hour > rhs.m_hour)
		{
			return(true);
		}
		if (lhs.m_hour < rhs.m_hour)
		{
			return(false);
		}
		// ==

		if (lhs.m_minute > rhs.m_minute)
		{
			return(true);
		}
		if (lhs.m_minute < rhs.m_minute)
		{
			return(false);
		}
		// ==

		return (false);
	}
	bool operator<(const Task::Date& lhs, const Task::Date& rhs)
	{
		return ((lhs != rhs) && !(lhs > rhs));
	}
	bool operator>=(const Task::Date& lhs, const Task::Date& rhs)
	{
		return (!(lhs < rhs));
	}
	bool operator<=(const Task::Date& lhs, const Task::Date& rhs)
	{
		return (!(lhs > rhs));
	}

	////////////////////////////////////////////////////////////

	Task::Task() : m_name(), m_description(), m_date(), m_category(), m_status(Status::Off)
	{

	}
	Task::Task(const Name& name, const Description& description, const DateStr& datestr, const Category& category) :
		m_name(name), m_description(description), m_date(Date::ConvertToDate(datestr)), m_category(category), m_status(Status::On)
	{}
	Task::~Task() {}

	////////////////////////////////////////////////////////////

	void Task::SetName(const Name& name)
	{
		m_name = name;
	}
	void Task::SetDescription(const Description& description)
	{
		m_description = description;
	}
	void Task::SetDate(const Date& date)
	{
		m_date = date;
	}
	void Task::SetCategory(const Category& category)
	{
		m_category = category;
	}
	void Task::SetStatus(const Status& status)
	{
		m_status = status;
	}

	////////////////////////////////////////////////////////////

	const Task::Name& Task::GetName() const&
	{
		return(m_name);
	}
	const Task::Description& Task::GetDescription() const&
	{
		return(m_description);
	}
	const Task::Date& Task::GetDate() const&
	{
		return(m_date);
	}
	const Task::Category& Task::GetCategory() const&
	{
		return(m_category);
	}
	const Task::Status& Task::GetStatus() const&
	{
		return(m_status);
	}

	const std::string Task::ToString() const
	{
		std::string value = "";
		value = '\"' + GetName() + "\" \"" + GetDescription() + "\" \"" + Task::Date::ConvertToString(GetDate()) + "\" \"" + GetCategory() + "\"";
		return (value);
	}

	////////////////////////////////////////////////////////////

	std::ostream& operator<<(std::ostream& out, const Task& task)
	{
		std::cout << task.ToString();
		return (out);
	}

	std::istream& operator>>(std::istream& cin, Task& task)
	{
		std::string uinput;
		
		std::cout << "Input name        : ";
		std::getline(cin, uinput); // name
		if (uinput.back() != '\"' || uinput.front() != '\"')
		{
			throw std::logic_error("TaskList.cpp, operator>>, incorrect input");
		}
		task.SetName({ &uinput[1], &uinput[uinput.size() - 1] });

		std::cout << "Input description : ";
		std::getline(cin, uinput); // description
		if (uinput.back() != '\"' || uinput.front() != '\"')
		{
			throw std::logic_error("TaskList.cpp, operator>>, incorrect input");
		}
		task.SetDescription({ &uinput[1], &uinput[uinput.size() - 1] });

		bool iscorrect = true;
		std::cout << "Input date        : ";
		std::getline(cin, uinput); // datestr
		if (uinput.back() != '\"' || uinput.front() != '\"')
		{
			throw std::logic_error("TaskList.cpp, operator>>, incorrect input");
		}
		task.SetDate(Task::Date::ConvertToDate({ &uinput[1], &uinput[uinput.size() - 1] }));

		std::cout << "Input category    : ";
		std::getline(cin, uinput); // category
		if (uinput.back() != '\"' || uinput.front() != '\"')
		{
			throw std::logic_error("TaskList.cpp, operator>>, incorrect input");
		}
		task.SetCategory({ &uinput[1], &uinput[uinput.size() - 1] });

		return(cin);
	}

	////////////////////////////////////////////////////////////

	Task& TaskList::operator[](const Name& name)
	{
		auto itr = std::find_if(m_tasklist.begin(), m_tasklist.end(),
			[&name](const Task& obj)
			{
				return (obj.GetName() == name);
			}
		);
		if (itr == m_tasklist.end())
		{
			throw std::invalid_argument("TaskList.cpp, TaskList::operator[Name]");
		}
		return (*itr);
	}

	////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////

	TaskList::ICondition::~ICondition() {}

	////////////////////////////////////////////////////////////

	void TaskList::TextCondition::SetCondition(const std::string& value, const TaskList::TextConditionType type)
	{
		m_value = value;
		m_type = type;
	}
	const std::string& TaskList::TextCondition::GetValue()
	{
		return (m_value);
	}
	const TaskList::TextConditionType& TaskList::TextCondition::GetTextConditionType()
	{
		return (m_type);
	}
	const bool TaskList::TextCondition::CheckCondition(const std::string& value) const
	{
		if (m_type == TextConditionType::Equals)
		{
			return (m_value == value);
		}
		if (m_type == TextConditionType::Like)
		{
			return (value.find(m_value) != -1);
		}
		throw std::logic_error("TaskList.cpp, TextCondition, CheckCondition, m_type is not correct");
	}

	////////////////////////////////////////////////////////////

	const TaskList::ConditionFor TaskList::NameCondition::GetConditionType() const
	{
		return (TaskList::ConditionFor::Name);
	}

	const TaskList::ConditionFor TaskList::DescriptionCondition::GetConditionType() const
	{
		return (TaskList::ConditionFor::Description);
	}

	const TaskList::ConditionFor TaskList::DateCondition::GetConditionType() const
	{
		return (TaskList::ConditionFor::Date);
	}

	////////////////////////////////////////////////////////////

	const TaskList::ConditionFor TaskList::CategoryCondition::GetConditionType() const
	{
		return (TaskList::ConditionFor::Category);
	}

	////////////////////////////////////////////////////////////

	void TaskList::DateCondition::SetCondition(const Task::Date& value, const DateConditionType type)
	{
		m_date = value;
		m_type = type;
	}

	void TaskList::DateCondition::SetCondition(const Task::DateStr& value, const DateConditionType type)
	{
		SetCondition(Task::Date::ConvertToDate(value), type);
	}

	const Task::Date& TaskList::DateCondition::GetDate() const
	{
		return (m_date);
	}

	const TaskList::DateConditionType& TaskList::DateCondition::GetDateConditionType() const
	{
		return(m_type);
	}

	const bool TaskList::DateCondition::Compare(const Task::Date& date) const
	{
		switch (GetDateConditionType())
		{
		case DateConditionType::Equals:		return (date == m_date);
		case DateConditionType::Greater:	return (date > m_date);
		case DateConditionType::GreaterEq:	return (date >= m_date);
		case DateConditionType::Less:		return (date < m_date);
		case DateConditionType::LessEq:		return (date <= m_date);
		default: throw std::logic_error("TaskList, Compare, DateConditionType is unknown");
		}
	}

	////////////////////////////////////////////////////////////

	const TaskList::ConditionFor TaskList::StatusCondition::GetConditionType() const
	{
		return (TaskList::ConditionFor::Status);
	}

	void TaskList::StatusCondition::SetStatus(const Status& status)
	{
		m_mustbe = status;
	}
	const bool  TaskList::StatusCondition::CheckStatus(const Status& status) const
	{
		return (m_mustbe == status);
	}

	////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////////////////

	void TaskList::Add(const Name& name, const Description& description, const DateString& date, const Category& category)
	{
		m_tasklist.push_back({name, description, date, category});
	}

	void TaskList::Done(const Task::Name& tname)
	{
		operator[](tname).SetStatus(Status::Off);
	}

	void TaskList::Update(const Task::Name& tname)
	{
		Task& task = operator[](tname);
		std::cin >> task;
		task.SetStatus(Status::On);
	}

	void TaskList::TaskList::Delete(const Task::Name& tname)
	{
		m_tasklist.remove_if(
			[&tname](const Task& task)
			{
				return (task.GetName() == tname);
			}
		);
	}

	////////////////////////////////////////////////////////////

	void TaskList::InitTasks(Selected& stasks) const
	{
		for (auto&& task : m_tasklist)
		{
			stasks.push_back(&task);
		}
	}

	static void SelectTasksName(const TaskList::NameCondition& condition, TaskList::Selected& stasks)
	{
		TaskList::Selected::iterator itr = stasks.begin();
		const TaskList::Selected::iterator end = stasks.end();

		for (; itr != end;)
		{
			if (condition.CheckCondition((*itr)->GetName()) == false)
			{
				itr = stasks.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

	static void SelectTasksDescription(const TaskList::DescriptionCondition& condition, TaskList::Selected& stasks)
	{
		TaskList::Selected::iterator itr = stasks.begin();
		const TaskList::Selected::iterator end = stasks.end();

		for (; itr != end;)
		{
			if (condition.CheckCondition((*itr)->GetDescription()) == false)
			{
				itr = stasks.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

	static void SelectTasksCategory(const TaskList::CategoryCondition& condition, TaskList::Selected& stasks)
	{
		TaskList::Selected::iterator itr = stasks.begin();
		const TaskList::Selected::iterator end = stasks.end();

		for (; itr != end;)
		{
			if (condition.CheckCondition((*itr)->GetCategory()) == false)
			{
				itr = stasks.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

	static void SelectTasksStatus(const TaskList::StatusCondition& condition, TaskList::Selected& stasks)
	{
		TaskList::Selected::iterator itr = stasks.begin();
		const TaskList::Selected::iterator end = stasks.end();

		for (; itr != end;)
		{
			if (condition.CheckStatus((*itr)->GetStatus()) == false)
			{
				itr = stasks.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

	static void SelectTasksDate(const TaskList::DateCondition& condition, TaskList::Selected& stasks)
	{
		TaskList::Selected::iterator itr = stasks.begin();
		const TaskList::Selected::iterator end = stasks.end();

		for (; itr != end;)
		{
			if (condition.Compare((*itr)->GetDate()) == false)
			{
				itr = stasks.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

	void TaskList::FilterTasks(const TaskList::QueryForSelect& query, TaskList::Selected& stasks) const
	{
		for (auto&& condition : query)
		{
			switch (condition->GetConditionType())
			{
			case TaskList::ConditionFor::Name:			SelectTasksName(dynamic_cast<TaskList::NameCondition&>			(*condition), stasks); break;
			case TaskList::ConditionFor::Description:	SelectTasksDescription(dynamic_cast<TaskList::DescriptionCondition&>	(*condition), stasks); break;
			case TaskList::ConditionFor::Category:		SelectTasksCategory(dynamic_cast<TaskList::CategoryCondition&>		(*condition), stasks); break;
			case TaskList::ConditionFor::Status:		SelectTasksStatus(dynamic_cast<TaskList::StatusCondition&>		(*condition), stasks); break;
			case TaskList::ConditionFor::Date:			SelectTasksDate(dynamic_cast<TaskList::DateCondition&>			(*condition), stasks); break;
			default: throw std::logic_error("TaskList.cpp, FilterTasks, FilterTasks, ConditionType is incorrect");
			}
		}
	}

	void TaskList::PrintSelectedTasks(const Selected& stasks) const
	{
		if (stasks.empty())
		{
			std::cout << "selected tasks is empty" << std::endl;
		}

		for (auto&& task : stasks)
		{
			std::cout << *task << std::endl;
		}
		std::cout << std::endl;
	}

	void TaskList::Select(const QueryForSelect& query) const
	{
		Selected stasks;
		InitTasks(stasks);
		FilterTasks(query, stasks);
		PrintSelectedTasks(stasks);
	}
}
