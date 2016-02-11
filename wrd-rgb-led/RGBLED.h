/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __WRD_RGB_LED_H__
#define __WRD_RGB_LED_H__

#include "mbed-drivers/mbed.h"

#if YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_RGB_LED_PRESENT
#include "wrd-rgb-led/RGBLEDImplementation.h"
#else
#include "wrd-rgb-led/RGBLEDNotPresent.h"
#endif

#include <queue>
#include <limits.h>

class RGBLED
{
public:
    /**
     * @brief RGB LED constructor.
     */
    RGBLED(void);

    /**
     * @brief Destructor.
     */
    ~RGBLED(void);

    /**
     * @brief Helper class for providing a fluent API.
     */
    class LEDAdder
    {
        friend RGBLED;
    public:
        /**
         * @brief Set RGB color value.
         * @details The duration in milliseconds is optional.
         * @param red The red color intensity in [0;255]
         * @param green The green color intensity in [0;255]
         * @param blue The blue color intensity in [0;255]
         * @param duration The time in milliseconds the LED is on.
         *                 Setting this to ULONG_MAX means forever.
         * @return LEDAdder reference for a fluent API.
         */
        LEDAdder& set(uint8_t red, uint8_t green, uint8_t blue, uint32_t duration = ULONG_MAX);

    private:
        /**
         * @brief Constructor for the LEDAdder helper class.
         * @param owner Pointer to the RGBLED the settings apply to.
         */
        LEDAdder(RGBLED* owner);

        /**
         * @brief Assignment operator.
         * @param a LEDAdder source.
         */
        const LEDAdder& operator=(const LEDAdder& a);

        /**
         * @brief Copy constructor.
         * @param a LEDAdder source.
         */
        LEDAdder(const LEDAdder& a);

        RGBLED* owner;
    };

    /**
     * @brief Set RGB color value.
     * @details The duration in milliseconds is optional.
     * @param red The red color intensity in [0;255]
     * @param green The green color intensity in [0;255]
     * @param blue The blue color intensity in [0;255]
     * @param duration The time in milliseconds the LED is on.
     *                 Setting this to ULONG_MAX means forever.
     * @return LEDAdder reference for a fluent API.
     */
    LEDAdder set(uint8_t red, uint8_t green, uint8_t blue, uint32_t duration = ULONG_MAX);

    /**
     * @brief Set RGB color value.
     * @details Commands are queued so vibration sequences can be grouped together.
     * @        The duration in milliseconds is optional.
     * @param red The red color intensity in [0;255]
     * @param green The green color intensity in [0;255]
     * @param blue The blue color intensity in [0;255]
     * @param duration The time in milliseconds the LED is on.
     *                 Setting this to ULONG_MAX means forever.
     * @return LEDAdder reference for a fluent API.
     */
    void addQueue(uint8_t red, uint8_t green, uint8_t blue, uint32_t duration = ULONG_MAX);

private:
    /* disable assignment and copy constructor */
    RGBLED(const RGBLED&);
    RGBLED& operator=(const RGBLED&);

    /* callback function for when LEDs have been set. */
    void setDone(void);

    typedef struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint32_t duration;
    } transaction_t;

    void processQueue(void);
    minar::callback_handle_t processQueueHandle;
    std::queue<transaction_t> sendQueue;

#if YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_RGB_LED_PRESENT
    RGBLEDImplementation led;
#else
    RGBLEDNotPresent led;
#endif
};

#endif // __WRD_RGB_LED_H__
