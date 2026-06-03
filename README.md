# Jon's Tiny Command Line Interface (CLI)

This CLI is meant for small, constrained embedded devices.  It is expected the developer will have a serial interface such as a UART or a virtual COM that will accept bytes from the user and send responses back.
This CLI implementation uses only <stdbool.h> and <string.h>

## To use
Include the jon_cli.h and jon_cli.c in your project.  jon_cli.h describes the interface while jon_cli.c has the implemtation.
The cli expects the user to implement one (two) functions - cliPrint() and if the terminal doesn't echo characters cliEcho()
Use - cliInit() must be called before using the cli. 
    - cliProcessChar() must be called for each character entered by the user

## Example project
main.c contains a sample project that uses the CLI
jons_small_cli.ppj is a [pelles C](https://www.pellesc.se/) project that can be used to easily compile and run the project on different platforms 
