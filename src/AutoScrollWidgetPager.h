/*
 * AutoScrool.h
 *
 *  Created on: Mar 3, 2014
 *      Author: Rex Si
 */

#pragma once

#include <gaia/core/Handler.h>
#include <gaia/ui/WidgetPager.h>

class AutoScrollWidgetPager: public gaia::ui::WidgetPager, public gaia::core::Handler {
public:
    AutoScrollWidgetPager(gaia::core::Context* paramContext);
    AutoScrollWidgetPager(gaia::core::Context* paramContext, gaia::core::AttributeSet* paramAttributeSet);
    virtual void handleMessage(const gaia::core::Message& msg);

    /**
     * <ul>
     * if stopScrollWhenTouch is true
     * <li>if event is down, stop auto scroll.</li>
     * <li>if event is up, start auto scroll again.</li>
     * </ul>
     */
    bool onTouchEvent(const gaia::core::MotionEvent& ev);

    /**
     * start auto scroll, first scroll delay time is {@link #getInterval()}
     */
    void startAutoScroll();

    /**
     * start auto scroll
     *
     * @param delayTimeInMills first scroll delay time
     */
    void startAutoScroll(int32_t delayTimeInMills);

    /**
     * stop auto scroll
     */
    void stopAutoScroll();

    /**
     * scroll only once
     */
    void scrollOnce();

    /**
     * get auto scroll time in milliseconds, default is {@link #DEFAULT_INTERVAL}
     *
     * @return the interval
     */
    int64_t getInterval() const;

    /**
     * set auto scroll time in milliseconds, default is {@link #DEFAULT_INTERVAL}
     *
     * @param interval the interval to set
     */
    void setInterval(int64_t interval);

    /**
     * get auto scroll direction
     *
     * @return {@link #LEFT} or {@link #RIGHT}, default is {@link #RIGHT}
     */
    int32_t getDirection() const;

    /**
     * set auto scroll direction
     *
     * @param direction {@link #LEFT} or {@link #RIGHT}, default is {@link #RIGHT}
     */
    void setDirection(int32_t direction);

    /**
     * whether automatic cycle when auto scroll reaching the last or first item, default is true
     *
     * @return the isCycle
     */
    bool isCycle() const;

    /**
     * set whether automatic cycle when auto scroll reaching the last or first item, default is true
     *
     * @param isCycle the isCycle to set
     */
    void setCycle(bool isCycle);

    /**
     * whether stop auto scroll when touching, default is true
     *
     * @return the stopScrollWhenTouch
     */
    bool isStopScrollWhenTouch() const;

    /**
     * set whether stop auto scroll when touching, default is true
     *
     * @param stopScrollWhenTouch
     */
    void setStopScrollWhenTouch(bool stopScrollWhenTouch);

    /**
     * get how to process when sliding at the last or first item
     *
     * @return the slideBorderMode {@link #SLIDE_BORDER_MODE_NONE}, {@link #SLIDE_BORDER_MODE_TO_PARENT},
     * {@link #SLIDE_BORDER_MODE_CYCLE}, default is {@link #SLIDE_BORDER_MODE_NONE}
     */
    int32_t getSlideBorderMode() const;

    /**
     * set how to process when sliding at the last or first item
     *
     * @param slideBorderMode {@link #SLIDE_BORDER_MODE_NONE}, {@link #SLIDE_BORDER_MODE_TO_PARENT},
     * {@link #SLIDE_BORDER_MODE_CYCLE}, default is {@link #SLIDE_BORDER_MODE_NONE}
     */
    void setSlideBorderMode(int32_t slideBorderMode);

    /**
     * whether animating when auto scroll at the last or first item, default is true
     *
     * @return
     */
    bool isBorderAnimation() const;

    /**
     * set whether animating when auto scroll at the last or first item, default is true
     *
     * @param isBorderAnimation
     */
    void setBorderAnimation(bool isBorderAnimation);

private:
    void sendScrollMessage(int64_t delayTimeInMills);

public:
    static const int32_t DEFAULT_INTERVAL; // milliseconds

    enum {
        LEFT,
        RIGHT,
        SLIDE_BORDER_MODE_NONE,          // do nothing when sliding at the last or first item
        SLIDE_BORDER_MODE_CYCLE,         // cycle when sliding at the last or first item
        SLIDE_BORDER_MODE_TO_PARENT,     // deliver event to parent when sliding at the last or first item
    };

private:
    enum {
        SCROLL_WHAT = 0xFF00,
    };
    int64_t mInterval;
    int32_t mDirection;                   // auto scroll direction, default is {@link #RIGHT}
    bool mIsCycle;                        // whether automatic cycle when auto scroll reaching the last or first item, default is true
    bool mStopScrollWhenTouch;            // whether stop auto scroll when touching, default is true
    int32_t mSlideBorderMode;             // how to process when sliding at the last or first item, default is {@link #SLIDE_BORDER_MODE_NONE}
    bool mIsBorderAnimation;              // whether animating when auto scroll at the last or first item
    bool mIsAutoScroll;
    bool mIsStopByTouch;
    float mTouchX;
    float mDownX;
};
