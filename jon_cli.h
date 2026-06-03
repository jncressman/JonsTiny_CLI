/*******************************************************************************
 * COPYRIGHT:
 *    Copyright 2026 Jonathan Cressman 
 *    Licensed under the MIT licence.  See license.txt
 *
 * FILENAME:
 *    jon_cli.h
 *
 * PROJECT:
 *    jon's simple CLI for embedded projects
 *
 * FILE DESCRIPTION:
 *        This file provides the interface for the CLI. The cli expects the user to implement
 *    two functions - cliPrint() and if the terminal doesn't echo characters cliEcho()
 * 
 *    Use - cliInit() must be called before using the cli. 
 *
 *        - cliProcessChar() must be called for each character entered
 *    Features - simple to create menus
 *             - minimal library functions - doesn't even need printf
 *
 *    Future enhancements - command history/recall
 *                        - command editing, currently only supports backspace
 *                        - standardized command parameter language to be displayed in help          
 *
 *    Setup/coding menus - each menu is a struct Menu_s
 *                       - the menu contains NULL terminated arrays of Menu_s and MenuItem_s
 *                       - MenuItem_s contain the functions that parse the commandline to perform
 *                         their particular command
 *
 *
 *                        **** EXAMPLE MENU DECLARATION ***    
                              bool exitProgram(char *commandLine, int len);

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
                                 &exitMenuItem,
                                 NULL   
                              };

                              const struct Menu_s rootMenu =
                              {
                                 "root menu",
                                 ROOT_MENU_KEY,
                                 "", //key would go here if this was a submenu
                                 rootSubmenus,
                                 rootMenuItems
                              };
  
 *
 *  
 *
 * CREATED BY:
 *    Jonathan Cressman May 16, 2026
 *
 ******************************************************************************/
#ifndef __JON_CLI_H_
#define __JON_CLI_H_

/***  HEADER FILES TO INCLUDE          ***/
#include <stdbool.h>

/***  DEFINES                          ***/
#define ROOT_MENU_KEY      "0"
#define UP_ONE_MENU_KEY    "-"
#define KEY_FIELD_SIZE     4           //used to align the menu descriptions after the key
#define MAX_MENU_STACK_DEPTH 10        //maximum level of submenus
#define MAX_ENTERED_LINE_LENGTH 80     //maximum number of characters that can be parsed

/***  MACROS                           ***/

/***  TYPE DEFINITIONS                 ***/
struct MenuItem_s
{
   const char *key;  //menu item to display to select this item
	const char *description; //description of this item and any arguments it will take;
	const char *help; //further help for this menu item, usually used to describe arguments
	bool (*ptr_menuItemFunction)(char *, int); /*ptr_menuItemFunction called when the key is entered
                                                               will be passed the entire entered line including the key
                                                               should return true on success, on false the 
                                                               help string will be printed */
                  
	
};

struct Menu_s
{
	const char *title;    //will be displayed when the menu is displayed
	const char *key;		//menu item to display this menu when it is a menu item in another menu
	const char *description; // description of the menu when it is a menu item in another menu
	const struct Menu_s **subMenus;  //sub menus of this menu, list is terminated with a NULL
	const struct MenuItem_s **menuItems; //menu items of this menu, list is terminated with a NULL
};


/***  GLOBAL VARIABLE DEFINITIONS      ***/

/***  EXTERNAL FUNCTION PROTOTYPES     ***/
/** Functions to be implemented by the user of the CLI **/
void cliPrint(const char *buffer, int len);
void cliEcho(char character); //echos the input if the terminal doesn't already do this



/** Functions called by user of CLI **/
void cliInit(const struct Menu_s *rootMenu);
void cliProcessChar(char character);

/** Useful functions that will aid in creating menu functions **/
int getSpaceDelimitedToken(char *buffer, char *buffEnd, char **tokenStart);  /* helper function to parse lines
                                                                                       safer than strtok */
void cliPrintString(const char *buffer); ////wrapper function of cliPrint to print a null terminated string
void cliPrintLine(const char *buffer); //wrapper function of cliPrint to print a null terminated string with a \n
void cliPrintkey(const char *buffer); //function that prints the key in a fixed size field, this aligns the descriptions


#endif   /* end of "#ifndef __ _H_" */
