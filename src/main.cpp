#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

// lord forgive me for this shitty code

class $modify(EvilLevelInfoLayer, LevelInfoLayer) {
private:
    int req;
public:
    void info(CCObject *sender) {
        FLAlertLayer::create(
            "Rating",
            "A <co>rating</c> indicates the quality of a certain <cg>level</c>.\n<cy>Featured:</c> A high quality <cy>star-rated</c> level.\n<co>Epic:</c> A higher quality <cy>star-rated</c> level.\n<cp>Legendary:</c> A very high quality <cy>star-rated</c> level.\n<cj>Mythic:</c> A <cy>star-rated</c> level with <cr>incredible</c> quality.\n<cg>Every level is rated by</c> <co>RobTop</c>.",
            "OK"
        )->show();
    }

    void noRateInfo(CCObject *sender) {
		if(req == 0) {
        	std::string content = "This <cg>level</c> is <cr>unrated</c>.\n<cy>Stars requested:</c><cr> none</c>.";
			FLAlertLayer::create(
				"Rating",
				content.c_str(),
				"OK"
			)->show();
		}
		else {
        	std::string content = "This <cg>level</c> is <cr>unrated</c>.\n<cy>Stars requested:</c><cg> " + std::to_string(req) + "</c>.";
			FLAlertLayer::create(
				"Rating",
				content.c_str(),
				"OK"
			)->show();
		}
    }

	bool init(GJGameLevel* level, bool p1) {
		if (!LevelInfoLayer::init(level, p1))
            return false;

        auto originalIcon = dynamic_cast<CCSprite*>(this->getChildByID("difficulty-sprite"));

        auto zpos = originalIcon->getZOrder();
        auto pos = originalIcon->getPosition();
		auto menu = CCMenu::create();
		this->addChild(menu);
		menu->setPosition(0, 0);

        req = level->m_starsRequested;

		//stupid workaround for the feature button, dont ask
		auto featureIconCoin = CCSprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
		auto btnFeature = CCMenuItemSpriteExtra::create(
			featureIconCoin, this, menu_selector(EvilLevelInfoLayer::info)
		);

		if (level->m_isEpic) {
			auto originalFeatureIcon = dynamic_cast<CCSprite*>(originalIcon->getChildren()->objectAtIndex(0));
			originalFeatureIcon->setOpacity(0);


			if (level->m_isEpic == 1) {
				btnFeature->setVisible(false);
				auto featureIcon = CCSprite::createWithSpriteFrameName("GJ_epicCoin_001.png");
				auto featuredCoinBtn = CCMenuItemSpriteExtra::create(
					featureIcon, this, menu_selector(EvilLevelInfoLayer::info)
				);
				featuredCoinBtn->setPosition(pos);
				featureIcon->setZOrder(-2);
				menu->addChild(featuredCoinBtn);
			}
			if (level->m_isEpic == 2) {
				btnFeature->setVisible(false);
				auto featureIcon = CCSprite::createWithSpriteFrameName("GJ_epicCoin2_001.png");
				auto featuredCoinBtn = CCMenuItemSpriteExtra::create(
					featureIcon, this, menu_selector(EvilLevelInfoLayer::info)
				);
				featuredCoinBtn->setPosition(pos);
				featureIcon->setZOrder(-2);
				menu->addChild(featuredCoinBtn);
			}
			if (level->m_isEpic == 3) {
				btnFeature->setVisible(false);
				auto featureIcon = CCSprite::createWithSpriteFrameName("GJ_epicCoin3_001.png");
				auto featuredCoinBtn = CCMenuItemSpriteExtra::create(
					featureIcon, this, menu_selector(EvilLevelInfoLayer::info)
				);
				featuredCoinBtn->setPosition(pos);
				featureIcon->setZOrder(-2);
				menu->addChild(featuredCoinBtn);
			}
		}
		if (level->m_featured)
		{
			btnFeature->setPosition(pos);
			featureIconCoin->setZOrder(-3);
				menu->addChild(btnFeature);
			log::debug("featured level");
		}
		else {
			log::debug("not featured wahh");
			if (Mod::get()->getSettingValue<bool>("unrated")) {
				auto infoBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
				auto noRateBtn = CCMenuItemSpriteExtra::create(
					infoBtnSpr, this, menu_selector(EvilLevelInfoLayer::noRateInfo)
				);
				noRateBtn->setPosition({170.250, 212});
				menu->addChild(noRateBtn);
				menu->setZOrder(3);
				infoBtnSpr->setScale(0.65);
			}
		}
		return true;
	}
};
