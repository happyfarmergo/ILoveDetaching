#include "Box.h"
#include "BoxManager.h"
#include "GlobalInclude.h"

BoxManager::BoxManager()
:
m_startBox(NULL), 
m_endBox(NULL), 
m_isMoving(true), 
m_isTouchEnable(true),
m_isNeedToFill(false),
fourDetachRow(true)
{
}

BoxManager::~BoxManager()
{
	delete []m_boxMatrix;
	delete m_startBox;
	delete m_endBox;
}

bool BoxManager::init()
{
	mapOrigion.x = (SCREEN_WIDTH - BOX_SIZE * COLUMN - BOADER_SIZE * (COLUMN - 1)) / 2;
	mapOrigion.y = (SCREEN_HEIGHT - BOX_SIZE * ROW - BOADER_SIZE * (ROW - 1)) / 2;

	initBox();
	scheduleUpdate();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(BoxManager::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(BoxManager::onTouchMoved, this);
	m_listener = listener;
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void BoxManager::initBox()
{
	for (int i = 0; i < ROW; ++i)
	for (int j = 0; j < COLUMN; ++j)
		createBox(i, j);
}

void BoxManager::createBox(int row, int col)
{
	Box *box = Box::createWithPos(row, col);
	this->addChild(box);
	m_boxMatrix[row][col] = box;
	//盒子降落动画
	Point desPos = this->getPointOfBox(row, col);
	Point startPos = Point(desPos.x, desPos.y + SCREEN_HEIGHT / 2);
	box->setPosition(startPos);
	float speed = startPos.y / (1.5 * SCREEN_HEIGHT);
	auto move = MoveTo::create(speed, desPos);
	box->runAction(move);
}

Point BoxManager::getPointOfBox(int row, int col)
{
	float x = mapOrigion.x + (BOX_SIZE + BOADER_SIZE)*col + BOX_SIZE / 2;
	float y = mapOrigion.y + (BOX_SIZE + BOADER_SIZE)*row + BOX_SIZE / 2;
	return Point(x, y);
}

void BoxManager::update(float dt)
{
	if (m_isMoving)
	{
		m_isMoving = false;
		for (int i = 0; i < ROW && (!m_isMoving); ++i)
		for (int j = 0; j < COLUMN; ++j)
		{
			Box *box = m_boxMatrix[i][j];
			if (box&&box->getNumberOfRunningActions() > 0)
			{
				m_isMoving = true;
				break;
			}
		}
	}

	m_isTouchEnable = !m_isMoving;

	if (!m_isMoving)
	{
		if (m_isNeedToFill)
		{
			fillBox();
			m_isNeedToFill = false;
			if (noBoxToDetach())
			{
				for (int i = 0; i < ROW; ++i)
				for (int j = 0; j < COLUMN; ++j)
					m_boxMatrix[i][j]->setIsNeedRemove(true);
				removeBox();
			}
		}
		else
		{
			checkAndRemoveBox();
		}
	}
}

bool BoxManager::noBoxToDetach()
{
	for (int r = 0; r < ROW; ++r)	
		for (int c = 0; c < COLUMN; ++c)	
		{
			if (m_boxMatrix[r][c]->getIndex() == find(r, c - 1, r - 1, c + 1, r + 1, c + 1))
				return false;
			if (m_boxMatrix[r][c]->getIndex() == find(r, c + 1, r - 1, c - 1, r + 1, c - 1))
				return false;
			if (m_boxMatrix[r][c]->getIndex() == find(r - 1, c, r + 1, c - 1, r + 1, c + 1))
				return false;
			if (m_boxMatrix[r][c]->getIndex() == find(r + 1, c, r - 1, c - 1, r - 1, c + 1))
				return false;
			if (m_boxMatrix[r][c]->getBoxType() == en_Eat_Mode)
				return false;
		}
	return true;
}

int BoxManager::find(int r1, int c1, int r2, int c2, int r3, int c3)
{
	bool isr1 = false, isr2 = false, isr3 = false;

	if (r1 >= 0 && r1 < ROW && c1 >= 0 && c1 < COLUMN)	isr1 = true;
	if (r2 >= 0 && r2 < ROW && c2 >= 0 && c2 < COLUMN)  isr2 = true;
	if (r3 >= 0 && r3 < ROW && c3 >= 0 && c3 < COLUMN)	isr3 = true;

	if (isr1)	
	{
		if (isr2 && m_boxMatrix[r1][c1]->getIndex() == m_boxMatrix[r2][c2]->getIndex())
			return m_boxMatrix[r1][c1]->getIndex();

		if (isr3 && m_boxMatrix[r1][c1]->getIndex() == m_boxMatrix[r3][c3]->getIndex())
			return m_boxMatrix[r1][c1]->getIndex();
	}

	if (isr2 && isr3 && m_boxMatrix[r2][c2]->getIndex() == m_boxMatrix[r3][c3]->getIndex())
		return m_boxMatrix[r2][c2]->getIndex();

	return -1;
}

void BoxManager::checkAndRemoveBox()
{
	Box *box = NULL;
	for (int i = 0; i < ROW; ++i)
	for (int j = 0; j < COLUMN; ++j)
	{
		box = m_boxMatrix[i][j];
		if (!box)
			continue;
		box->setIgnoreCheck(false);
	}

	for (int i = 0; i < ROW; ++i)
	for (int j = 0; j < COLUMN; ++j)
	{
		box = m_boxMatrix[i][j];
		if (!box || box->getIsNeedRemove() || box->getIgnoreCheck())
			continue;

		std::list<Box *>rowChainList, colChainList;
		searchOneRow(box, rowChainList);
		searchOneCol(box, colChainList);

		//十字消除
		if (colChainList.size() == 3 && rowChainList.size() == 3)
		{
			for (auto beg1 = rowChainList.begin(); beg1 != rowChainList.end(); ++beg1)
			{
				box = (Box *)(*beg1);
				if (!box)
					continue;
				if (beg1 == rowChainList.begin())
				{
					box->setIsNeedRemove(false);
					box->setIgnoreCheck(true);
					box->setDisplayMode(en_Triangle_Mode);
					continue;
				}
				markRemove(box);
			}

			for (auto beg2 = colChainList.begin(); beg2 != colChainList.end(); ++beg2)
			{
				box = (Box *)(*beg2);
				if (!box)
					continue;
				if (beg2 == colChainList.begin())
					continue;
				markRemove(box);
			}
			continue;
		}
	}

	//三消、四消、五消
	for (int i = 0; i < ROW; ++i)
	for (int j = 0; j < COLUMN; ++j)
	{
		box = m_boxMatrix[i][j];
		if (!box || box->getIsNeedRemove()||box->getIgnoreCheck())
			continue;

		std::list<Box *>rowChainList,colChainList;
		searchOneRow(box, rowChainList);
		searchOneCol(box, colChainList);

		std::list<Box *>&longerList = colChainList.size() > rowChainList.size() ? colChainList : rowChainList;
		if (longerList.size() < 3)
			continue;
		fourDetachRow = colChainList.size() > rowChainList.size() ? false : true;
		
		auto beg = longerList.begin();
		auto end = longerList.end();
		bool isNational = true;
		for (; beg != end; ++beg)
		{
			box = (Box *)(*beg);
			if (!box)
				continue;
			if (box == m_startBox||box == m_endBox)
			{
				if (longerList.size() >= 4)
				{
					isNational = false;
					box->setIsNeedRemove(false);
					box->setIgnoreCheck(true);
					box->setDisplayMode(longerList.size() == 4 ? 
						(fourDetachRow ? en_Vertical_Mode : en_Horizontal_Mode) : en_Eat_Mode);
					if (longerList.size() > 4)
						box->setIndex(-1);
					continue;
				}
			}
			markRemove(box);
		}
		if (isNational)
		{
			if (longerList.size() >= 4)
			{
				box->setIsNeedRemove(false);
				box->setIgnoreCheck(true);
				box->setDisplayMode(longerList.size() == 4 ?
					(fourDetachRow ? en_Vertical_Mode : en_Horizontal_Mode) : en_Eat_Mode);
				if (longerList.size() > 4)
					box->setIndex(-1);
			}
		}
	}
	removeBox();
}

void BoxManager::markRemove(Box *box)
{
	if (box->getIsNeedRemove() || box->getIgnoreCheck())
		return;

	box->setIsNeedRemove(true);
	Box *temBox = NULL;
	if (box->getBoxType() == en_Horizontal_Mode)
	{
		for (int i = 0; i < COLUMN; ++i)
		{
			temBox = m_boxMatrix[box->getRow()][i];
			if (!temBox || box == temBox)
				continue;
			if (temBox->getBoxType() == en_Normal_Mode)
				temBox->setIsNeedRemove(true);
			else
				markRemove(temBox);
		}
	}
	else if (box->getBoxType() == en_Vertical_Mode)
	{
		for (int i = 0; i < ROW; ++i)
		{
			temBox = m_boxMatrix[i][box->getCol()];
			if (!temBox || box == temBox)
				continue;
			if (temBox->getBoxType() == en_Normal_Mode)
				temBox->setIsNeedRemove(true);
			else
				markRemove(temBox);
		}
	}
	else if (box->getBoxType() == en_Triangle_Mode)
	{
		int left = box->getCol() - 2, right = box->getCol() + 2;
		int up = box->getRow() + 2, down = box->getRow() - 2;
		left = left >= 0 ? left : 0;
		right = (right >= COLUMN) ? COLUMN - 1 : right;
		up = (up >= ROW) ? ROW - 1 : up;
		down = down < 0 ? 0 : down;
		for (int i = down; i < up;++i)
		for (int j = left; j < right; ++j)
		{
			temBox = m_boxMatrix[i][j];
			if (!temBox || box == temBox)
				continue;
			temBox->setIsNeedRemove(true);
		}
	}
}

void BoxManager::removeBox()
{
	Box *box = NULL;
	m_isMoving = true;
	for (int i = 0; i < ROW; ++i)
	for (int j = 0; j < COLUMN; ++j)
	{
		box = m_boxMatrix[i][j];
		if (!box)
			continue;
		if (box->getIsNeedRemove())
		{
			m_isNeedToFill = true;

			if (box->getBoxType() == en_Horizontal_Mode)
			{
				explodeOneRow(box->getPosition());
			}
			else if (box->getBoxType() == en_Vertical_Mode)
			{
				explodeOneCol(box->getPosition());
			}
			else if (box->getBoxType() == en_Triangle_Mode)
			{
				explodeTriangle(box->getPosition());
			}
			explodeBox(box);
		}
	}
}

void BoxManager::explodeBox(Box *box)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
		SimpleAudioEngine::getInstance()->playEffect("Music/music_explode.wav", false);

	box->runAction(ScaleTo::create(0.2f, 0.0));

	Sprite *circle = Sprite::create("circle.png");
	this->addChild(circle, 18);
	circle->setPosition(box->getPosition());
	circle->setScale(0.0);
	box->runAction(Sequence::create(ScaleTo::create(0.5f, 1.0,1.0), 
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, circle)), NULL));

	auto particleStars = ParticleSystemQuad::create("stars.plist");
	particleStars->setAutoRemoveOnFinish(true);
	particleStars->setBlendAdditive(false);
	particleStars->setPosition(box->getPosition());
	double scale = 0.5;
	particleStars->setScale(scale);
	particleStars->setColor(Color3B::RED);
	this->addChild(particleStars, 20);

	m_boxMatrix[box->getRow()][box->getCol()] = NULL;
	box->removeFromParent();
}

