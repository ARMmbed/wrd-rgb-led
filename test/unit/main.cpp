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

#include "mbed-drivers/mbed.h"

#include "wrd-rgb-led/RGBLED.h"

RGBLED rgb;

#define DELAY 2

void rotate()
{
    printf("rotate!\r\n");

    /** red
    */
    for (size_t up = 1; up < 255; up++)
    {
        rgb.set(up, 0, 0, DELAY);
    }

    for (size_t down = 255; down > 0; down--)
    {
        rgb.set(down, 0, 0, DELAY);
    }

    /** green
     */
    for (size_t up = 1; up < 255; up++)
    {
        rgb.set(0, up, 0, DELAY);
    }

    for (size_t down = 255; down > 0; down--)
    {
        rgb.set(0, down, 0, DELAY);
    }

    /** blue
    */
    for (size_t up = 1; up < 255; up++)
    {
        rgb.set(0, 0, up, DELAY);
    }

    for (size_t down = 255; down > 0; down--)
    {
        rgb.set(0, 0, down, DELAY);
    }

    rgb.set(0, 0, 1, DELAY, rotate);
}

void app_start(int, char *[])
{
    rotate();
}
