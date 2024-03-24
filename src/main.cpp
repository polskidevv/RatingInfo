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
    void info(CCObject *sender) {
        FLAlertLayer::create(
            "Rating",
            "A <co>rating</c> indicates the quality of a certain <cl>level</c>.\n<cy>Featured:</c> A high quality <cy>star-rated</c> level.\n<co>Epic:</c> A higher quality <cy>star-rated</c> level.\n<cp>Legendary:</c> A very high quality <cy>star-rated</c> level.\n<cj>Mythic:</c> A <cy>star-rated</c> level with <cr>incredible</c> quality.\n<cg>Every level is rated by</c> <co>RobTop</c>.",
            "OK"
        )->show();
    }

    void starOnly(CCObject *sender) {
		if(plat) {
			std::string content = "This <cl>level</c> is <cy>rated</c>, but it's not <cy>featured</c>.\nMoons requested:<cg> " + std::to_string(req) + "</c>.";
			FLAlertLayer::create(
				"Moon Rating",
				content.c_str(),
				"OK"
			)->show();	
		}
		else {
			std::string content = "This <cl>level</c> is <cy>rated</c>, but it's not <cy>featured</c>.\nStars requested:<cg> " + std::to_string(req) + "</c>.";
			FLAlertLayer::create(
				"Star Rating",
				content.c_str(),
				"OK"
			)->show();
		}
    }

    void noRateInfo(CCObject *sender) {
		if(req == 0) {
			if(plat) {
        		std::string content = "This <cl>level</c> is <cr>unrated</c>.\nMoons requested:<cr> none</c>.";
				FLAlertLayer::create(
					"Moon Rating",
					content.c_str(),
					"OK"
				)->show();
			}
			else {
        		std::string content = "This <cl>level</c> is <cr>unrated</c>.\nStars requested:<cr> none</c>.";
				FLAlertLayer::create(
					"Star Rating",
					content.c_str(),
					"OK"
				)->show();
			}

		}
		else {
			if(plat) {
				std::string content = "This <cl>level</c> is <cr>unrated</c>.\nMoons requested:<cg> " + std::to_string(req) + "</c>.";
				FLAlertLayer::create(
					"Moon Rating",
					content.c_str(),
					"OK"
				)->show();
			}
			else {
				std::string content = "This <cl>level</c> is <cr>unrated</c>.\nStars requested:<cg> " + std::to_string(req) + "</c>.";
				FLAlertLayer::create(
					"Star Rating",
					content.c_str(),
					"OK"
				)->show();
			}
		}
    }

	bool init(GJGameLevel* level, bool p1) {
		if (!LevelInfoLayer::init(level, p1))
            return false;

        auto originalIcon = static_cast<CCSprite*>(this->getChildByID("difficulty-sprite"));
		auto starIcon = static_cast<CCSprite*>(this->getChildByID("stars-icon"));
        auto starPos = starIcon->getPosition();

        auto zpos = originalIcon->getZOrder();
        auto pos = originalIcon->getPosition();
		auto menu = CCMenu::create();
		this->addChild(menu);
		menu->setPosition(0, 0);

        req = level->m_starsRequested;

        plat = level->isPlatformer();

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
			log::debug("not featured");
			if (Mod::get()->getSettingValue<bool>("unrated")) {
				if(level->m_stars == 0) {
					auto infoBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
					auto noRateBtn = CCMenuItemSpriteExtra::create(
						infoBtnSpr, this, menu_selector(EvilLevelInfoLayer::noRateInfo)
					);
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
				else if(level->m_stars > 0 && !level->m_featured) {
					if(plat) {
						starIcon->setVisible(false);
						auto featureIcon = CCSprite::createWithSpriteFrameName("moon_small01_001.png");
						auto featuredCoinBtn = CCMenuItemSpriteExtra::create(
							featureIcon, this, menu_selector(EvilLevelInfoLayer::starOnly)
						);
						featuredCoinBtn->setPosition(starPos);
						featureIcon->setZOrder(-2);
						menu->addChild(featuredCoinBtn);
					}
					else {
						starIcon->setVisible(false);
						auto featureIcon = CCSprite::createWithSpriteFrameName("star_small01_001.png");
						auto featuredCoinBtn = CCMenuItemSpriteExtra::create(
							featureIcon, this, menu_selector(EvilLevelInfoLayer::starOnly)
						);
						featuredCoinBtn->setPosition(starPos);
						featureIcon->setZOrder(-2);
						menu->addChild(featuredCoinBtn);
					}
				}
			}
		}
		return true;
	}
};