void BoxManager::explodeOneRow(Point point)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
		SimpleAudioEngine::getInstance()->playEffect("Music/music_explodeOne.wav", false);

	double scaleX = 4;
	double scaleY = 0.7;
	double time = 0.3;
	Point startPosition = point;
	float speed = 0.6f;

	auto colorSpriteRight = Sprite::create("colorHRight.png");
	this->addChild(colorSpriteRight, 10);
	Point endPosition1 = Point(point.x - SCREEN_WIDTH, point.y);
	colorSpriteRight->setPosition(startPosition);
	colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
		NULL));

	auto colorSpriteLeft = Sprite::create("colorHLeft.png");
	addChild(colorSpriteLeft, 10);
	Point endPosition2 = Point(point.x + SCREEN_WIDTH, point.y);
	colorSpriteLeft->setPosition(startPosition);
	colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition2),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
		NULL));
}

void BoxManager::explodeOneCol(Point point)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
		SimpleAudioEngine::getInstance()->playEffect("Music/music_explodeOne.wav", false);

	double scaleY = 4;
	double scaleX = 0.7;
	double time = 0.3;
	Point startPosition = point;
	float speed = 0.6f;

	auto colorSpriteDown = Sprite::create("colorVDown.png");
	addChild(colorSpriteDown, 10);
	Point endPosition1 = Point(point.x, point.y - SCREEN_HEIGHT);
	colorSpriteDown->setPosition(startPosition);
	colorSpriteDown->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteDown)),
		NULL));

	auto colorSpriteUp = Sprite::create("colorVUp.png");
	addChild(colorSpriteUp, 10);
	Point endPosition2 = Point(point.x, point.y + SCREEN_HEIGHT);
	colorSpriteUp->setPosition(startPosition);
	colorSpriteUp->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition2),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteUp)),
		NULL));

}

