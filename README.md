# vex-recorder
ROBOTC library for generating an autonomous program from motor movements.

### Usage

Include `vex_recorder.c` in your ROBOTC program, and add a `startTask` call to
your main task:

```c
#include <vex_recorder.c>
...

task main()
{
    startTask(record_autonomous);
    ....
}
```
Note: If you add a `startTask` call to your main task like this, the timer will start counting down as soon as the "start" button is pressed in the debug window. If you want to make it so it only starts recording after a button press on the controller, use the following code in your ROBOTC program instead:

```c
#include <vex_recorder.c>
...

task main()
{
    bool startedRecording = false;
    while (true) {
        if(!startedRecording && vexRT[SOMEBTN]) {
            startedRecording = true;
            startTask(record_autonomous);
        }
        ....
    }
}
