#include "TollgateDataLayer.h"
#include "GlobalDefine.h"

bool TollgateDataLayer::init()
{
	if (!Layer::init())
		return false;
	NOTIFY->addObserver(this, callfuncO_selector(TollgateDataLayer::recvRefreshScore), "ScoreChange", NULL);
	return true;
}

void TollgateDataLayer::recvRefreshScore(Ref *pData)
{
	int iScore = int(pData);
	m_score += iScore;
	auto pScore = (Label *)this->getParent()->getChildByTag(LABELSCORE);
	pScore->setString(StringUtils::format("Score: %d", m_score));
}

int TollgateDataLayer::getScore()const
{
	return m_score;
}

void TollgateDataLayer::cleanup()
{
	NOTIFY->removeAllObservers(this);
	Layer::cleanup();
}