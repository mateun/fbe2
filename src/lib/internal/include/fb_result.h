#pragma once
#include <string>

enum FB_RESULT_TYPE {
	OK,
	ERR
};

struct FBResult {
	FB_RESULT_TYPE resultType;
	std::string errorMsg;
};
