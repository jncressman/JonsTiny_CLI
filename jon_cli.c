/*******************************************************************************
 * COPYRIGHT:
 *    Copyright 2026 Jonathan Cressman 
 *    Licensed under the MIT licence.  See license.txt
 *
 * FILENAME:
 *    jon_cli.c
 *
 * PROJECT:
 *    jon's simple CLI for embedded projects
 *
 * FILE DESCRIPTION:
 *    This file implements the cli
 *
 * CREATED BY:
 *    Jonathan Cressman May 16, 2026
 *
 ******************************************************************************/

/*** HEADER FILES TO INCLUDE  ***/
#include <string.h>

#include "jon_cli.h"

/*** VARIABLE DEFINITIONS     ***/
static const struct Menu_s *menuStack[MAX_MENU_STACK_DEPTH];  //used to go up one menu
static int menuDepth;  //index of the current menu in the stack

/*** FUNCTION PROTOTYPES      ***/
static void processLine(char *buffer, int len);
static void displayCurrentMenu(void);
static bool keyMatch         (char *enteredKey, int enteredKeyLen, const char *menuKey);

/*** FUNCTIONS                ***/
/******************************************************************************
 * NAME:
 *    cliInit
 *
 * SYNOPSIS:
 *    void cliInit(const struct Menu_s *ptr_rootMenu)
 *
 * PARAMETERS:
 *    INPUT 
 *        const struct Menu_s *ptr_rootMenu -- pointer to the root menu of the cli
 *
 * FUNCTION:
 *    Initializes the cli with the root menu    
 *
 * RETURNS:
 *    void
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
void cliInit(const struct Menu_s *ptr_rootMenu)
{
   menuStack[0]= ptr_rootMenu;
   menuDepth = 0;

   displayCurrentMenu();
}

/******************************************************************************
 * NAME:
 *    cliProcessChar
 *
 * SYNOPSIS:
 *    void cliProcessChar(char character)
 *
 * PARAMETERS:
 *    INPUT 
 *        char character -- character recieved by from the terminal for processing
 *
 * FUNCTION:
 *    Buffers a character from the user input.  Once an end of line is recieved the 
 * buffer will be parsed and executed
 *
 * RETURNS:
 *    void
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
void cliProcessChar(char character)
{
   static int len;
   static char buffer[MAX_ENTERED_LINE_LENGTH];
   
   if (character == 0x0D)  //windows charrige return
   {
      //do nothing, ignore
   }
   
   else if (character == 0x0A)  //line feed/end of line process the commandline
   {
      processLine(buffer, len);
      len = 0;
   }

   /*
   Not implemented yet
   
   else if (character == 0x26) //up arrowused to recall previous commands
   {
      loadAndShowPreviousCommand();
   }
   */

   else if (character == 0x08) //backspace
   {
      if (len >0)
      {
         len--;
      }
   }

   else  //add the character to the command buffer if there is room
   {
      if (len < (MAX_ENTERED_LINE_LENGTH - 1) )
      {
         buffer[len] = character;
         len++;
      }
   }
}

 
/******************************************************************************
 * NAME:
 *    cliEcho
 *
 * SYNOPSIS:
 *    void cliEcho(char character)
 *
 * PARAMETERS:
 *    INPUT 
 *        char character -- character to echo back
 *
 * FUNCTION:
 *    Does nothing.  If characters need to be echoed back this function
 *  should be overwritten with a function to print the character back
 *
 * RETURNS:
 *    void
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/ 
/*__weak */ void cliEcho(char character)
{
   ;  //this implementation does nothing.  If it is needed the user can override the function 
}


