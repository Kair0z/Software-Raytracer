#pragma once
#include <string>
#include <regex>
#include <vector>
#include <iostream>

namespace Utils
{
	std::vector<std::string> GetCaptureGroups(const std::string& input, const std::regex& regex)
	{
		std::vector<std::string> result;

		if (regex_match(input, regex))
		{
			std::smatch allMatches;

			if (regex_search(input, allMatches, regex))
			{
				for (int i{}; i < int(allMatches.size()); ++i)
				{
					result.push_back(allMatches[i]);
				}
			}
		}

		return result;
	}
}
