
#pragma once

#include "TaskList.hpp"

namespace TeamSoftTask
{

	class IUserCommand
	{
	protected:
		TaskList::Command m_command;

	public:
		virtual ~IUserCommand() = 0;
		virtual TaskList::Command GetCommand() const;
		virtual void SetCommand(const TaskList::Command& command);
	};

	class UCommandName : public IUserCommand
	{
	protected:
		Task::Name m_name;

	public:
		UCommandName(const TaskList::Command&);
		virtual Task::Name GetName() const;
		virtual void SetName(const Task::Name& name);
	};

	///////////////////////////////////////////////////////////////////////

	using Attributes = std::list<std::string>;

	class UCommandAdd : public UCommandName
	{
	protected:
		Attributes m_attributes;

	public:
		UCommandAdd();
		void AddAttribute(const std::string& att);
		Attributes& GetAttributes();
		const Attributes& GetAttributes() const;
	};

	///////////////////////////////////////////////////////////////////////

	class UCommandSelect : public IUserCommand
	{
	protected:
		TaskList::QueryForSelect m_query;
		
	public:
		UCommandSelect();
		void AddCondition(TaskList::Condition&& cond);
		TaskList::QueryForSelect& GetQuery();
		const TaskList::QueryForSelect& GetQuery() const;
	};

	///////////////////////////////////////////////////////////////////////

	class UserListener
	{
	public:
		UserListener() = delete;
		UserListener(std::unique_ptr<TaskList> tasklist);
		virtual ~UserListener();
	public:
		virtual void StartListening();
		virtual std::string InputForStopListening();
		virtual std::string ReadInput();
		virtual const std::unique_ptr<IUserCommand> ParseInput(std::string line);
		virtual void Do(const std::unique_ptr<IUserCommand>& command);
		virtual void StopListening();

	protected:
		virtual void Add	(const UCommandAdd& ucommmand);
		virtual void Done	(const UCommandName& ucommmand);
		virtual void Update	(const UCommandName& ucommmand);
		virtual void Delete	(const UCommandName& ucommmand);
		virtual void Select	(const UCommandSelect& ucommmand);

	protected:
		virtual std::unique_ptr<IUserCommand> ParseAdd		(const std::string& line);
		virtual std::unique_ptr<IUserCommand> ParseDone		(const std::string& line);
		virtual std::unique_ptr<IUserCommand> ParseUpdate	(const std::string& line);
		virtual std::unique_ptr<IUserCommand> ParseDelete	(const std::string& line);
		virtual std::unique_ptr<IUserCommand> ParseSelect	(const std::string& line);

	protected:
		std::unique_ptr<TaskList> m_tasklist;
	};

	class UserListenerLog : public UserListener
	{
	public:
		UserListenerLog() = delete;
		UserListenerLog(std::unique_ptr<TaskList> tasklist);

		virtual void StartListening() override;
		virtual std::string InputForStopListening() override;
		virtual std::string ReadInput() override;
		virtual const std::unique_ptr<IUserCommand> ParseInput(std::string line) override;
		virtual void Do(const std::unique_ptr<IUserCommand>& command) override;
		virtual void StopListening() override;
	};

}
