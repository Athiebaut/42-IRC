#include "../incs/classes/Bot.hpp"

size_t Bot::countCommand(std::string str)
{
	size_t size = 0;

	for (size_t pos = str.find("!joke"); pos != std::string::npos; size++)
		pos = str.find("!joke", pos + 1);
	for (size_t pos = str.find("!age"); pos != std::string::npos; size++)
		pos = str.find("!age", pos + 1);
	return size;
}

void Bot::getCommand(std::string recived, std::string &nick , std::string &command, int mode)
{
	if (mode == 1)
	{
		if (recived.find('#') == std::string::npos)
		{
			if(recived[0] == ':')
				recived.erase(recived.begin());
			size_t i = 0;
			size_t pos = 0;
			while (recived[i] != ' ')
				i++;
			nick = recived.substr(0, i);
			if (recived.find("!joke") != std::string::npos)
			{
				pos = recived.find("!joke");
				if (pos != std::string::npos && (recived[pos - 1] == ':' || recived[pos - 1] == ' '))
				{
					i = pos + 1;
					while (i < recived.size() && isalpha(recived[i]) != 0)
						i++;
					if (i < recived.size() && (recived[i] != ' '))
						return ;
					if (pos != std::string::npos && i != pos)
						command = recived.substr(pos, (i - pos));
				}
			}
			else if (recived.find("!age") != std::string::npos && pos != std::string::npos)
			{
				pos = recived.find("!age");
				if (pos != std::string::npos && (recived[pos - 1] == ':' || recived[pos - 1] == ' ') && (recived[pos + 4] == ' ' || pos + 4 == recived.size()))	
					command = recived.substr(pos);
			}
			if (!command.empty() && command[0] == ':')
				command.erase(command.begin());
		}
		else if (recived.find('#') != std::string::npos)
		{
			size_t i;
			size_t pos;
			i = recived.find('#');
			pos = i + 1;
			while (recived[pos] != ' ')
				pos++;
			pos -= i;
			nick = recived.substr(i, pos);
			if (recived.find("!joke") != std::string::npos)
			{
				pos = recived.find("!joke");
				if (pos != std::string::npos && (recived[pos - 1] == ':' || recived[pos - 1] == ' '))
				{
					i = pos + 1;
					while (i < recived.size() && isalpha(recived[i]) != 0)
						i++;
					if (i < recived.size() && (recived[i] != ' '))
						return ;
					if (pos != std::string::npos && i != pos)
						command = recived.substr(pos, (i - pos));
				}
			}
			else if (recived.find("!age") != std::string::npos && pos != std::string::npos)
			{
				pos = recived.find("!age");
				if (pos != std::string::npos && (recived[pos - 1] == ':' || recived[pos - 1] == ' ') && (recived[pos + 4] == ' ' || pos + 4 == recived.size()))	
					command = recived.substr(pos);
			}
			if (!command.empty() && command[0] == ':')
				command.erase(command.begin());
		}
	}
	else if (mode == 0)
	{
		if (recived.find("invited") != std::string::npos)
		{
			size_t count = 0;
			command = "invited";
			size_t pos = recived.find_first_of("#");
			count = pos;
			while (isprint(recived[count]))
				count++;
			count -= pos;
			nick = recived.substr(pos, count);
		}
	}
}

void Bot::sendMessage(std::string message, int fd)
{
	std::string formatedMessage = MessageHandler::ircFormat(message);
	if(send(fd, formatedMessage.c_str(), formatedMessage.size(), 0) == -1)
		std::cerr << "Send failed." << std::endl;
}

void Bot::send_privmsg(std::string message, std::string UserNick, int sock)
{
	std::string msg = "PRIVMSG " + UserNick + " :" + message + "\r\n";
	if (send(sock, msg.c_str(), msg.size(),0) == -1)
		std::cerr << "Send failed" << std::endl;
}