void BoxManager::explodeSpecialBox(Box *box, int index)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
		SimpleAudioEngine::getInstance()->playEffect("Music/music_explodeOne.wav", false);
	box->setScale(2.0);
	Box *temBox = NULL;
	Box *pointBox = m_startBox->getBoxType() == en_Eat_Mode ? m_startBox : m_endBox;
	for (int i = 0; i < ROW; ++i)
	for (int j = 0; j < COLUMN; ++j)
	{
		temBox = m_boxMatrix[i][j];
		if (!temBox)
			continue;
		if (temBox->getIndex() == index)
		{
			if (temBox->getBoxType() != en_Normal_Mode)
			{
				markRemove(temBox);
				continue;
			}
			m_boxMatrix[i][j] = NULL;
			float time = 1.0f;
			Action *action = Sequence::create(ScaleTo::create(0.2f,1.5),
				Spawn::create(ScaleTo::create(time, 0.2),
				RotateBy::create(time, 360),
				MoveTo::create(time, pointBox->getPosition()),
				NULL), CallFunc::create(CC_CALLBACK_0(Box::removeFromParent, temBox)), NULL);
			temBox->runAction(action->clone());
		}
	}
	explodeBox(pointBox);
}

void BoxManager::explodeTriangle(Point point)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
		SimpleAudioEngine::getInstance()->playEffect("Music/music_explodeOne.wav", false);
}

