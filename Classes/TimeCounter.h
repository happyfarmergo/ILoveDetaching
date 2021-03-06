#ifndef _TIMECOUNTER_H_
#define _TIMECOUNTER_H_

#include "cocos2d.h"
USING_NS_CC;

class TimeCounter :public Node
{
public:
	CREATE_FUNC(TimeCounter);
	virtual bool init();
	virtual void update(float dt);
	void start();
	float getCurTime();
private:
	float m_time;
};

#endif