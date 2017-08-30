/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <signal.h>

#include "at42qt1070.hpp"
#include "upm_utilities.h"

using namespace std;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

void
printButtons(upm::AT42QT1070& touch)
{
    bool buttonPressed = false;
    uint8_t buttons = touch.getButtons();

    cout << "Buttons Pressed: ";
    for (int i = 0; i < 7; i++) {
        if (buttons & (1 << i)) {
            cout << i << " ";
            buttonPressed = true;
        }
    }

    if (!buttonPressed)
        cout << "None";

    cout << endl;

    if (touch.isCalibrating())
        cout << "Calibration is occurring." << endl;

    if (touch.isOverflowed())
        cout << "Overflow was detected." << endl;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate an AT42QT1070 on I2C bus 0

    upm::AT42QT1070 touch(AT42QT1070_I2C_BUS, AT42QT1070_DEFAULT_I2C_ADDR);

    while (shouldRun) {
        touch.updateState();
        printButtons(touch);
        upm_delay_us(100000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