void BoxManager::fillBox()
{
	m_isMoving = true;
	int i, j, RowBoxToFill[COLUMN] = { 0 };
	Box *box = NULL;
	int sum = 0;

	for (i = 0; i < COLUMN; ++i)
	{
		int rowBoxsToFill = 0;
		for (j = 0; j < ROW; ++j)
		{
			box = m_boxMatrix[j][i];
			if (!box)
				++rowBoxsToFill;
			else if (rowBoxsToFill > 0)
			{
				int endRow = j - rowBoxsToFill;

				Point startPos = box->getPosition();
				Point endPos = this->getPointOfBox(endRow, i);
				float speed = (startPos.y - endPos.y) / (1.5*SCREEN_HEIGHT);
				auto action = MoveTo::create(speed, endPos);
				box->stopAllActions();
				box->runAction(action);

				m_boxMatrix[endRow][i] = box;
				m_boxMatrix[j][i] = NULL;
				box->setRow(endRow);
			}
		}
		RowBoxToFill[i] = rowBoxsToFill;
		sum += rowBoxsToFill;
	}
	for (i = 0; i < COLUMN; ++i)
	for (j = ROW - RowBoxToFill[i]; j < ROW; ++j)
		createBox(j, i);
	NOTIFY->postNotification("ScoreChange", (Ref *)(sum * 30));
}

