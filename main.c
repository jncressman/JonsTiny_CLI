/*******************************************************************************
 * COPYRIGHT:
 *    Copyright 2026 Jonathan Cressman 
 *    Licensed under the MIT licence.  See license.txt
 *
 * FILENAME:
 *    main.c
 *
 * PROJECT:
 *    jon's simple CLI for embedded projects
 *
 * FILE DESCRIPTION:
 *    This file is a sample use of cli
 *
 * CREATED BY:
 *    Jonathan Cressman May 16, 2026
 *
 ******************************************************************************/

/*** HEADER FILES TO INCLUDE  ***/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "jon_cli.h"

/*** DEFINES                  ***/
#define MAJOR 1
#define MINOR 0

/*** MACRO ***/
//macro to resolve a #define and then convert it to a string.  Works with __LINE__
#define STRINGIFY(x) STRINGIFY2(x)
#define STRINGIFY2(x) #x

/*** TYPE DEFINITIONS         ***/





/*** MENU VARIABLE DEFINITIONS     ***/
/* 
   format is for each menu - menu item functions
                           - menu items
                           - menu sub menus
*/
/* NOTE !!!!!!
 You will likely want to put all menus, menu items and menu function prototypes
 in the same .c file.  Other wise you may find yourself with circular dependencies
*/

/**  sample menu    **/
bool displayTime(char *commandLine, int len);

const struct MenuItem_s displayTimeMenuItem =
{
   "t",
   "display the current time",
   "this command takes no arguments",
   &displayTime
};

const struct MenuItem_s *sampleSubmenuItems[] = 
{
   &displayTimeMenuItem,
   NULL
};


const struct Menu_s sampleSubmenu =
{
   "Sample submenu title",
   "S",
   "Sample submenu", 
   NULL,
   sampleSubmenuItems
};



/**  root menu  **/
bool sumIntegers(char *commandLine, int len);
bool echo(char *commandLine, int len);
bool exitProgram(char *commandLine, int len);



const struct MenuItem_s echoMenuItem = 
{
   "e",
   "echos back the entire command line",
   "", //can't fail
   echo
};

const struct MenuItem_s sumIntegersMenuItem = 
{
   "s",
   "sum a number of integers",
   "format s {int, ...}",
   sumIntegers
};

const struct MenuItem_s exitMenuItem = 
{
   "x",
   "eXit the program",
   "",  //can't fail
   exitProgram
};


const struct Menu_s *rootSubmenus[] =
{
   &sampleSubmenu,
   NULL   
};



const struct MenuItem_s *rootMenuItems[] =
{
   &sumIntegersMenuItem,
   &echoMenuItem,
   &exitMenuItem,
   NULL   
};



const struct Menu_s rootMenu =
{
   "Sample CLI Program \n" "created "  __DATE__  "\nversion " STRINGIFY(MAJOR) "." STRINGIFY(MINOR) "\nby Jonathan Cressman",
   ROOT_MENU_KEY,
   "", //this is never a menu option
   rootSubmenus,
   rootMenuItems
};
   

