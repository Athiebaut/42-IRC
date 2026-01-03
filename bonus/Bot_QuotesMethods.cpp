#include "../incs/classes/Bot.hpp"

int Bot::getquotes(std::string filename)
{
	std::string line;
	std::ifstream file(filename.c_str());

	if (!file.is_open())
	{
		std::cerr << "Failed to open file" << std::endl;
		return 0;
	}
	while (std::getline(file, line))
		_quotes.push_back(line);
	file.close();
	return 1;
}

std::string Bot::getquotes(std::vector<std::string> &_quotes, int size)
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	return _quotes[std::rand() % size];
}