#include"Leaderboard.h"
#include<sstream>

Leaderboard::Leaderboard() {

}

Leaderboard::~Leaderboard() {


}

//https://curl.haxx.se/libcurl/c/htmltitle.html
static int WriteCallback(char *data, size_t size, size_t nmemb, std::string *writerData) {
	if (writerData == NULL)
		return 0;

	writerData->append(data, size*nmemb);

	return size * nmemb;
}

//Posts score to dreamlo service using HTTP GET request
void Leaderboard::PostScore(std::string username, int score) {
	std::string content;
	std::string finalURL = url + privateKey + "/add/" + username + "/" + std::to_string(score);

	curl_global_init(CURL_GLOBAL_ALL);	//Set up libcurl environment. CURL_GLOBAL_ALL initialises everything possible.
	CURL* curl = nullptr;
	curl = curl_easy_init();			//Create curl easy object
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, finalURL.c_str());		//Set curl URL option to our private leaderboard URL

		curl_easy_perform(curl);		//Perform request

		curl_easy_cleanup(curl);		//Must be called if curl_easy_init() is called once everything is finished.
	}
	curl_global_cleanup();				//Must be called if curl_global_init() is called once finished using libcurl.
}

//Returns the scores.
std::string Leaderboard::GetScores() {
	std::string content;
	std::string finalURL = url + publicKey + "/pipe";

	curl_global_init(CURL_GLOBAL_ALL);	//Set up libcurl environment. CURL_GLOBAL_ALL initialises everything possible.
	CURL* curl = nullptr;
	curl = curl_easy_init();			//Create curl easy object

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, finalURL.c_str());			//Set curl URL option to our public leaderboard URL
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);	//Gives curl the writer function
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);			//Tells curl where to write data

		curl_easy_perform(curl);		//Perform request

		curl_easy_cleanup(curl);		//Must be called if curl_easy_init() is called once everything is finished.
	}
	curl_global_cleanup();				//Must be called if curl_global_init() is called once finished using libcurl.

	return content;
}

std::vector<std::string> Leaderboard::GetFormattedScores() {
	std::vector<std::string> formattedScores;	//Vector which is returned
	std::stringstream finalScores;				//Final string that will be added to vector
	std::vector<std::string> scoresDataList = SplitString('\n', GetScores());	//Get all scores data, each entry split into vector

	userScores.clear();

	for (int i = 0; i < (scoresDataList.size() < 20 ? 20 : scoresDataList.size()); i++) {
		//If we still have scores to process
		if (i < scoresDataList.size()) {
			std::vector<std::string> entryData = SplitString('|', scoresDataList.at(i));

			if(i < 20)
				finalScores << i + 1 << ". " << entryData.at(0) << " (" << entryData.at(1) << ")" << std::endl << std::endl;

			userScores.push_back(std::stoi(entryData.at(1)));		//Add score to vector, used for determining where new scores will be
		} else {	//If we ran out of scores/less than 20.
			finalScores << i + 1 << ". -" << std::endl << std::endl;
		}

		if (i == 9) {
			formattedScores.push_back(finalScores.str());
			finalScores.str("");
		}
	}

	formattedScores.push_back(finalScores.str());

	return formattedScores;
}

std::vector<int> Leaderboard::GetUserScores() {
	return userScores;
}

std::vector<std::string> Leaderboard::SplitString(char c, std::string s) {
	std::vector<std::string> finalStringVec;
	int prevCharCount = 0;

	for (int i = 0; i < strlen(s.c_str()); i++) {
		if (s.at(i) == c) {
			finalStringVec.push_back(s.substr(0 + prevCharCount + (prevCharCount == 0 ? 0 : 1), i - (prevCharCount + (prevCharCount == 0 ? 0 : 1))));	//Add i characters of 's' to vector.
			prevCharCount = i;
		}
	}

	return finalStringVec;
}


