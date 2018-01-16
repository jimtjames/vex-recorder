/*
 * vex-recoder
 * ROBOTC library for generating an autonomous program from motor movements
 *
 * Copyright (C) 2018 Alex James (TheRacerMaster). All rights reserved.
 * Copyright (C) 2018 Jim James (KingRandomGuy). All rights reserved.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 */

#ifndef VEX_RECORDER_H_
#define VEX_RECORDER_H_

/* Open the debug stream window in ROBOTC when the program starts up. */
#pragma DebuggerWindows("debugStream")

/* Number of motors on the VEX Cortex. */
#define NUM_MOTORS          10
/* Length of the autonomous period (ms). Maximum length is 32767 ms.*/
#define AUTONOMOUS_TIME     15000
/* Minimum total motor value difference needed to trigger a state change. */
#define MIN_MOTOR_VAL_DIFF  10
/* Name of the task in the generated program. */
#define TASK_NAME           "main"

/*
 * Records motor movements to generate an autonomous program (printed to the
 * debug stream window).
 */
task record_autonomous()
{
    /* Previously stored motor values. */
    byte  previous_motor_vals[NUM_MOTORS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    /* Current motor values. */
    byte current_motor_vals[NUM_MOTORS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    /* Last time a state change occurred. */
    unsigned short last_state_change = 0;
    /* Current time. */
    unsigned short current_time = 0;
    /* Calculated total motor value difference. */
    unsigned int diff = 0;
    /* Prevents sleep from being the first line in the autonomous program. */
    bool first = true;

    /* Clear the debug stream buffer and reset the timer. */
    clearDebugStream();
    clearTimer(T1);

    /* Print the start of the program. */
    writeDebugStreamLine("task %s()\n{", TASK_NAME);

    /* Only record movements during the time allowed for the autonomous period. */
    while (time1[T1] < AUTONOMOUS_TIME)
    {
        /* Reset the total motor difference to 0. */
        diff = 0;

        /* Copy the existing data in current_motor_vals to previous_motor_vals. */
        for (int i = 0; i < NUM_MOTORS; ++i)
        {
            previous_motor_vals[i] = current_motor_vals[i];
        }

        /* Update current_motor_vals with the current motor values. */
        current_motor_vals[0] = motor[port1];
        current_motor_vals[1] = motor[port2];
        current_motor_vals[2] = motor[port3];
        current_motor_vals[3] = motor[port4];
        current_motor_vals[4] = motor[port5];
        current_motor_vals[5] = motor[port6];
        current_motor_vals[6] = motor[port7];
        current_motor_vals[7] = motor[port8];
        current_motor_vals[8] = motor[port9];
        current_motor_vals[9] = motor[port10];

        /*
         * Calculate the total difference between the current and previous
         * motor values.
         */
        for (int i = 0; i < NUM_MOTORS; ++i)
        {
            diff += abs(current_motor_vals[i] - previous_motor_vals[i]);
        }

        /* Ignore minimal motor value differences. */
        if (diff < MIN_MOTOR_VAL_DIFF)
        {
            continue;
        }

        /* Save the current time. */
        current_time = time1[T1];

        /*
         * Print out the generated lines to the debug stream.
         * Do not put a sleep statement on the first line of the
         * autonomous function.
         */
        if (!first)
        {
            writeDebugStreamLine("\tsleep(%d);",
            current_time - last_state_change);
        }

        first = false;
        for (int i = 0; i < NUM_MOTORS; ++i)
        {
            /* Only print out a statement if a motor's value has changed. */
            if (current_motor_vals[i] != previous_motor_vals[i])
            {
                writeDebugStreamLine("\tmotor[port%d] = %d;", i + 1,
                                     current_motor_vals[i]);
            }
        }

        /* Save the last time the motor was updated. */
        last_state_change = current_time;
    }

    /* Reset non-zero motor values to zero at the end of the autonomous program. */
    for (int i = 0; i < NUM_MOTORS; ++i)
    {
        if (current_motor_vals[i] != 0)
        {
            writeDebugStreamLine("\tmotor[port%d] = 0;", i + 1);
        }
    }

    writeDebugStream("}");
}

#endif // VEX_RECORDER_H_
