#pragma once
#include<string>
#include<iostream>
#include<vector>

#define CURL_STATICLIB
#include "curl/curl.h"

#ifdef _DEBUG
#	pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#	pragma comment(lib, "curl/libcurl_a.lib")
#endif

class Leaderboard {
private:
	const std::string url = "http://dreamlo.com/lb/";
	const std::string publicKey = "5c1c34f1b6398000e0adcfd4";
	const std::string privateKey = "O-Sgo-9-Z0GmiQQsNiaizAMzThjnew5EOPIcroY3rx3Q";

	std::vector<int> userScores;

public:
	Leaderboard();
	~Leaderboard();


	void PostScore(std::string username, int score);
	std::string GetScores();
	std::vector<std::string> GetFormattedScores();
	std::vector<int> GetUserScores();

	std::vector<std::string> SplitString(char c, std::string s);
};