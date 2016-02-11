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

#ifndef __WRD_RGB_LED_NOT_PRESENT_H__
#define __WRD_RGB_LED_NOT_PRESENT_H__

#include "mbed-drivers/mbed.h"

using namespace mbed::util;

class RGBLEDNotPresent
{
public:
    /**
     * @brief Constructor. Implementation of the WRD RGB API.
     * @details Dummy constructor.
     */
    RGBLEDNotPresent()
    { }

    /**
     * @brief Set RGB color.
     * @details Dummy function.
     */
    void set(uint8_t red,
             uint8_t green,
             uint8_t blue,
             FunctionPointer0<void> callback = (void(*)(void)) NULL)
    {
        (void) red;
        (void) green;
        (void) blue;

        if (callback)
        {
            minar::Scheduler::postCallback(callback);
        }
    }
};

#endif // __WRD_RGB_LED_NOT_PRESENT_H__
