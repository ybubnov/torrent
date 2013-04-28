#include "RandomStringGenerator.h"
#include <time.h>

RandomStringGenerator::RandomStringGenerator(){
	std::srand((unsigned int)time(0));
}

std::string RandomStringGenerator::ascii_string(int length){
	std::stringstream data;
	std::string result;
	char next;

	for(int i = 0; i < length; i++){
		
		do{
			next = std::rand() % 256;
		}while(!((next >= '0' && next <= '9' ) || (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z')));

		data << next;
	}

	result = std::string(data.str());
	std::transform(result.begin(), result.end(), result.begin(), toupper);

	return result;
}



std::vector<char> RandomStringGenerator::random_string(int length){
	std::vector<char> result;

	for(int i = 0; i < length; i++){
		result.push_back(std::rand() % 25);
	}

	return result;
}
