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

#include "wrd-rgb-led/RGBLED.h"

RGBLED::RGBLED(void)
    :   processQueueHandle(NULL),
        led()
{
}

RGBLED::~RGBLED(void)
{
    if (processQueueHandle)
    {
        minar::Scheduler::cancelCallback(processQueueHandle);
    }
}

RGBLED::LEDAdder::LEDAdder(RGBLED* _owner)
    :   owner(_owner)
{}

const RGBLED::LEDAdder& RGBLED::LEDAdder::operator=(const RGBLED::LEDAdder& adder)
{
    owner = adder.owner;

    return *this;
}

RGBLED::LEDAdder::LEDAdder(const LEDAdder& adder)
{
    *this = adder;
}

RGBLED::LEDAdder& RGBLED::LEDAdder::set(uint8_t red, uint8_t green, uint8_t blue, uint32_t duration)
{
    /* queue action using non-fluent API */
    owner->addQueue(red, green, blue, duration);

    return *this;
}

RGBLED::LEDAdder RGBLED::set(uint8_t red, uint8_t green, uint8_t blue, uint32_t duration)
{
    RGBLED::LEDAdder adder(this);

    addQueue(red, green, blue, duration);

    return adder;
}

void RGBLED::addQueue(uint8_t red, uint8_t green, uint8_t blue, uint32_t duration)
{
    /* add command to queue */
    RGBLED::transaction_t action;
    action.red = red;
    action.green = green;
    action.blue = blue;
    action.duration = duration;

    sendQueue.push(action);

    /* schedule queue to be processed */
    if (processQueueHandle == NULL)
    {
        processQueueHandle = minar::Scheduler::postCallback(this, &RGBLED::processQueue)
                                .tolerance(1)
                                .getHandle();
    }
}

void RGBLED::processQueue(void)
{
    /* clear task handle */
    processQueueHandle = NULL;

    // only process if queue is not empty
    if (sendQueue.size() > 0)
    {
        /* get next action */
        RGBLED::transaction_t action = sendQueue.front();

        /* use RGB LED implementation to set colors */
        FunctionPointer0<void> fp(this, &RGBLED::setDone);
        led.set(action.red, action.green, action.blue, fp);
    }
    else
    {
        /* turn off LED */
        FunctionPointer0<void> fp(this, &RGBLED::setDone);
        led.set(0, 0, 0);
    }
}

void RGBLED::setDone(void)
{
    /* remove action from queue */
    RGBLED::transaction_t action = sendQueue.front();
    sendQueue.pop();

    /* schedule next action to be processed based on the duration of this action */
    if (action.duration != ULONG_MAX)
    {
        processQueueHandle = minar::Scheduler::postCallback(this, &RGBLED::processQueue)
                                .tolerance(1)
                                .delay(minar::milliseconds(action.duration))
                                .getHandle();
    }
    else
    {
        /* if no duration was set and the queue is not empty, process next action
           immediately, otherwise leave LED in current state. */
        if (sendQueue.size() > 0)
        {
            processQueueHandle = minar::Scheduler::postCallback(this, &RGBLED::processQueue)
                                    .tolerance(1)
                                    .getHandle();
        }
    }
}
