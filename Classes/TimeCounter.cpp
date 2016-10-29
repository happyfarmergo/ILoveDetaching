#include "TimeCounter.h"

bool TimeCounter::init()
{
	return true;
}

void TimeCounter::update(float dt)
{
	m_time += dt;
}

void TimeCounter::start()
{
	m_time = 0;
	this->scheduleUpdate();
}

float TimeCounter::getCurTime()
{
	return m_time;
}