
#include <Geode/modify/MusicDownloadManager.hpp>

struct MusicResourcesPath : Modify<MusicResourcesPath, MusicDownloadManager> {
	gd::string pathForSong(int songID) {
		std::stringstream stream;
		stream << songID << ".mp3";
		auto savePath =
			ghc::filesystem::path(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
		auto path = savePath / "Resources" / stream.str();
		return gd::string(path.string());
	}
};

$execute {
	auto savePath =
		ghc::filesystem::path(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
	ghc::filesystem::create_directory(savePath / "Resources");
}