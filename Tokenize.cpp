#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

#include <string>
#include <vector>
#include <algorithm>

#ifdef WIN32
#define string std::string
#define vector std::vector
#endif

vector<string>
GetTokens(string theString, char separator)
{
	vector<string> foundStrings;
	
	string::iterator beginPos = theString.begin();
	string::iterator endPos;

	while (beginPos != theString.end())
	{
		endPos = std::find(beginPos,theString.end(),separator);
		string foundString(beginPos,endPos);
		foundStrings.push_back(foundString);
		if (endPos == theString.end())
			break;
		beginPos = ++endPos;
	}
	return foundStrings;
}
