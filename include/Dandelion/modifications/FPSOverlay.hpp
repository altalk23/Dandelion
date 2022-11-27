#include <cocos-ext.h>
#include <cocos2d.h>

namespace tulip::dandelion {
	class FPSOverlay : public cocos2d::CCNode {
	protected:
		cocos2d::CCLabelBMFont* m_label;
		cocos2d::extension::CCScale9Sprite* m_backdrop;
		int m_frames;
		double m_accumulate;
		double m_resetInterval;
		std::string m_lastString;

	public:
		bool m_lock;
		static FPSOverlay* get();
		bool init() override;
		void reset();
		void update(float dt) override;
		void updateLabel(std::string const& str);

		FPSOverlay();
		cocos2d::CCSize getBackdropSize();
	};

}