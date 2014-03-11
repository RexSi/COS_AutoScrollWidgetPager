#define DEBUG_LEVEL 0
#define LOG_TAG "AutoScrollViewPagerTest"

#include <pthread.h>
#include <bionic/libc/include/sys/socket.h>
#include <gaia/base/gloger.h>
#include <gaia/base/String.h>
#include <gaia/base/Vector.h>
#include <gaia/core/Context.h>
#include <gaia/core/Persistence.h>
#include <gaia/core/vision/Scene.h>
#include <gaia/ui/Button.h>
#include <gaia/ui/ImageWidget.h>
#include <gaia/ui/IPagerProvider.h>
#include <gaia/ui/LinearController.h>
#include <gaia/ui/LinearControllerParams.h>
#include <gaia/ui/RelativeController.h>
#include <gaia/ui/RelativeControllerParams.h>
#include <gaia/ui/TextWidget.h>

#include "AutoScrollViewPagerTest/R.h"
#include "AutoScrollWidgetPager.h"
#include "MainPage.h"

using namespace gaia::base;
using namespace gaia::core;
using namespace gaia::ui;

const int32_t gPictures[] = {
        AutoScrollViewPagerTest::R::drawable::picture_1,
        AutoScrollViewPagerTest::R::drawable::picture_2,
        AutoScrollViewPagerTest::R::drawable::picture_3,
        AutoScrollViewPagerTest::R::drawable::picture_4,
        AutoScrollViewPagerTest::R::drawable::picture_5,
        AutoScrollViewPagerTest::R::drawable::picture_6,
        AutoScrollViewPagerTest::R::drawable::picture_7,
        AutoScrollViewPagerTest::R::drawable::picture_8,
        AutoScrollViewPagerTest::R::drawable::picture_9,
        AutoScrollViewPagerTest::R::drawable::picture_10,
};

MainPage::MainPage()
        : mpProvider(NULL), mpWidgetPager(NULL) {
}

MainPage::~MainPage() {
}

void MainPage::onInit(Persistence* const p) {
    Scene::SceneFactory(this);

    mpProvider = new MyPageProvider(createWidgets());
    LinearControllerParams params(ControllerParams::MATCH_PARENT, 400);
    mpWidgetPager = new AutoScrollWidgetPager(&Context(this));
    mpWidgetPager->setControllerParams(&params);
    mpWidgetPager->setProvider(mpProvider);
    mpWidgetPager->startAutoScroll(5000);

    LinearController* pCtrl = new LinearController(this);
    mWidgetPool.append(pCtrl);
    pCtrl->addWidget(mpWidgetPager);

    mpScene->attachController(pCtrl);
    mpScene->setupSceneLayout();
}

void MainPage::onTear() {
    for (size_t i = 0; i < mWidgetPool.size(); i++) {
        delete mWidgetPool.itemAt(i);
    }
    mWidgetPool.clear();
    delete mpProvider;
    delete mpWidgetPager;
    getSceneSingleton()->SceneDestroy();
}

Vector<Widget*> MainPage::createWidgets() {
    RelativeController* plc = NULL;;
    TextWidget* txt = NULL;
    Button* btn = NULL;
    ImageWidget* img = NULL;

    RelativeControllerParams imgParams(ControllerParams::MATCH_PARENT, ControllerParams::MATCH_PARENT);

    RelativeControllerParams button(ControllerParams::WRAP_CONTENT, ControllerParams::WRAP_CONTENT);
    button.setRelation(RelativeControllerParams::ALIGN_PARENT_BOTTOM);
    button.setRelation(RelativeControllerParams::ALIGN_PARENT_LEFT);

    RelativeControllerParams text(ControllerParams::WRAP_CONTENT, ControllerParams::WRAP_CONTENT);
    text.setRelation(RelativeControllerParams::CENTER_IN_PARENT);

    Vector<Widget*> widgets;
    for (size_t i = 0; i < 10; i++) {
        plc = new RelativeController(this);
        mWidgetPool.append(plc);

        img = new ImageWidget(this);
        mWidgetPool.append(img);
        img->setImageResource(gPictures[i]);
        img->setScaleType(ImageWidget::FIT_XY);
        plc->addWidget(img);
        img->setControllerParams(&imgParams);

        txt = new TextWidget(this);
        txt->setControllerParams(&text);
        mWidgetPool.append(txt);
        txt->setText(String::format("this is %d picture.", i + 1));
        txt->setTextColor(0xffff0000);
        plc->addWidget(txt);

        btn = new Button(this);
        btn->setControllerParams(&button);
        mWidgetPool.append(btn);
        btn->setText(String::format("button %d", i+1));
        plc->addWidget(btn);

        widgets.append(plc);
    }
    GLOG(LOG_TAG, LOGINFO, "widgets size %d", widgets.size());
    return widgets;
}

MyPageProvider::MyPageProvider(const Vector<Widget*>& pWidgets)
        : mWidgets(pWidgets) {
    GLOGENTRY(LOG_TAG);
}

MyPageProvider::~MyPageProvider() {
    GLOGENTRY(LOG_TAG);
}

int32_t MyPageProvider::getCount() {
    GLOGENTRY(LOG_TAG);
    return mWidgets.size();
}

CharSequence* MyPageProvider::getPageTitle(int32_t position) {
    GLOGENTRY(LOG_TAG);
    GLOG(LOG_TAG, LOGDBG, "getPageTitle position %d", position);
    return NULL;
}

Widget* MyPageProvider::instantiateItem(WidgetController* container, int32_t position) {
    GLOGENTRY(LOG_TAG);
    GLOG(LOG_TAG, LOGDBG, "instantiateItem position %d/%d", position, mWidgets.size());
    container->addWidget(mWidgets[position]);
    return mWidgets[position];
}

void MyPageProvider::destroyItem(WidgetController* container, int32_t position, Widget* object) {
    GLOGENTRY(LOG_TAG);
    container->removeWidget(mWidgets[position]);
}

bool MyPageProvider::isWidgetFromObject(Widget* widget, Widget* object) {
    GLOGENTRY(LOG_TAG);
    return widget == object;
}

int32_t MyPageProvider::getItemPosition(gaia::core::Widget* object) {
    GLOGENTRY(LOG_TAG);
    return POSITION_NONE;
}

template class Export<MainPage, Page> ;

