/*
 * Program: lcd_AutoWrap.c
 * Author:  D. O. Corlett
 *
 * Displays a long string of multiple words
 * to the LCD without splitting any words
 * between the lines of the LCD.
 */

#include <util/delay.h>
#include "lcd.h"

// Takes a string to find the first word in it,
// and an array to store the first word. Returns
// the first word found in that string and store it in
// the array.
int GetWordFromString(char string[], int stringSize, int wordNumber, char word[])
{
	int wordLength = 0;			// Stores the length of the word
	int spaceNumber = 0;		// Stores the number of spaces up to the word of wordNumber
	int wordStartIndex = 0;		// The array index of the start of the word
	// find the array index for the start of the word
	for(int h=0; spaceNumber!=wordNumber && (int)string[h]!=0 && h<stringSize; h++)
	{
		if((int)string[h]==' ')
		{
    		spaceNumber++;
       		wordStartIndex = h+1;
    	}
    }

    // loop through the string adding each character to the
    // word until a space occurs or the end of the string,
    // hence until end of word occurs.
    for(int i = wordStartIndex; string[i]!=' ' && (int)string[i]!=0 && i<stringSize; i++)
    {
       	word[i-wordStartIndex] = string[i];
       	wordLength = i-wordStartIndex+1;
    }

    word[wordLength] = 0;	// end string null character
    return wordLength;
}

// Finds the number of words in a string
int NumberOfWords(char string[], int stringSize)
{
	int numberOfWords = 0;

	// loop through the whole string.
	for(int i = 0; i<stringSize; i++)
	{
		// count the word before the space character ' '.
		if((int)string[i] == ' ')
		{
			numberOfWords++;
		}
		// increment count last word in string (the word before the end string character 0)
		if((int)string[i] == 0)
		{
			numberOfWords++;
			break;
		}
	}
	return numberOfWords;
}




int GetWordLength(char string[], int stringSize, int wordNumber)
{
	int wordLength = 0;
	int spaceNumber = 0;
	int wordStartIndex = 0;
	// find the array index for the start of the word
	for(int h=0; spaceNumber!=wordNumber && (int)string[h]!=0 && h<stringSize; h++)
	{
		if((int)string[h]==' ')
		{
	   		spaceNumber++;
	   		wordStartIndex = h+1;
	   	}
	}

	for(int i = wordStartIndex; string[i]!=' ' && (int)string[i]!=0 && i<stringSize; i++)
    {
       	wordLength = i-wordStartIndex+1;
    }
    return wordLength;
}




// Displays a string of words on the LCD without
// splitting words between a line on the display.
void AutoWrap(int stringSize, char string[])
{
	int lineLength = 16;
	int lineNum = 0;
	int linePos = 0;
	char word[64];
	char displayString[lineLength*2];
	int wordLength = 0;
	int displayIndex = 0;
	int wordNum = 0;
	int numWords = 0;

	// Finds the number of words in the string.
	numWords = NumberOfWords(string, stringSize);
	// Increments wordNum to add each word to the displayString
	for(wordNum = 0; wordNum < numWords; wordNum++)
	{
		// finds the length of wordNum and also loads the word into the word array
		wordLength = GetWordFromString(string, stringSize, wordNum, word);

		// Checks if word will fit in current line if not it prints the new line character
		if((linePos + wordLength) > lineLength)
		{
			displayString[displayIndex] = 10;		// load new line character 10 character to displayString
			displayIndex++;							// increment displayIndex
			lineNum++;								// increment line number because new line
			linePos = 0;							// reset line position to 0 because new line
		}

		// loads word wordNum into the display String.
		for(int i = 0; i < wordLength; i++)
		{
			displayString[displayIndex] = word[i];	// load word into displayString
			displayIndex++;							// increment displayIndex
			linePos++;								// increment line position
		}
		// Puts a space after the word that has just been loaded into displayString, unless it is the last word.
		if(wordNum < (numWords - 1))
		{
			displayString[displayIndex] = ' ';		// load space ' ' character to displayString
			displayIndex++;							// increment displayIndex
			linePos++;								// increment line position
		}
	}


	// Displays the string on the LCD
	//displayString[displayIndex] = 0;				// end of string character (not working, will only display once) only an issue when more than 32 characters are in string
	sprintf(displayString, "%s", displayString);	// format displayString as string (include end of string character at end) fixes problem above
	lcd_display(displayString);						// display displayString on LCD

}




int main(void)
{
	// Initialise LCD
    lcd_init(LCD_CTRL_PORT_C | LCD_DATA_PORT_D_C);

    // Variables for strings to be displayed on the LCD
    char string[256] = "AB CDE FGHIJ KLMNOP QRS TUV WQYZ";
    while (1)
    {
    	// Clear the LCD before writing to it
    	lcd_clear();

    	AutoWrap(sizeof(string), string);
    	_delay_ms(1000);


    }

    return 0;
}
