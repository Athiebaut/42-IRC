#include "../incs/classes/Bot.hpp"

int Bot::getInviteHandler(std::vector<std::string> &buff, std::string &nickname, std::string &command, int sock)
{
	size_t pos = buff.back().find_first_of("\r\n");
	if (pos != std::string::npos)
		buff.back() = buff.back().substr(0, pos);
	getCommand(buff.back(), nickname, command, 0);
	if (countCommand(buff.back()) > 1)
	{
		send_privmsg("Too many commands", nickname, sock);
		buff.pop_back();
		command.clear();
		return 1;

	}
	if (command == "invited")
	{
		std::stringstream ss;
		ss << "JOIN " << nickname;
		sendMessage(ss.str(), sock);
		send_privmsg("Hi, if you want, I can give you a !joke or your !age <year-month-day>!! :)", nickname, sock);
		buff.pop_back();
		command.clear();
		return 1;
	}
	return 0;
}

int Bot::commandHandler(std::vector<std::string> &buff, std::string &nickname, std::string &command, int sock)
{
	size_t pos = buff.back().find_first_of("\r\n");
	if (pos != std::string::npos)
		buff.back() = buff.back().substr(0, pos);
	getCommand(buff.back(), nickname, command, 1);
	if(countCommand(buff.back()) > 1)
	{
		send_privmsg("Too many commands", nickname, sock);
		buff.pop_back();
		command.clear();
		return 1;

	}
	if(command.find("!age") != std::string::npos)
	{

		std::string date;
		SplitBuff(command, date);
		ageCalculator(date, nickname, sock);
		buff.pop_back();
		command.clear();
		return 1;
	}
	else if(command == "!joke")
	{
		std::string quotes = getquotes(_quotes, _quotes.size());
		send_privmsg(quotes.c_str(), nickname, sock);
		buff.pop_back();
		command.clear();
		return 1;
	}
	else if (buff.back().find("!") != std::string::npos && command.empty())
	{
		command.clear();
		buff.pop_back();
		return 1;
	}
	return 0;
}