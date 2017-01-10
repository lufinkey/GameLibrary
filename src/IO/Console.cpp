
#include <GameLibrary/IO/Console.hpp>
#include <iostream>

namespace fgl
{
	void Console::write(const String& output)
	{
		std::cout << output;
	}

	void Console::writeLine(const String& output)
	{
		std::cout << output << std::endl;
	}

	void Console::writeError(const String& output)
	{
		std::cerr << output;
	}

	void Console::writeErrorLine(const String& output)
	{
		std::cerr << output << std::endl;
	}
}
