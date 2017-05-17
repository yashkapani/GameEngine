#include "UserOutput.h"

namespace eae6320
{
	namespace UserOutput
	{
		void Print(std::string iStr)
		{

			OutputDebugString(iStr.c_str());
		}
	}
}