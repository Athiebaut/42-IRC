#include "../incs/classes/Bot.hpp"

std::string Bot::SplitBuff(std::string buff, std::string &date)
{
	std::istringstream stm(buff);
	std::string token;
	stm >> token;
	stm >> date;
	return token;
}

int Bot::ParsAge(std::string age)
{
	std::string year, month, day, age1;
	age1 = age;
	std::stringstream ss(age);

	int flag = 0;
	while (std::getline(ss, age, '-'))
	{
		flag++;
		for (size_t i = 0; i < age.size(); ++i)
		{
			if (!isdigit(age[i]) && age[i] != '-') 
				return 0;
		}
	}
	if (flag != 3) 
		return 0;

	int found = age1.find("-");
	year = age1.substr(0, found);
	age1 = age1.substr(found+1);
	found = age1.find("-");
	month = age1.substr(0, found);
	day = age1.substr(found+1);

	if (std::atoi(year.c_str()) > 2024 || std::atoi(year.c_str()) < 1900)  
		return 0;
	if (std::atoi(year.c_str()) == 2024 && std::atoi(month.c_str()) > 1) 
		return 0;
	if (std::atoi(month.c_str()) > 12 || std::atoi(month.c_str()) < 1 || std::atoi(day.c_str()) > 31 || std::atoi(day.c_str()) < 1 ) 
		return 0;
	if (std::atoi(month.c_str()) == 4 || std::atoi(month.c_str()) == 6 || std::atoi(month.c_str()) == 9 || std::atoi(month.c_str()) == 11)
	{
		if (std::atoi(day.c_str()) > 30) 
			return 0;
	}
	if ((std::atoi(year.c_str()) % 4 == 0 && std::atoi(year.c_str()) % 100 != 0) || std::atoi(year.c_str()) % 400 == 0)
	{
		if (std::atoi(month.c_str()) == 2 && std::atoi(day.c_str()) > 29) 
			return 0;
	}
	else
	{
			if (std::atoi(month.c_str()) == 2 && std::atoi(day.c_str()) > 28) 
				return 0;
	}
	return 1;
}

void Bot::ageCalculator(std::string age, std::string Nickname,int ircsock)
{
	if (!ParsAge(age))
	{
		Bot::send_privmsg("Invalid date format(!age <year-month-day>).", Nickname, ircsock);
		return;
	}

	int year, month, day;
	year = std::atoi(age.substr(0, 4).c_str());
	month = std::atoi(age.substr(5, 2).c_str());
	day = std::atoi(age.substr(8, 2).c_str());

	std::tm birth_date;
	memset(&birth_date, 0, sizeof(birth_date));
	birth_date.tm_year = year - 1900;
	birth_date.tm_mon = month - 1;
	birth_date.tm_mday = day;

	std::time_t birth_time = mktime(&birth_date);
	std::time_t current_time;
	std::time(&current_time);

	double seconds = difftime(current_time, birth_time);
	int years = static_cast<int>(seconds / (365.25 * 24 * 60 * 60));
	int months = static_cast<int>((seconds - years * 365.25 * 24 * 60 * 60) / (30.44 * 24 * 60 * 60));
	int days = static_cast<int>((seconds - years * 365.25 * 24 * 60 * 60 - months * 30.44 * 24 * 60 * 60) / (24 * 60 * 60));
	
	std::stringstream ss;
	ss << "You are : " << years << " years, " << months << " months, " << days << " days old";
	Bot::send_privmsg(ss.str(), Nickname, ircsock);
}