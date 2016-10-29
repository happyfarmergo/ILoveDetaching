#ifndef _TOLLGATEDATALAYER_H_
#define _TOLLGATEDATALAYER_H_

#include "cocos2d.h"
USING_NS_CC;

class TollgateDataLayer :public Layer
{
public:
	CREATE_FUNC(TollgateDataLayer);
	virtual bool init();
	virtual void cleanup();
	void recvRefreshScore(Ref *pData);
	int getScore()const;
private:
	int m_score = 0;
};

#endif