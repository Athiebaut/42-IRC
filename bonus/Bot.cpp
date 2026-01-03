#include "../incs/classes/Bot.hpp"

//---------------------------------------------------BOT METHODS---------------------------------------------------//

Bot::Bot() : _Loggedin(false) {}
Bot::~Bot() {}
Bot::Bot(const Bot& other) {*this = other;}
Bot& Bot::operator=(const Bot& other)
{
	if (this != &other)
	{
		this->_quotes = other._quotes;
		this->_Loggedin = other._Loggedin;
		this->_botnick = other._botnick;
	}
	return *this;
}

void	Bot::setNick(std::string nick)
{
	_botnick = nick;
}

//---------------------------------------------------INIT METHODS---------------------------------------------------//

void Bot::init(int sock, std::string nick, std::string user, std::string adr)
{
	std::string welcome;
	ssize_t recivedBytes;
	std::string nickname, command;

	_Loggedin = false;
	char buffer[1024];
	std::vector<std::string> buff;
	std::string botmask = nick + "!" + '~' + user + "@" + adr;
	welcome = MessageHandler::ircWelcomeMessage(user, botmask);
	while(true)
	{
		bzero(buffer, sizeof(buffer));
		recivedBytes = recv(sock, buffer, (sizeof(buffer) - 1), 0);
		if(recivedBytes <= 0)
		{
			perror("Bot :");
			return ;
		}
		if (buffer[0])
			buff.push_back(std::string(buffer));
		while(!buff.empty() && !_Loggedin)
		{
			if (buff.back().find(welcome, 0) == std::string::npos)
				buff.pop_back();
			else
			{
				std::cout << "\nBOT Is Connected!\n" << std::endl;
				_Loggedin = true;
			}
		}
		if (!_Loggedin)
		{
			std::cout << "Error while connecting." << std::endl;
			return ;
		}
		else if (buff.back().find(MessageHandler::ircPing()) != std::string::npos)
		{
			std::stringstream ss;
			ss << MessageHandler::ircPong();
			sendMessage(ss.str(), sock);
		}
		else if(buff.back().find("PRIVMSG") != std::string::npos && _Loggedin)
		{
			if (commandHandler(buff, nickname, command, sock) == 1)
				continue ;
		}
		else if (buff.back().find("NOTICE") != std::string::npos && _Loggedin && buff.back().find("PRIVMSG") == std::string::npos)
		{
			if (getInviteHandler(buff, nickname, command, sock) == 1)
				continue;
		}
	}
}
