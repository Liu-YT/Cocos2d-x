#include "ModifyUserScene.h"
#include "Utils.h"
#include "network\HttpClient.h"
#include "json\document.h"

using namespace cocos2d::network;

cocos2d::Scene * ModifyUserScene::createScene() {
  return ModifyUserScene::create();
}

bool ModifyUserScene::init() {
  if (!Scene::init()) return false;
  
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto postDeckButton = MenuItemFont::create("Post Deck", CC_CALLBACK_1(ModifyUserScene::putDeckButtonCallback, this));
  if (postDeckButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + postDeckButton->getContentSize().height / 2;
    postDeckButton->setPosition(Vec2(x, y));
  }

  auto backButton = MenuItemFont::create("Back", [](Ref* pSender) {
    Director::getInstance()->popScene();
  });
  if (backButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - backButton->getContentSize().height / 2;
    backButton->setPosition(Vec2(x, y));
  }

  auto menu = Menu::create(postDeckButton, backButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  deckInput = TextField::create("Deck json here", "arial", 24);
  if (deckInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 100.0f;
    deckInput->setPosition(Vec2(x, y));
    this->addChild(deckInput, 1);
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


void ModifyUserScene::putDeckButtonCallback(Ref * pSender) {
	// Your code here
	std::string deck = "{\"deck\":" + deckInput->getString() + "}";
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8000/users");
	request->setRequestType(HttpRequest::Type::PUT);
	request->setResponseCallback(CC_CALLBACK_2(ModifyUserScene::onHttpRequestCompleted, this));
	request->setRequestData(deck.c_str(), strlen(deck.c_str()));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void ModifyUserScene::onHttpRequestCompleted(HttpClient * sender, HttpResponse * response) {
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

	std::string errMsg = "Update Failed!\n";

	if (document["status"] == false) {
		messageBox->setString(errMsg + document["msg"].GetString());
	}
	else {
		messageBox->setString(document["msg"].GetString());
	}
}