/*** helper functions ***/
/******************************************************************************
 * NAME:
 *    getSpaceDelimitedToken
 *
 * SYNOPSIS:
 *    int getSpaceDelimitedToken(char *buffer, char *buffEnd, char **tokenStart)
 *
 * PARAMETERS:
 *    INPUT 
 *        char *buffer -- pointer to buffer to parse
 *        char *buffEnd -- pointer to the byte after the buffer (i.e. equals buffer + buf length)
 *    OUTPUT
 *        char **tokenStart -- pointer to the pointer the start of the found token
 *
 * FUNCTION:
 *    finds the next token in a buffer that is delimited by a space or the end of 
 * buffer
 *
 * RETURNS:
 *    zero if no more tokens are found or the lenght of the token
 *
 * EXAMPLE:
               commandLineEnd = commandLine + commandLen;
               tokenLen = getSpaceDelimitedToken(commandLine, commandLineEnd, &token);
               while (tokenLen > 0)
               {
                  //do something with token

                  tokenLen = getSpaceDelimitedToken(token + tokenLen, commandLineEnd, &token);
               }
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
int getSpaceDelimitedToken(char *buffer, char *buffEnd, char **tokenStart)
{
   int len;

   *tokenStart = buffer;

   //skip any initial delimiters
   while ( (*tokenStart < buffEnd)
          &&(**tokenStart == ' ') )
   {  
      (*tokenStart)++;
   }
   

   len = 0;
   while ( ((*tokenStart + len) < buffEnd)
          &&( *(*tokenStart + len) != ' ') )
   {  
      len++;
   }
   return len;
}

/** module functions   **/
/******************************************************************************
 * NAME:
 *    keyMatch
 *
 * SYNOPSIS:
 *    bool keyMatch(char *enteredKey, int enteredKeyLen, const char *menuKey)
 *
 * PARAMETERS:
 *    INPUT 
 *        char *enteredKey     -- buffer containing the entered command line key
 *        int enteredKeyLen    -- length of the entered key
 *        const char *menuKey  -- NULL terminated string to match against
 *
 * FUNCTION:
 *    Buffers a character from the user input.  Once an end of line is recieved the 
 * buffer will be parsed and executed
 *
 * RETURNS:
 *    true if the keys match, false if they don't 
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
static bool keyMatch(char *enteredKey, int enteredKeyLen, const char *menuKey)
{
   if (enteredKeyLen != strlen(menuKey) )
   {
      return false;
   }
   
   if (memcmp(enteredKey, menuKey, enteredKeyLen) != 0)
   {
      return false;
   }
   
   return true;
}

/******************************************************************************
 * NAME:
 *    processLine
 *
 * SYNOPSIS:
 *    void  processLine(char *buffer, int len)
 *
 * PARAMETERS:
 *    INPUT 
 *        char *buffer -- buffer containing the entered command line 
 *        int len      -- length of the entered line
 *
 * FUNCTION:
 *    parses the entered command line based on the current menu
 *
 * RETURNS:
 *    void
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
static void  processLine(char *buffer, int len)
{
   int keyLen;  //length of the first token (key) in the command entered
   int index;   //index of the sub menu or menu item we are matching against
   char *key;

   keyLen = getSpaceDelimitedToken(buffer, buffer + len, &key);

   //check against the common menu items 
   //these could be put in an array and parsed like the other submenus and menu items
   //but there are currently only 2 
   if (keyMatch(key, keyLen, ROOT_MENU_KEY))  //check if the command is go to root menu
   {
      //goto the root menu
      menuDepth = 0;  
      displayCurrentMenu();
      return;
   }
   
   if (keyMatch(key, keyLen, UP_ONE_MENU_KEY))  //check if the command is go up one menu
   {
      //goto up one menu
      if (menuDepth > 0)
      {
         menuDepth--;
      }  
      displayCurrentMenu();

      return;
   }
   
   //check the key against the submenus
   if (menuStack[menuDepth]->subMenus != NULL)
   {
      index = 0;

      while (menuStack[menuDepth]->subMenus[index] != NULL)
      {
         if (keyMatch(key, keyLen, menuStack[menuDepth]->subMenus[index]->key))
         {  
            //goto the submenu if we have stack space
            if (menuDepth < (MAX_MENU_STACK_DEPTH - 1) )
            {
               menuStack[menuDepth+1] = menuStack[menuDepth]->subMenus[index];
               menuDepth++;            
            }

            displayCurrentMenu();
            return;
         }
         index++;
      }
   }

   //check the key against the menu items
   if (menuStack[menuDepth]->menuItems != NULL)
   {
      index = 0;
      while (menuStack[menuDepth]->menuItems[index] != NULL)
      {
         if(keyMatch(key, keyLen, menuStack[menuDepth]->menuItems[index]->key))
         {  
            //call the menu item command
            if (menuStack[menuDepth]->menuItems[index]->ptr_menuItemFunction(buffer, len) == false)
            {
               //print the menuItem help if the command failed
               cliPrintLine(menuStack[menuDepth]->menuItems[index]->help);
            }
            
            return;
         }
         index++;
      }
   }

   //key didn't match against anything
   displayCurrentMenu();
}

/******************************************************************************
 * NAME:
 *    cliPrintString
 *
 * SYNOPSIS:
 *    void  cliPrintString(const char *buffer)
 *
 * PARAMETERS:
 *    INPUT 
 *        char *buffer -- buffer containing a string to print
 *
 * FUNCTION:
 *    helper function that will print the string as is
 *
 * RETURNS:
 *    void
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
void cliPrintString(const char *buffer)
{
   cliPrint(buffer, strlen(buffer)); 
}

/******************************************************************************
 * NAME:
 *    cliPrintLine
 *
 * SYNOPSIS:
 *    void  cliPrintLine(const char *buffer)
 *
 * PARAMETERS:
 *    INPUT 
 *        char *buffer -- buffer containing a string to print
 *
 * FUNCTION:
 *    helper function that will print the string and then a carriage return 
 *
 * RETURNS:
 *    void
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
void cliPrintLine(const char *buffer)
{
   cliPrintString(buffer);
   cliPrint("\n", 1); 
}

/******************************************************************************
 * NAME:
 *    cliPrintKey
 *
 * SYNOPSIS:
 *    void cliPrintKey(const char *buffer)
 *
 * PARAMETERS:
 *    INPUT 
 *        char *buffer -- buffer containing a key
 *
 * FUNCTION:
 *    helper function that will print the key and then pad with spaces to KEY_FIELD_SIZE
 *
 * RETURNS:
 *    void
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
void cliPrintKey(const char *buffer)
{
   int len = strlen(buffer);
   cliPrint(" ", 1);
   cliPrint(buffer, len);
   cliPrint(" ", 1);  
   
   //calculate how many more spaces to print so that the key field is KEY_FIELD_SIZE
   len = KEY_FIELD_SIZE - len - 2;
   if (len >0)
   {
      while (len)
      {
         cliPrint(" ", 1);
         len--;
      }
   }
}

/******************************************************************************
 * NAME:
 *    displayCurrentMenu
 *
 * SYNOPSIS:
 *    void displayCurrentMenu(void)
 *
 * PARAMETERS: none
 *
 * FUNCTION:
 *    displays the current menu title, default options, sub menus and menu items
 *
 * RETURNS:
 *    void
 *
 * CREATED 
 *    Jonathan Cressman May 26, 2026
 *
 ******************************************************************************/
static void displayCurrentMenu(void)
{
   const char menuHeader[] = "*****************";
   const char menuFixedOptions [] = " "ROOT_MENU_KEY"  root menu\n "UP_ONE_MENU_KEY" previous menu \n";

   int index;
 
   cliPrintLine(menuHeader);
   cliPrintLine(menuStack[menuDepth]->title);
   cliPrintLine(menuHeader);
   cliPrintLine(menuFixedOptions);

   //print the sub menus
   if (menuStack[menuDepth]->subMenus != NULL)
   {
      index = 0;

      while (menuStack[menuDepth]->subMenus[index] != NULL)
      {
         cliPrintKey(menuStack[menuDepth]->subMenus[index]->key);

         cliPrintLine(menuStack[menuDepth]->subMenus[index]->description);
         index++;
      }
   }

   //print the menu items
   if (menuStack[menuDepth]->menuItems != NULL)
   {
      index = 0;
      while (menuStack[menuDepth]->menuItems[index] != NULL)
      {
         cliPrintKey(menuStack[menuDepth]->menuItems[index]->key);
         cliPrintLine(menuStack[menuDepth]->menuItems[index]->description);
         index++;
      }
   }
}

