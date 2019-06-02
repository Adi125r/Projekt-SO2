#include "Fork.h"
#include<string>

Fork::Fork()
{
	this->status=0;
}

void Fork::setStatus(bool status)
{
	this->status=status;
}

std::string Fork::getStatus()
{
	if(this->status==0)
		return "AVAILABLE";
	else
		return"UNAVAILABLE";
}