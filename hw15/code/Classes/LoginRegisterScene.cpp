#include "LoginRegisterScene.h"
#include "Utils.h"

USING_NS_CC;
using namespace cocos2d::network;
using namespace cocos2d::ui;

cocos2d::Scene * LoginRegisterScene::createScene() {
  return LoginRegisterScene::create();
}

bool LoginRegisterScene::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto loginButton = MenuItemFont::create("Login", CC_CALLBACK_1(LoginRegisterScene::loginButtonCallback, this));
  if (loginButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + loginButton->getContentSize().height / 2;
    loginButton->setPosition(Vec2(x, y));
  }

  auto registerButton = MenuItemFont::create("Register", CC_CALLBACK_1(LoginRegisterScene::registerButtonCallback, this));
  if (registerButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + registerButton->getContentSize().height / 2 + 100;
    registerButton->setPosition(Vec2(x, y));
  }

  auto backButton = MenuItemFont::create("Back", [] (Ref* pSender) {
    Director::getInstance()->popScene();
  });
  if (backButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - backButton->getContentSize().height / 2;
    backButton->setPosition(Vec2(x, y));
  }

  auto menu = Menu::create(loginButton, registerButton, backButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  usernameInput = TextField::create("username", "arial", 24);
  if (usernameInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 100.0f;
    usernameInput->setPosition(Vec2(x, y));
    this->addChild(usernameInput, 1);
  }

  passwordInput = TextField::create("password", "arial", 24);
  if (passwordInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 130.0f;
    passwordInput->setPosition(Vec2(x, y));
    this->addChild(passwordInput, 1);
  }

  messageBox = Label::create("", "arial", 30);
  if (messageBox) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 200.0f;
    messageBox->setPosition(Vec2(x, y));
    this->addChild(messageBox, 1);
  }

  return true;
}

void LoginRegisterScene::loginButtonCallback(cocos2d::Ref * pSender) {
	// Your code here
	//log("%s", usernameInput->getString().c_str());
	//log("%s", passwordInput->getString().c_str());
	if (usernameInput->getString() == "" || passwordInput->getString() == "") {
		this->messageBox->setString("Login Failed!\n");
		return;
	}
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value username(StringRef(usernameInput->getString().c_str()));
	document.AddMember("username", username, allocator);

	rapidjson::Value password(StringRef(passwordInput->getString().c_str()));
	document.AddMember("password", password, allocator);

	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	document.Accept(writer);
	//log("%s\n", buffer.GetString());

	ask = "Login";
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8000/auth");
	request->setRequestType(HttpRequest::Type::POST);
	request->setRequestData(buffer.GetString(), strlen(buffer.GetString()));
	request->setResponseCallback(CC_CALLBACK_2(LoginRegisterScene::onHttpRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->enableCookies(nullptr);
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void LoginRegisterScene::registerButtonCallback(Ref * pSender) {
	// Your code here
	//log("%s", usernameInput->getString().c_str());
	//log("%s", passwordInput->getString().c_str());

	if (usernameInput->getString() == "" || passwordInput->getString() == "") {
		this->messageBox->setString("Login Failed!\n");
		return;
	}

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value username(StringRef(usernameInput->getString().c_str()));
	document.AddMember("username", username, allocator);

	rapidjson::Value password(StringRef(passwordInput->getString().c_str()));
	document.AddMember("password", password, allocator);

	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	document.Accept(writer);
	//log("%s\n", buffer.GetString());

	ask = "Register";
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8000/users");
	request->setRequestType(HttpRequest::Type::POST);
	request->setRequestData(buffer.GetString(), strlen(buffer.GetString()));
	request->setResponseCallback(CC_CALLBACK_2(LoginRegisterScene::onHttpRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void LoginRegisterScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
	if (!response)	return;

	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
	}
	std::string header;
	header.insert(header.begin(), response->getResponseHeader()->begin(), response->getResponseHeader()->end());
	log("%s", header.c_str());
	rapidjson::Document document;
	std::vector<char> *data = response->getResponseData();
	std::string res;
	res.insert(res.begin(), data->begin(), data->end());
	document.Parse<rapidjson::kParseDefaultFlags>(res.c_str());
	
	std::string errMsg = ask + " Failed!\n";

	if (document["status"] == false) {
		messageBox->setString(errMsg + document["msg"].GetString());
	}
	else {
		messageBox->setString(document["msg"].GetString());
	}
}