/*** FUNCTIONS                ***/
/******************************************************************************
 * NAME:
 *    cliPrint
 *
 * SYNOPSIS:
 *    void cliPrint(const char *buffer, int len)
 * 
  * PARAMETERS:
 *    INPUT char *buff, int len -- describe the buffer to print 
 *
 * FUNCTION:
 *    prints a buffer to the cli console.  This function must be provided to the CLI
 *    
 * RETURNS:
 *    void
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
void cliPrint(const char *buffer, int len)
{
   int i;
   for (i=0;i<len;i++)
   {
      putchar(buffer[i]);
   }
}

/******************************************************************************
 * NAME:
 *    exitProgram
 *
 * SYNOPSIS:
 *    bool exitProgram(char *commandLine, int len)
 * 
 * This is a menuItem_s bool (*ptr_menuItemFunction)(char *, int) function
 *
 * PARAMETERS:
 *    not used 
 *
 * FUNCTION:
 *    exits the program    
 *
 * RETURNS:
 *    void
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
bool exitProgram(char *commandLine, int len)
{
   exit(0);
}

/******************************************************************************
 * NAME:
 *    exitProgram
 *
 * SYNOPSIS:
 *    bool exitProgram(char *commandLine, int len)
 * 
 * This is a menuItem_s bool (*ptr_menuItemFunction)(char *, int) function
 *
 * PARAMETERS:
 *    not used 
 *
 * FUNCTION:
 *    prints the current time    
 *
 * RETURNS:
 *    true
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
bool displayTime(char *commandLine, int len)
{ 
   time_t now;
   
   char outBuff[30];

   int tokenLen;
   char *nextToken;

   tokenLen = getSpaceDelimitedToken(commandLine, commandLine + len, &nextToken);
   
   if (tokenLen != 1)  //warning this check fails if we change the length of the menu item key
   {
      return false;
   }

   now = time(NULL);

   // Get readable string
   ctime_r(&now, outBuff);
   cliPrintString("the current time is ");
   cliPrintLine(outBuff);

   return true;
}

/******************************************************************************
 * NAME:
 *    buffToInt
 *
 * SYNOPSIS:
 *    bool buffToInt(char *buff, int len, int *accumulator)
 * 
 * PARAMETERS:
 *    INPUT char *buff, int len -- describe the buffer to parse
 *    OUTPUT int *accumulator -- value of the token
 *
 * FUNCTION:
 *    converts an ascii base 10 number in buff of length len to an int    
 *
 * RETURNS:
 *    true if all command line parameters are integers
 *    false if it is unable to parse any of the parameters
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
bool buffToInt(char *buff, int len, int *accumulator)
{
   
   int i;

   *accumulator = 0;

   for (i=0; i<len; i++)
   {
      *accumulator*=10;
      if (buff[i] >= '0' && buff[i] <= '9')
      {
         *accumulator += buff[i] - '0';
      }
      else
      {
         return false;
      }
   }

   return true;
   
}

/******************************************************************************
 * NAME:
 *    sumIntegers
 *
 * SYNOPSIS:
 *    bool sumIntegers(char *commandLine, int len)
 * 
 * This is a menuItem_s bool (*ptr_menuItemFunction)(char *, int) function
 *
 * PARAMETERS:
 *    INPUT
 *        char *commandLine, int len -- describe the command line that will be parsed
 *
 * FUNCTION:
 *    parses the command line for integers and sums them    
 *
 * RETURNS:
 *    true if all command line parameters are integers
 *    false if it is unable to parse any of the parameters
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
bool sumIntegers(char *commandLine, int len)
{
   int tokenLen;
   char *token;
   char *commandLineEnd = commandLine + len;

   int sum = 0;
   int tokenValue;

   char outString[30];

   tokenLen = getSpaceDelimitedToken(commandLine, commandLineEnd, &token);

   //nextToken now points to the menu key
 

   tokenLen = getSpaceDelimitedToken(token + tokenLen, commandLineEnd, &token);

   while (tokenLen > 0)
   {
      if (buffToInt(token, tokenLen, &tokenValue) == false)
      {
         return false;
      }

      sum += tokenValue;

      tokenLen = getSpaceDelimitedToken(token + tokenLen, commandLineEnd, &token);
   }

   snprintf(outString, sizeof(outString), "the sum is %d", sum);
   cliPrintLine(outString);
   

   return true;
}

/******************************************************************************
 * NAME:
 *    echo
 *
 * SYNOPSIS:
 *    bool echo(char *commandLine, int len)
 * 
 * This is a menuItem_s bool (*ptr_menuItemFunction)(char *, int) function
 *
 * PARAMETERS:
 *    INPUT
 *        char *commandLine, int len -- describe the command line 
 *
 * FUNCTION:
 *    echos back the input command line    
 *
 * RETURNS:
 *    true 
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
bool echo(char *commandLine, int len)
{
   cliPrint(commandLine, len);
   cliPrint("\n",1);
   return true;
}

/******************************************************************************
 * NAME:
 *    main
 *
 * SYNOPSIS:
 *    void main(void)
 * 
 * PARAMETERS: none
 *
 * FUNCTION:
 *    main entry point of the program.  Initializes the CLI and then loops
 * processing characters entered
 *
 * RETURNS:
 *    0 on the CLI exit command 
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
int main()
{
   cliInit(&rootMenu);

#pragma warn(push)
#pragma warn(disable:2134)
   for(;;)
   {
      cliProcessChar((char)getchar());
   }
#pragma warn(pop)
}
