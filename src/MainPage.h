#pragma once
#include <gaia/core/Page.h>

class MainPage: public gaia::core::Page {
public:
    MainPage();
    ~MainPage();

protected:
    virtual void onInit(gaia::core::Persistence* const p);
    virtual void onTear();
    gaia::base::Vector<gaia::core::Widget*> createWidgets();

private:
    AutoScrollWidgetPager* mpWidgetPager;
    gaia::ui::IPagerProvider* mpProvider;
    gaia::base::Vector<gaia::core::Widget*> mWidgetPool;
};

class MyPageProvider: public gaia::ui::IPagerProvider {
public:
    MyPageProvider(const gaia::base::Vector<gaia::core::Widget*>& pWidgets);
    virtual ~MyPageProvider();

    virtual int32_t getCount();
    virtual gaia::base::CharSequence* getPageTitle(int32_t position);
    virtual gaia::core::Widget* instantiateItem(gaia::core::WidgetController* container, int32_t position);
    virtual void destroyItem(gaia::core::WidgetController* container, int32_t position, gaia::core::Widget* object);
    virtual bool isWidgetFromObject(gaia::core::Widget* widget, gaia::core::Widget* object);
    virtual int32_t getItemPosition(gaia::core::Widget* object);

private:
    gaia::base::Vector<gaia::core::Widget*> mWidgets;
};