void BoxManager::searchOneRow(Box *box, std::list<Box *> &chainList)
{
	chainList.push_back(box);
	int nearRow = box->getRow() - 1;
	Box *nearBox = NULL;

	while (nearRow >= 0)
	{
		nearBox = m_boxMatrix[nearRow][box->getCol()];
		if (nearBox&&nearBox->getIndex() == box->getIndex() && !nearBox->getIsNeedRemove())
		{
			chainList.push_back(nearBox);
			--nearRow;
		}
		else
			break;
	}

	nearRow = box->getRow() + 1;
	while (nearRow < ROW)
	{
		nearBox = m_boxMatrix[nearRow][box->getCol()];
		if (nearBox&&nearBox->getIndex() == box->getIndex() && !nearBox->getIsNeedRemove())
		{
			chainList.push_back(nearBox);
			++nearRow;
		}
		else
			break;
	}
}

void BoxManager::searchOneCol(Box *box, std::list<Box *> &chainList)
{
	chainList.push_back(box);
	int nearCol = box->getCol() - 1;
	Box *nearBox = NULL;

	while (nearCol >= 0)
	{
		nearBox = m_boxMatrix[box->getRow()][nearCol];
		if (nearBox&&nearBox->getIndex() == box->getIndex() && !nearBox->getIsNeedRemove())
		{
			chainList.push_back(nearBox);
			--nearCol;
		}
		else
			break;
	}

	nearCol = box->getCol() + 1;
	while (nearCol < COLUMN)
	{
		nearBox = m_boxMatrix[box->getRow()][nearCol];
		if (nearBox&&nearBox->getIndex() == box->getIndex() && !nearBox->getIsNeedRemove())
		{
			chainList.push_back(nearBox);
			++nearCol;
		}
		else
			break;
	}
}

bool BoxManager::onTouchBegan(Touch *touch, Event *unused)
{
	m_startBox = NULL;
	m_endBox = NULL;
	if (m_isTouchEnable)
	{
		auto loc = touch->getLocation();
		m_startBox = this->getBoxOfPoint(&loc);
	}
	return m_isTouchEnable;
}

Box *BoxManager::getBoxOfPoint(Point *point)
{
	Box *box = NULL;
	Rect bRec(0, 0, 0, 0);
	Size sz;
	sz.height = BOX_SIZE;
	sz.width = BOX_SIZE;
	for (int i = 0; i < ROW; ++i)
	for (int j = 0; j < COLUMN; ++j)
	{
		box = m_boxMatrix[i][j];
		if (!box)
			continue;
		bRec.origin.x = box->getPosition().x - BOX_SIZE / 2;
		bRec.origin.y = box->getPosition().y - BOX_SIZE / 2;
		bRec.size = sz;
		if (bRec.containsPoint(*point))
			return box;
	}
	return NULL;
}

