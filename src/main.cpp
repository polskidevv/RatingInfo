#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

/*
	this code is really bad and messy
	if you wanna help fix it then make a pull request with requested changes :)
*/

class $modify(EvilLevelInfoLayer, LevelInfoLayer) {
private:
    int req;
	bool plat;
public:
	// the below code snippet was crafted by acaruso
	cocos2d::CCNode* getChildBySpriteFrameNameRecursive(cocos2d::CCNode* parent, char const* name) {
		return findFirstChildRecursive<cocos2d::CCNode>(parent, [=](auto* spr) {
			return isSpriteFrameName(spr, name);
		});
	}
	// the above code snippet was crafted by acaruso
	
    void info(CCObject *sender) {
        FLAlertLayer::create(
            "Rating Explanation",
            "<co>Ratings</c> indicate the quality of <cl>levels</c>.\nRated: A <co>rated</c> level.\n<cy>Featured:</c> A quality <co>rated</c> level.\n<co>Epic:</c> A high quality <co>rated</c> level.\n<cp>Legendary:</c> A <cp>higher quality</c> <co>rated</c> level.\n<cj>Mythic:</c> An <cj>incredible</c> <co>rated</c> level.\nEvery <cl>level</c> is <co>rated</c> by <co>RobTop</c>.",
            "OK"
        )->show();
    }

    void noRateInfo(CCObject *sender) {
        std::string starsOrMoons = "Stars";
        if (m_fields->plat) { starsOrMoons = "Moons"; }
		std::string content = fmt::format("This <cl>level</c> is <cr>unrated</c>.\n{} requested:<cr> none</c>.", starsOrMoons);
		if(m_fields->req != 0) {
			content = fmt::format("This <cl>level</c> is <cr>unrated</c>.\n{} requested: <cg>{}</c>.", starsOrMoons, m_fields->req);
		}
		FLAlertLayer::create(
			fmt::format("{} Rating", starsOrMoons).c_str(),
			content.c_str(),
			"OK"
		)->show();
    }

	bool init(GJGameLevel* level, bool p1) {
		if (!LevelInfoLayer::init(level, p1)) { return false; }
		auto starIcon = static_cast<CCSprite*>(getChildByID("stars-icon"));
        auto starPos = starIcon->getPosition();

		auto menu = CCMenu::create();
		addChild(menu);
		menu->setPosition(0, 0);

        m_fields->req = level->m_starsRequested;
        m_fields->plat = level->isPlatformer();

		if (level->m_isEpic || level->m_featured || level->m_stars > 0) {
			starIcon->setVisible(false);
			if(m_fields->plat) {
				auto featureIcon = CCSprite::createWithSpriteFrameName("moon_small01_001.png");
				auto featuredCoinBtn = CCMenuItemSpriteExtra::create(featureIcon, this, menu_selector(EvilLevelInfoLayer::info));
				featuredCoinBtn->setPosition(starPos);
				featureIcon->setZOrder(-2);
				menu->addChild(featuredCoinBtn);
			} else {
				auto featureIcon = CCSprite::createWithSpriteFrameName("star_small01_001.png");
				auto featuredCoinBtn = CCMenuItemSpriteExtra::create(featureIcon, this, menu_selector(EvilLevelInfoLayer::info));
				featuredCoinBtn->setPosition(starPos);
				featureIcon->setZOrder(-2);
				menu->addChild(featuredCoinBtn);
			}
		} else {
			if (Mod::get()->getSettingValue<bool>("unrated")) {
				if(level->m_stars == 0) {
					auto infoBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
					auto noRateBtn = CCMenuItemSpriteExtra::create(infoBtnSpr, this, menu_selector(EvilLevelInfoLayer::noRateInfo));
					auto diffSprite = getChildByIDRecursive("difficulty-sprite");
					noRateBtn->setPosition({diffSprite->getPositionX() + 14, diffSprite->getPositionY() + 19});
					menu->addChild(noRateBtn);
					menu->setZOrder(3);
					infoBtnSpr->setScale(0.65);
				}
			}
		}
		return true;
	}
};