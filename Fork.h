#include<string>
class Fork
{
	

bool status; //0-wolny widelec, 1-zajety widelec
	
public:

	int id;
	std::mutex mutex;
	void setStatus(bool status);
	
	std::string getStatus();
};