void BoxManager::onTouchMoved(Touch *touch, Event *unused)
{
	if (!m_isTouchEnable || !m_startBox)
	{
		m_startBox = false;
		m_endBox = false;
		return;
	}

	auto location = touch->getLocation();
	int row = m_startBox->getRow();
	int column = m_startBox->getCol();

	Rect upRect(m_startBox->getPosition().x - BOX_SIZE / 2, m_startBox->getPosition().y + BOX_SIZE / 2, BOX_SIZE, BOX_SIZE);
	if (upRect.containsPoint(location))
	{
		++row;
		if (row < ROW)
			m_endBox = m_boxMatrix[row][column];
		swapBox();
		return;
	}

	Rect downRect(m_startBox->getPosition().x - BOX_SIZE / 2, m_startBox->getPosition().y - 3 * BOX_SIZE / 2, BOX_SIZE, BOX_SIZE);
	if (downRect.containsPoint(location))
	{
		--row;
		if (row >= 0)
			m_endBox = m_boxMatrix[row][column];
		swapBox();
		return;
	}

	Rect leftRect(m_startBox->getPosition().x - 3 * BOX_SIZE / 2, m_startBox->getPosition().y - BOX_SIZE / 2, BOX_SIZE, BOX_SIZE);
	if (leftRect.containsPoint(location))
	{
		--column;
		if (column >= 0)
			m_endBox = m_boxMatrix[row][column];
		swapBox();
		return;
	}

	Rect rightRect(m_startBox->getPosition().x + BOX_SIZE / 2, m_startBox->getPosition().y - BOX_SIZE / 2, BOX_SIZE, BOX_SIZE);
	if (rightRect.containsPoint(location))
	{
		++column;
		if (column < COLUMN)
			m_endBox = m_boxMatrix[row][column];
		swapBox();
		return;
	}
}

void BoxManager::swapBox()
{
	if (!m_startBox || !m_endBox)
		return;

	m_isMoving = true;
	m_isTouchEnable = false;

	Point startPos = m_startBox->getPosition();
	Point endPos = m_endBox->getPosition();

	double time = 0.2;

	swap();

	if (m_startBox->getBoxType() == en_Eat_Mode || m_endBox->getBoxType() == en_Eat_Mode)
	{
		m_startBox->runAction(MoveTo::create(time, endPos));
		m_endBox->runAction(MoveTo::create(time, startPos));
		if (m_startBox->getBoxType() == en_Eat_Mode)
			explodeSpecialBox(m_startBox,m_endBox->getIndex());
		else
			explodeSpecialBox(m_endBox, m_startBox->getIndex());
		m_isNeedToFill = true;
		return;
	}

	if ((m_startBox->getBoxType() == en_Vertical_Mode
		||m_startBox->getBoxType()==en_Horizontal_Mode)
		&&(m_endBox->getBoxType()==en_Vertical_Mode
		|| m_endBox->getBoxType() == en_Horizontal_Mode))
	{
		markRemove(m_startBox);
		markRemove(m_endBox);
		m_startBox->runAction(MoveTo::create(time, endPos));
		m_endBox->runAction(MoveTo::create(time, startPos));
		return;
	}

	std::list<Box *>colChainListOfFirst;
	this->searchOneCol(m_startBox, colChainListOfFirst);
	std::list<Box *>rowChainListOfFirst;
	this->searchOneRow(m_startBox, rowChainListOfFirst);
	std::list<Box *>colChainListOfSecond;
	this->searchOneCol(m_endBox, colChainListOfSecond);
	std::list<Box *>rowChainListOfSecond;
	this->searchOneRow(m_endBox, rowChainListOfSecond);

	if (colChainListOfFirst.size() >= 3
		|| rowChainListOfFirst.size() >= 3
		|| colChainListOfSecond.size() >= 3
		|| rowChainListOfSecond.size() >= 3)
	{
		m_startBox->runAction(MoveTo::create(time, endPos));
		m_endBox->runAction(MoveTo::create(time, startPos));
		return;
	}

	swap();

	m_startBox->runAction(Sequence::create(MoveTo::create(time, endPos), MoveTo::create(time, startPos), NULL));
	m_endBox->runAction(Sequence::create(MoveTo::create(time, startPos), MoveTo::create(time, endPos), NULL));
}

void BoxManager::swap()
{
	m_boxMatrix[m_startBox->getRow()][m_startBox->getCol()] = m_endBox;
	m_boxMatrix[m_endBox->getRow()][m_endBox->getCol()] = m_startBox;
	int row = m_startBox->getRow();
	int col = m_startBox->getCol();
	m_startBox->setRow(m_endBox->getRow());
	m_startBox->setCol(m_endBox->getCol());
	m_endBox->setRow(row);
	m_endBox->setCol(col);
}