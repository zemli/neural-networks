#include "header.h"
#include <time.h>

std::string get_date_string() {
	using namespace std;
	using namespace std::chrono;
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	auto ttime_t = system_clock::to_time_t(tp);
	auto tp_sec = system_clock::from_time_t(ttime_t);
	milliseconds ms = duration_cast<milliseconds>(tp - tp_sec);

	std::tm * ttm = localtime(&ttime_t);

	char date_time_format[] = "%Y.%m.%d-%H.%M.%S";

	char time_str[] = "yyyy.mm.dd.HH-MM.SS.fff";

	strftime(time_str, strlen(time_str), date_time_format, ttm);

	string result(time_str);
	result.append(".");
	result.append(to_string(ms.count()));

	return result;
}