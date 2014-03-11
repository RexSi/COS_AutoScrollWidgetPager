/*
 * AutoScrool.cpp
 *
 *  Created on: Mar 3, 2014
 *      Author: Rex Si
 */

#include <gaia/base/String.h>
#include <gaia/core/event/MotionEvent.h>
#include <gaia/core/Message.h>
#include <gaia/ui/IPagerProvider.h>

#include "AutoScrollWidgetPager.h"

using namespace gaia::base;
using namespace gaia::core;
using namespace gaia::ui;

const int32_t AutoScrollWidgetPager::DEFAULT_INTERVAL = 2000; // milliseconds

AutoScrollWidgetPager::AutoScrollWidgetPager(Context* paramContext)
        : WidgetPager(paramContext, true),
          mInterval(DEFAULT_INTERVAL),
          mDirection(RIGHT),
          mIsCycle(true),
          mStopScrollWhenTouch(true),
          mSlideBorderMode(SLIDE_BORDER_MODE_NONE),
          mIsBorderAnimation(true),
          mIsAutoScroll(false),
          mIsStopByTouch(false),
          mTouchX(0.0),
          mDownX(0.0) {
}

AutoScrollWidgetPager::AutoScrollWidgetPager(Context* paramContext, AttributeSet* paramAttributeSet)
        : WidgetPager(paramContext, paramAttributeSet, true),
          mInterval(DEFAULT_INTERVAL),
          mDirection(RIGHT),
          mIsCycle(true),
          mStopScrollWhenTouch(true),
          mSlideBorderMode(SLIDE_BORDER_MODE_NONE),
          mIsBorderAnimation(true),
          mIsAutoScroll(false),
          mIsStopByTouch(false),
          mTouchX(0.0),
          mDownX(0.0) {
}

void AutoScrollWidgetPager::startAutoScroll() {
    mIsAutoScroll = true;
    sendScrollMessage(mInterval);
}

void AutoScrollWidgetPager::startAutoScroll(int32_t delayTimeInMills) {
    mIsAutoScroll = true;
    sendScrollMessage(delayTimeInMills);
}

void AutoScrollWidgetPager::stopAutoScroll() {
    mIsAutoScroll = false;
    removeMessages(SCROLL_WHAT);
}

void AutoScrollWidgetPager::sendScrollMessage(int64_t delayTimeInMills) {
    removeMessages(SCROLL_WHAT);
    Message msg;
    msg.setCode(SCROLL_WHAT);
    sendMessageDelayed(msg, delayTimeInMills);
}

void AutoScrollWidgetPager::scrollOnce() {
    IPagerProvider* pProvider = getProvider();
    if (pProvider == NULL) {
        return;
    }
    int32_t totalCount = pProvider->getCount();
    if (totalCount <= 1) {
        return;
    }
    int32_t currentItem = getCurrentItem();

    int32_t nextItem = (mDirection == LEFT) ? --currentItem : ++currentItem;
    if (nextItem < 0) {
        if (mIsCycle) {
            setCurrentItem(totalCount - 1, mIsBorderAnimation);
        }
    } else if (nextItem == totalCount) {
        if (mIsCycle) {
            setCurrentItem(0, mIsBorderAnimation);
        }
    } else {
        setCurrentItem(nextItem, mIsBorderAnimation);
    }
}

bool AutoScrollWidgetPager::onTouchEvent(const MotionEvent& ev) {
    if (mStopScrollWhenTouch) {
        if (ev.getAction() == MotionEvent::ACTION_DOWN && mIsAutoScroll) {
            mIsStopByTouch = true;
            stopAutoScroll();
        } else if (ev.getAction() == MotionEvent::ACTION_UP && mIsStopByTouch) {
            startAutoScroll();
        }
    }

    WidgetController* pParent = dynamic_cast<WidgetController*>(getParent());
    if (mSlideBorderMode == SLIDE_BORDER_MODE_TO_PARENT || mSlideBorderMode == SLIDE_BORDER_MODE_CYCLE) {
        mTouchX = ev.getX();
        if (ev.getAction() == MotionEvent::ACTION_DOWN) {
            mDownX = mTouchX;
        }
        int32_t currentItem = getCurrentItem();
        IPagerProvider* adapter = getProvider();
        int32_t pageCount = adapter == NULL ? 0 : adapter->getCount();
        /**
         * current index is first one and slide to right or current index is last one and slide to left.<br/>
         * if slide border mode is to parent, then requestDisallowInterceptTouchEvent false.<br/>
         * else scroll to last one when current item is first one, scroll to first one when current item is last
         * one.
         */
        if ((currentItem == 0 && mDownX <= mTouchX) || (currentItem == pageCount - 1 && mDownX >= mTouchX)) {
            if (mSlideBorderMode == SLIDE_BORDER_MODE_TO_PARENT) {
                if (pParent)
                    pParent->requestDisallowInterceptTouchEvent(false);
            } else {
                if (pageCount > 1) {
                    setCurrentItem(pageCount - currentItem - 1, mIsBorderAnimation);
                }
                if (pParent)
                    pParent->requestDisallowInterceptTouchEvent(true);
            }
            return WidgetPager::onTouchEvent(ev);
        }
    }
    if (pParent)
        pParent->requestDisallowInterceptTouchEvent(true);
    return WidgetPager::onTouchEvent(ev);
}

