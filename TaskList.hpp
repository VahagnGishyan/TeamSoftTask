#pragma once


#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <filesystem>
#include <regex>
#include <cassert>
#include <memory>

////////////////////////////////////////////////////////////

namespace TeamSoftTask
{
	static auto MacroLogFileName = "logging.txt";

	void Logging(const std::string& message);

	class Task
	{
	public:
		using Name = std::string;
		using Description = std::string;
		using DateStr = std::string;
		using Category = std::string;
		enum class Status
		{
			On, 
			Off
		};

	public:
		class Date
		{
		protected:
			// format is year-month-day hour:minute => 2020-12-12 00:00
			unsigned int m_year;	// >= 0, < 10000
			unsigned int m_month;	// >= 0, <= 12
			unsigned int m_day;		// >= 0, <= 31
			unsigned int m_hour;	// >= 0, <= 24
			unsigned int m_minute;	// >= 0, <= 60 

		public:
			static const DateStr ConvertToString(const Date& data);
			static const bool IsStringDataFormat(const DateStr& dataAsString);
			static const Date ConvertToDate(const DateStr& line);

		public:

			friend bool operator==(const Task::Date& lhs, const Task::Date& rhs);
			friend bool operator!=(const Task::Date& lhs, const Task::Date& rhs);
			friend bool operator>(const Task::Date& lhs, const Task::Date& rhs);
			friend bool operator<(const Task::Date& lhs, const Task::Date& rhs);
			friend bool operator>=(const Task::Date& lhs, const Task::Date& rhs);
			friend bool operator<=(const Task::Date& lhs, const Task::Date& rhs);
		};

	public:
		Task();
		Task(const Name& name, const Description& description, const DateStr& date, const Category& category);
		virtual ~Task();

	public:
		virtual void SetName(const Name& name);
		virtual void SetDescription(const Description& description);
		virtual void SetDate(const Date& date);
		virtual void SetCategory(const Category& category);
		virtual void SetStatus(const Status& status);

		virtual const Name& GetName() const&;
		virtual const Description& GetDescription() const&;
		virtual const Date& GetDate() const&;
		virtual const Category& GetCategory() const&;
		virtual const Status& GetStatus() const&;

		virtual const std::string ToString() const;

	public:
		friend std::ostream& operator<<(std::ostream& out, Task& task);
		friend std::istream& operator>>(std::istream& in,  Task& task);

	protected:
		Name		m_name;
		Description m_description;
		Date		m_date;
		Category	m_category;
		Status		m_status;
	};

	class TaskList
	{
	public:
		using Name = Task::Name;
		using Description = Task::Description;
		using DateString = Task::DateStr;
		using Category = Task::Category;
		using Status = Task::Status;
		using Container = std::list<Task>;

		enum class Command
		{
			Add,
			Done,
			Update,
			Delete,
			Select
		};

		enum class ConditionFor
		{
			Name, // ==, like
			Description,
			Date,
			Category,
			Status
		};

	public:
		class ICondition
		{
		public:
			virtual ~ICondition();
			virtual const ConditionFor GetConditionType() const = 0;
		};

		enum class TextConditionType
		{
			Equals, 
			Like
		};

		class TextCondition : public ICondition
		{
		protected:
			std::string m_value;
			TextConditionType m_type;

		public:
			virtual void SetCondition(const std::string& value, const TextConditionType type);
			const TextConditionType& GetTextConditionType();
			const bool CheckCondition(const std::string& category) const;

		protected:
			virtual const std::string& GetValue();
		};

		class NameCondition : public TextCondition
		{
		public:
			virtual const ConditionFor GetConditionType() const override;
		};

		class DescriptionCondition : public TextCondition
		{
		public:
			virtual const ConditionFor GetConditionType() const override;
		};

		class CategoryCondition : public TextCondition
		{
		public:
			virtual const ConditionFor GetConditionType() const override;
		};

		class StatusCondition : public ICondition
		{
		protected:
			Status m_mustbe;
		public:
			virtual const ConditionFor GetConditionType() const override;

			virtual void SetStatus(const Status& status);
			virtual const bool CheckStatus(const Status& status) const;
		};

		enum class DateConditionType
		{
			Equals,		// =
			Greater,	// >
			Less,		// <
			GreaterEq,	// >=
			LessEq,		// <=
		};

		class DateCondition : public ICondition
		{
		protected:
			Task::Date m_date;
			DateConditionType m_type;
		public:
			virtual const ConditionFor GetConditionType() const override;

			virtual void SetCondition(const Task::Date& value, const DateConditionType type);
			virtual void SetCondition(const Task::DateStr& value, const DateConditionType type);
			virtual const Task::Date& GetDate() const;
			virtual const DateConditionType& GetDateConditionType() const;
			virtual const bool Compare(const Task::Date& date) const;
		};

		using Condition = std::unique_ptr<ICondition>;
		using QueryForSelect = std::list<Condition>;
		using Selected = std::list<const Task*>;

	private:
		void InitTasks(Selected& list)  const;

	public:
		virtual Task& operator[](const Name& name);

	public:
		virtual void Add	(const Name& name, const Description& description, const DateString& date, const Category& category);
		virtual void Done	(const Task::Name& tname);
		virtual void Update	(const Task::Name& tname);
		virtual void Delete	(const Task::Name& tname);
		virtual void Select	(const QueryForSelect& query) const;

	protected:
		virtual void FilterTasks(const QueryForSelect& query, Selected& stasks) const;
		virtual void PrintSelectedTasks(const Selected& stasks) const;

	protected:
		Container m_tasklist;
	};
}
