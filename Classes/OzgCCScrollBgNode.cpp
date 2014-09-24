#include "OzgCCScrollBgNode.h"

USING_NS_CC;
using namespace std;

OzgCCScrollBgNode::~OzgCCScrollBgNode()
{
    this->unscheduleUpdate();

	for (int i = 0; i < (int)this->m_bgList.size(); i++)
        CCTextureCache::sharedTextureCache()->removeTextureForKey(this->m_bgList[i].c_str());
    
	CCLOG("ScrollBgNode释放");
}

bool OzgCCScrollBgNode::init(std::vector<std::string> bgList)
{
	if (CCNode::init())
	{
		//只有1个元素的情况下，会出现问题，所以需要再补1个同样的元素上去
		if (bgList.size() == 1)
			bgList.push_back(bgList[0]);

		this->m_bgList = bgList;

		this->m_scrollSpeed = -1.0f; //默认滚动速度，负数为向左，正数为向右

		auto node = CCNode::create();

		if (this->m_scrollSpeed < 0)
		{
			//向左

			for (int i = 0; i < (int)bgList.size(); i++)
			{
				auto bg = CCSprite::create(bgList[i].c_str());
				bg->setAnchorPoint(CCPoint(0, 0));

				if (i == 0)
					bg->setPosition(CCPoint(0, 0));
				else
				{
					auto prevBg = node->getChildByTag(i - 1);
					bg->setPosition(CCPoint(prevBg->getContentSize().width, 0));
				}

				bg->setTag(i);
				node->addChild(bg);
			}
		}
		else if (this->m_scrollSpeed > 0)
		{
			//向右

			for (int i = 0; i < (int)bgList.size(); i++)
			{
				auto bg = CCSprite::create(bgList[i].c_str());
				bg->setAnchorPoint(CCPoint(0, 0));

				if (i == 0)
					bg->setPosition(CCPoint(0, 0));
				else
				{
					auto prevBg = node->getChildByTag(i - 1);
					bg->setPosition(CCPoint(-prevBg->getContentSize().width, 0));
				}

				bg->setTag(i);
				node->addChild(bg);
			}
		}
				
		node->setTag(1);
		this->addChild(node, 1, CCPoint(1, 0), CCPoint(0, 0));

		this->scheduleUpdate();

		return true;
	}
	return false;
}

OzgCCScrollBgNode* OzgCCScrollBgNode::create(std::vector<std::string> bgList)
{
	OzgCCScrollBgNode *obj = new OzgCCScrollBgNode();
	if (obj && obj->init(bgList))
	{
		obj->autorelease();
		return obj;
	}
	CC_SAFE_DELETE(obj);
	return NULL;
}

void OzgCCScrollBgNode::update(float delta)
{
	CCAssert(this->m_scrollSpeed != 0, "速度值不能为0");

	auto x = this->getPositionX() + this->m_scrollSpeed;
	this->setPositionX(x);

	auto node = this->getChildByTag(1);

	if (this->m_scrollSpeed < 0)
	{
		//向左

		for (int i = 0; i < (int)this->m_bgList.size(); i++)
		{
			auto bg = node->getChildByTag(i);

			auto point = node->convertToWorldSpace(bg->getPosition());
			point = this->getParent()->convertToNodeSpace(point);
			if (point.x < -bg->getContentSize().width)
			{
				CCNode *bg2 = NULL;
				if (i + 1 >= this->m_bgList.size())
					bg2 = node->getChildByTag(0);
				else
					bg2 = node->getChildByTag(i + 1);

				bg->setPositionX(bg2->getPositionX() + bg2->getContentSize().width);
			}
		}
	}
	else if (this->m_scrollSpeed > 0)
	{
		//向右

		for (int i = 0; i < (int)this->m_bgList.size(); i++)
		{
			auto bg = node->getChildByTag(i);

			auto point = node->convertToWorldSpace(bg->getPosition());
			point = this->getParent()->convertToNodeSpace(point);
			if (point.x > bg->getContentSize().width)
			{
				CCNode *bg2 = NULL;
				if (i + 1 >= this->m_bgList.size())
					bg2 = node->getChildByTag(0);
				else
					bg2 = node->getChildByTag(i + 1);

				bg->setPositionX(bg2->getPositionX() - bg2->getContentSize().width);
			}
		}
	}

}