void AutoScrollWidgetPager::handleMessage(const Message& msg) {
    switch (msg.getCode()) {
    case SCROLL_WHAT:
        scrollOnce();
        sendScrollMessage(mInterval);
    default:
        break;
    }
}

/**
 * get auto scroll time in milliseconds, default is {@link #DEFAULT_INTERVAL}
 *
 * @return the interval
 */
int64_t AutoScrollWidgetPager::getInterval() const {
    return mInterval;
}

/**
 * set auto scroll time in milliseconds, default is {@link #DEFAULT_INTERVAL}
 *
 * @param interval the interval to set
 */
void AutoScrollWidgetPager::setInterval(int64_t interval) {
    mInterval = interval;
}

/**
 * get auto scroll direction
 *
 * @return {@link #LEFT} or {@link #RIGHT}, default is {@link #RIGHT}
 */
int32_t AutoScrollWidgetPager::getDirection() const {
    return (mDirection == LEFT) ? LEFT : RIGHT;
}

/**
 * set auto scroll direction
 *
 * @param direction {@link #LEFT} or {@link #RIGHT}, default is {@link #RIGHT}
 */
void AutoScrollWidgetPager::setDirection(int32_t direction) {
    mDirection = direction;
}

/**
 * whether automatic cycle when auto scroll reaching the last or first item, default is true
 *
 * @return the isCycle
 */
bool AutoScrollWidgetPager::isCycle() const {
    return mIsCycle;
}

/**
 * set whether automatic cycle when auto scroll reaching the last or first item, default is true
 *
 * @param isCycle the isCycle to set
 */
void AutoScrollWidgetPager::setCycle(bool isCycle) {
    mIsCycle = isCycle;
}

/**
 * whether stop auto scroll when touching, default is true
 *
 * @return the stopScrollWhenTouch
 */
bool AutoScrollWidgetPager::isStopScrollWhenTouch() const {
    return mStopScrollWhenTouch;
}

/**
 * set whether stop auto scroll when touching, default is true
 *
 * @param stopScrollWhenTouch
 */
void AutoScrollWidgetPager::setStopScrollWhenTouch(bool stopScrollWhenTouch) {
    mStopScrollWhenTouch = stopScrollWhenTouch;
}

/**
 * get how to process when sliding at the last or first item
 *
 * @return the slideBorderMode {@link #SLIDE_BORDER_MODE_NONE}, {@link #SLIDE_BORDER_MODE_TO_PARENT},
 * {@link #SLIDE_BORDER_MODE_CYCLE}, default is {@link #SLIDE_BORDER_MODE_NONE}
 */
int32_t AutoScrollWidgetPager::getSlideBorderMode() const {
    return mSlideBorderMode;
}

/**
 * set how to process when sliding at the last or first item
 *
 * @param slideBorderMode {@link #SLIDE_BORDER_MODE_NONE}, {@link #SLIDE_BORDER_MODE_TO_PARENT},
 * {@link #SLIDE_BORDER_MODE_CYCLE}, default is {@link #SLIDE_BORDER_MODE_NONE}
 */
void AutoScrollWidgetPager::setSlideBorderMode(int32_t slideBorderMode) {
    mSlideBorderMode = slideBorderMode;
}

/**
 * whether animating when auto scroll at the last or first item, default is true
 *
 * @return
 */
bool AutoScrollWidgetPager::isBorderAnimation() const {
    return mIsBorderAnimation;
}

/**
 * set whether animating when auto scroll at the last or first item, default is true
 *
 * @param isBorderAnimation
 */
void AutoScrollWidgetPager::setBorderAnimation(bool isBorderAnimation) {
    mIsBorderAnimation = isBorderAnimation;
}
