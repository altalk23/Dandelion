#include <FPSOverlay.hpp>
#include <Geode/Loader.hpp>
#include <Main.hpp>
#include <sstream>

USE_GEODE_NAMESPACE();
using namespace tulip::dandelion;

static inline FPSOverlay* s_sharedState = nullptr;

FPSOverlay::FPSOverlay() :
	m_lock(false),
	m_frames(0),
	m_resetInterval(1.0),
	m_lastString("") {}

FPSOverlay* FPSOverlay::get() {
	if (!s_sharedState) {
		s_sharedState = new FPSOverlay();
		s_sharedState->init();
	}
	return s_sharedState;
}

cocos2d::CCSize FPSOverlay::getBackdropSize() {
	auto textSize = m_label->getContentSize();
	return cocos2d::CCSizeMake(2 * 6 + textSize.width * 2, 2 * 6 + textSize.height * 2);
}

bool FPSOverlay::init() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	m_label = cocos2d::CCLabelBMFont::create("No FPS", "chatFont.fnt");
	m_label->setPosition({ 3, winSize.height - 3 });
	m_label->setAnchorPoint(cocos2d::CCPoint(0, 1));
	m_label->setScale(0.4);
	m_label->setColor(cocos2d::ccc3(255, 0, 255));
	this->addChild(m_label, 2);

	m_backdrop = cocos2d::extension::CCScale9Sprite::create("square02_small.png");
	m_backdrop->setContentSize(getBackdropSize());
	m_backdrop->setPosition({ 2, winSize.height - 2 });
	m_backdrop->setAnchorPoint(cocos2d::CCPoint(0, 1));
	m_backdrop->setOpacity(127);
	m_backdrop->setScale(0.2);
	this->addChild(m_backdrop, 1);

	if (m_lastString != "") {
		this->updateLabel(m_lastString);
	}

	return true;
}

void FPSOverlay::update(float dt) {
	++m_frames;
	m_accumulate += dt;

	if (m_accumulate > m_resetInterval) {
		float framerate = m_frames / m_accumulate;
		framerate = std::round(framerate * 10) / 10;
		m_frames = 0;
		m_accumulate = 0;

		std::stringstream stream;
		stream << framerate << " FPS";
		m_lastString = stream.str();
		this->updateLabel(m_lastString);
	}
}

void FPSOverlay::updateLabel(std::string const& str) {
	m_label->setString(str.c_str());
	m_backdrop->setContentSize(getBackdropSize());
}

// SceneManager needs an update to obey loadinglayer

#include <Geode/modify/AchievementNotifier.hpp>

struct WillSwitchToScene : Modify<WillSwitchToScene, AchievementNotifier> {
	void willSwitchToScene(cocos2d::CCScene* newScene) {
		AchievementNotifier::willSwitchToScene(newScene);
		if (FPSOverlay::get()->m_lock) {
			return;
		}

		if (FPSOverlay::get()->getParent() != newScene) {
			FPSOverlay::get()->removeFromParentAndCleanup(false);
		}

		if (auto children = newScene->getChildren()) {
			if (auto layer = reinterpret_cast<cocos2d::CCNode*>(children->objectAtIndex(0))) {
				layer->addChild(FPSOverlay::get(), 100001);
			}
		}
	}
};

#include <Geode/modify/LoadingLayer.hpp>

struct LoadingLayerCheck : Modify<LoadingLayerCheck, LoadingLayer> {
	bool init(bool fromReload) {
		if (!LoadingLayer::init(fromReload)) {
			return false;
		}
		FPSOverlay::get()->removeAllChildren();
		FPSOverlay::get()->m_lock = true;
		return true;
	}
};

#include <Geode/modify/MenuLayer.hpp>

struct MenuLayerCheck : Modify<MenuLayerCheck, MenuLayer> {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}
		FPSOverlay::get()->m_lock = false;
		FPSOverlay::get()->removeAllChildren();
		FPSOverlay::get()->init();
		return true;
	}
};

$execute {
	registerEnable([]() {
		Loader::get()->queueInGDThread([]() {
			CCDirector::get()->getScheduler()->scheduleUpdateForTarget(FPSOverlay::get(), 0, true);
			auto scene = CCDirector::get()->getRunningScene();
			if (scene) {
				if (scene->getChildrenCount() > 0) {
					auto layer = static_cast<CCNode*>(scene->getChildren()->objectAtIndex(0));
					layer->addChild(FPSOverlay::get(), 100001);
				}
			}
		});
	});

	registerDisable([]() {
		Loader::get()->queueInGDThread([]() {
			CCDirector::get()->getScheduler()->unscheduleUpdateForTarget(FPSOverlay::get());
			FPSOverlay::get()->removeFromParentAndCleanup(false);
		});
	});

	// listenForSettingChanges<BoolSetting>("fps-overlay", [](auto setting) {
	// 	FPSOverlay::get()->setVisible(setting->getValue());
	// });
}
