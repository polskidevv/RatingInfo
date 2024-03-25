#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

// lord forgive me for this shitty code

class $modify(EvilLevelInfoLayer, LevelInfoLayer) {
private:
	int req, rating;
public:
	void info(CCObject *sender) {
		FLAlertLayer::create(
			"Rating",
			"A <co>rating</c> indicates the quality of a certain <cg>level</c>.\n<co>Rated:</c> A <cy>star-rated</c> level.\n<cy>Featured:</c> A quality <cy>star-rated</c> level.\n<co>Epic:</c> A high quality <cy>star-rated</c> level.\n<cp>Legendary:</c> A higher quality <cy>star-rated</c> level.\n<cj>Mythic:</c> A <cy>star-rated</c> level with <cr>incredible</c> quality.\nEvery level is <cy>rated</c> by <co>RobTop</c>.",
			"OK"
		)->show();
	}

	void noRateInfo(CCObject *sender) {
		if (m_fields->req == 0 && m_fields->rating == 0) {
			std::string content = "This <cg>level</c> is <cr>unrated</c>.\n<cy>Stars requested:</c><cr> none</c>.";
			FLAlertLayer::create(
				"Rating",
				content.c_str(),
				"OK"
			)->show();
		} else if (m_fields->rating == 0) {
			std::string content = "This <cg>level</c> is <cr>not featured</c>.\n<cy>Stars requested:</c><cg> " + std::to_string(rating) + "</c>.";
			FLAlertLayer::create(
				"Rating",
				content.c_str(),
				"OK"
			)->show();
		} else {
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
		menu->setID("rating-info"_spr);

		m_fields->req = level->m_starsRequested;
		m_fields->rating = level->m_stars.value();

		//stupid workaround for the feature button, dont ask
		auto featureIconCoin = CCSprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
		auto btnFeature = CCMenuItemSpriteExtra::create(featureIconCoin, this, menu_selector(EvilLevelInfoLayer::info));

		if (level->m_isEpic) {
			if (!Loader::get()->getLoadedMod("acaruso.horn")) {
				dynamic_cast<CCSprite*>(originalIcon->getChildren()->objectAtIndex(0))->setOpacity(0);
				btnFeature->setVisible(false);
				if (level->m_isEpic == 1) {
					auto featureIcon = CCSprite::createWithSpriteFrameName("GJ_epicCoin_001.png");
					auto featuredCoinBtn = CCMenuItemSpriteExtra::create(featureIcon, this, menu_selector(EvilLevelInfoLayer::info));
					featuredCoinBtn->setPosition(pos);
					featureIcon->setZOrder(-2);
					menu->addChild(featuredCoinBtn);
				} else if (level->m_isEpic == 2) {
					auto featureIcon = CCSprite::createWithSpriteFrameName("GJ_epicCoin2_001.png");
					auto featuredCoinBtn = CCMenuItemSpriteExtra::create(featureIcon, this, menu_selector(EvilLevelInfoLayer::info));
					featuredCoinBtn->setPosition(pos);
					featureIcon->setZOrder(-2);
					menu->addChild(featuredCoinBtn);
				} else if (level->m_isEpic == 3) {
					auto featureIcon = CCSprite::createWithSpriteFrameName("GJ_epicCoin3_001.png");
					auto featuredCoinBtn = CCMenuItemSpriteExtra::create(featureIcon, this, menu_selector(EvilLevelInfoLayer::info));
					featuredCoinBtn->setPosition(pos);
					featureIcon->setZOrder(-2);
					menu->addChild(featuredCoinBtn);
				}
			} else {
				auto infoIcon = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
				auto infoBtn = CCMenuItemSpriteExtra::create(infoIcon, this, menu_selector(EvilLevelInfoLayer::info));
				infoBtn->setPosition({456, getChildByIDRecursive("other-menu")->getPositionY()});
				menu->addChild(infoBtn);
			}
		} else if (level->m_featured) {
			if (!Loader::get()->getLoadedMod("acaruso.horn")) {
				btnFeature->setPosition(pos);
				menu->addChild(btnFeature);
			} else {
				auto infoIcon = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
				auto infoBtn = CCMenuItemSpriteExtra::create(infoIcon, this, menu_selector(EvilLevelInfoLayer::info));
				infoBtn->setPosition({456, getChildByIDRecursive("other-menu")->getPositionY()});
				menu->addChild(infoBtn);
			}
		} else if (m_fields->rating != 0 || !Mod::get()->getSettingValue<bool>("unrated")) {
			auto infoBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
			auto noRateBtn = CCMenuItemSpriteExtra::create(infoBtnSpr, this, menu_selector(EvilLevelInfoLayer::noRateInfo));
			// booo android
			#if defined(GEODE_IS_ANDROID)
				noRateBtn->setPosition({232.250, 212});
			#else
				noRateBtn->setPosition({170.250, 212});
			#endif
			menu->addChild(noRateBtn);
			menu->setZOrder(3);
			infoBtnSpr->setScale(0.65);
		}
		return true;
	}
};
