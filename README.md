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
