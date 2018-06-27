#include "UsersInfoScene.h"
#include "network\HttpClient.h"
#include "json\document.h"
#include "Utils.h"

using namespace cocos2d::network;
using namespace rapidjson;

cocos2d::Scene * UsersInfoScene::createScene() {
  return UsersInfoScene::create();
}

bool UsersInfoScene::init() {
  if (!Scene::init()) return false;

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto getUserButton = MenuItemFont::create("Get User", CC_CALLBACK_1(UsersInfoScene::getUserButtonCallback, this));
  if (getUserButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + getUserButton->getContentSize().height / 2;
    getUserButton->setPosition(Vec2(x, y));
  }

  auto backButton = MenuItemFont::create("Back", [](Ref* pSender) {
    Director::getInstance()->popScene();
  });
  if (backButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - backButton->getContentSize().height / 2;
    backButton->setPosition(Vec2(x, y));
  }

  auto menu = Menu::create(getUserButton, backButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  limitInput = TextField::create("limit", "arial", 24);
  if (limitInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 100.0f;
    limitInput->setPosition(Vec2(x, y));
    this->addChild(limitInput, 1);
  }

  messageBox = Label::create("", "arial", 30);
  if (messageBox) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height / 2;
    messageBox->setPosition(Vec2(x, y));
    this->addChild(messageBox, 1);
  }

  return true;
}

void UsersInfoScene::getUserButtonCallback(Ref * pSender) {
  // Your code here
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8000/users?limit=" + limitInput->getString());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(UsersInfoScene::onHttpRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void UsersInfoScene::onHttpRequestCompleted(HttpClient * sender, HttpResponse * response) {
	if (!response)	return;

	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
	}

	rapidjson::Document document;
	std::vector<char> *data = response->getResponseData();
	std::string res;
	res.insert(res.begin(), data->begin(), data->end());
	document.Parse<rapidjson::kParseDefaultFlags>(res.c_str());

	std::string errMsg = "Get Users Failed!\n";

	if (document["status"] == false) {
		messageBox->setString(errMsg + document["msg"].GetString());
	}
	else {
		std::string output = "";
		// 用户
		for (unsigned i = 0; i < document["data"].Size(); ++i) {
			output = output + "Username: " + document["data"][i]["username"].GetString() + "\n";
			output += "Deck: \n";
			// 卡牌数组
			for (unsigned j = 0; j < (document["data"])[i]["deck"].Size(); j++) {
				// 每组卡牌遍历
				for (rapidjson::Value::ConstMemberIterator iter = ((document["data"])[i]["deck"])[j].MemberBegin(); iter != ((document["data"])[i]["deck"])[j].MemberEnd(); iter++) {
					rapidjson::Value jKey;
					rapidjson::Value jValue;
					Document::AllocatorType allocator;
					jKey.CopyFrom(iter->name, allocator);
					jValue.CopyFrom(iter->value, allocator);
					if (jKey.IsString()) {
						std::string name = jKey.GetString();
						std::string value = cocos2d::Value(jValue.GetInt()).asString();
						output += "    " +  name + ':' + value + '\n';
					}
				}
				output += "    ---\n";
			}
			output += "---\n";
		}
		messageBox->setString(output);
	}
}