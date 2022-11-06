#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning (disable : 4996)
void control();

int stringCounter = 0;

char tempChar, tempString[1000] = { '\0' };
FILE *file, *outputFile;

int integerArray[2] = { 0,0 };
char operatorsArray[7][3] = { "+","-","*","++","--",":=","/" };
char keywordsArray[18][10] = { "break","case","char","const","continue","do","else","enum","float","for","goto","if","int",
"long","record","return","static","while" };

bool stringCheck = false, commentCheckLeftPar = false, commentCheckAsterisk1 = false, commentCheckAsterisk2 = false, digitCheck = false,
operatorCheck = false, alphaCheck = false, error = false, errorPrinted1 = false, errorPrinted2 = false, identifierError = false, integerError = false;


int main()
{
	outputFile = fopen("code.lex.txt", "w");
	if ((file = fopen("code.psi.txt", "r")) == NULL)
		fprintf(outputFile, "Psi file is either not found or not readable.");
	else {
		while (((tempChar = fgetc(file)) != EOF) ) {
			//identifierError identifierın 20 karakterden uzun olup olmadığını kontrol etmek için
			if (identifierError && !(tempChar == '_' || isalpha(tempChar) || isdigit(tempChar))) {
				memset(tempString, 0, sizeof(tempString));
				tempString[0] = '\0';
				stringCounter = 0;
				digitCheck = false;
				operatorCheck = false;
				alphaCheck = false;
				identifierError = false;
			}
			//integerError integerin 10 rakamdan uzun olup olmadığını kontrol etmek için
			if (integerError && !(isdigit(tempChar))) {

				memset(tempString, 0, sizeof(tempString));
				tempString[0] = '\0';
				stringCounter = 0;
				digitCheck = false;
				operatorCheck = false;
				alphaCheck = false;
				integerError = false;
			}
			if (tempString[0] == '\0') {
				error = false;
				errorPrinted1 = false;
				errorPrinted2 = false;
			}
			if (tempChar == '*' && commentCheckLeftPar) {
				commentCheckAsterisk1 = true;
				commentCheckLeftPar = false;
				continue;
			}
			if (commentCheckLeftPar && !commentCheckAsterisk1) {
				commentCheckLeftPar = false;
				fprintf(outputFile,"LeftPar\n");
			}
			if (!commentCheckAsterisk1) {
				if (!stringCheck) {
					if (tempChar == '\"' || tempChar == '(' || tempChar == ')' || tempChar == '[' || tempChar == ']' || tempChar == '{' || tempChar == '}' || tempChar == ';') {
						if (tempString[0] != '\0') {
							control();
						}

						if (errorPrinted1 || errorPrinted2) { //herhangi bir şekilde hata yazdırıldıktan sonra bir sonraki kontrolü dolu bir arrayle yapmamak için arrayi boşaltır
							memset(tempString, 0, sizeof(tempString));
							tempString['\0'];
							stringCounter = 0;
							digitCheck = false;
							operatorCheck = false;
							alphaCheck = false;
						}
					}
					if (tempChar == '\"')
					{
						stringCheck = true;
						continue;
					}
					else if (tempChar == '(') {
						commentCheckLeftPar = true;
						continue;
					}
					else if (tempChar == ')') {
						fprintf(outputFile, "RightPar\n");
					}
					else if (tempChar == '[') {
						fprintf(outputFile, "LeftSquareBracket\n");
					}
					else if (tempChar == ']') {
						fprintf(outputFile, "RightSquareBracket\n");
					}
					else if (tempChar == '{') {
						fprintf(outputFile, "LeftCurlyBracket\n");
					}
					else if (tempChar == '}') {
						fprintf(outputFile, "RightCurlyBracket\n");
					}
					else if (tempChar == ';') {
						fprintf(outputFile, "EndOfLine\n");
					}

					else {
						if (stringCounter > 20)
						{
							if (!identifierError) {
								fprintf(outputFile, "Identifiers can not have more than 20 characters.\n");
								identifierError = true;
							}

						}
						else {
							if (tempString[0] == '+' || tempString[0] == '-' || tempString[0] == '*' || tempString[0] == ':' || tempString[0] == '/')
								operatorCheck = true;
							else if (isdigit(tempString[0]))
								digitCheck = true;
							else if (isalpha(tempString[0]))
								alphaCheck = true;
							if (operatorCheck && !(tempChar == '+' || tempChar == '-' || tempChar == '*' || tempChar == ':' || tempChar == '=' || tempChar == '/')) {
								control();
								if (errorPrinted1) {
									memset(tempString, 0, sizeof(tempString));
									tempString[0] = '\0';
									stringCounter = 0;
									digitCheck = false;
									operatorCheck = false;
									alphaCheck = false;
								}
							}
							if (isdigit(tempChar)) {
								if (operatorCheck)
								{
									control();
								}
								if (strlen(tempString) <= 10) {
									tempString[stringCounter] = tempChar;
									tempString[stringCounter + 1] = '\0';
									stringCounter++;
								}
								if (digitCheck && strlen(tempString) > 10) {
									if (!integerError) {
										fprintf(outputFile, "Maximum integer size is 10 digits. \n");
										integerError = true;
									}
								}
							}
							else if (isalpha(tempChar)) {
								if (digitCheck) {
									if (!error) {
										fprintf(outputFile, "Identifiers can not start with a digit.\n");
										error = true;

									}
								}
								else if (operatorCheck) {
									control();
								}
								if (strlen(tempString) <= 20) {
									tempString[stringCounter] = tempChar;
									tempString[stringCounter + 1] = '\0';
									stringCounter++;
								}
							}
							else if (tempChar == '+' || tempChar == '-' || tempChar == '*' || tempChar == ':' || tempChar == '=' || tempChar=='/') {
								if (operatorCheck) {
									tempString[stringCounter] = tempChar;
									tempString[stringCounter + 1] = '\0';
									control();
									if (errorPrinted1) {
										tempString[0] = tempString[1];
										tempString[1] = '\0';
										stringCounter = 1;
									}
								}
								else {
									if (tempString[0] != '\0') {
										control();
									}
									tempString[stringCounter] = tempChar;
									tempString[stringCounter + 1] = '\0';
									stringCounter++;
								}
							}
							else if (tempChar == '_') {
								if (tempString[0] == '\0') {
									error = true;
								}
								if (strlen(tempString) <= 20) {
									tempString[stringCounter] = tempChar;
									tempString[stringCounter + 1] = '\0';
									stringCounter++;
								}
							}
							else if (tempChar == ' ' || tempChar == '\n' || tempChar == '\t') {
								if (tempString[0] != '\0') {
									control();

								}
								if (errorPrinted1 || errorPrinted2 || error) {
									memset(tempString, 0, sizeof(tempString));
									tempString['\0'];
									stringCounter = 0;
									digitCheck = false;
									operatorCheck = false;
									alphaCheck = false;
								}
								continue;
							}
							else {
								tempString[stringCounter] = tempChar;
								tempString[stringCounter + 1] = '\0';
								stringCounter++;
								fprintf(outputFile, "You have entered a wrong character to the file.\n");
							}
						}
					}
				}
				else {
					if (tempChar == '\"') {
						stringCheck = false;
						continue;
					}
					else {
						continue;
					}
				}
			}
			else {
				if (tempChar == '*') {
					commentCheckAsterisk2 = true;
					continue;
				}
				else if (commentCheckAsterisk2 && tempChar == ')') {
					commentCheckAsterisk1 = false;
					commentCheckAsterisk2 = false;
					continue;
				}
				else {
					commentCheckAsterisk2 = false;
					continue;
				}
				continue;
			}
		}
		if (tempString[0] != '\0') {
			control();
		}
		if (commentCheckAsterisk1 && !error) {
			fprintf(outputFile, "Lexical Error: A comment is not terminated.");
		}
		if (stringCheck && !error) {
			fprintf(outputFile, "Lexical Error: A String is not terminated.");
		}
	}
	fclose(file);
	fclose(outputFile);
	return 0;
}
void control()
{
	bool found = false; // keyword, integer, operator, identifier bulduğunda true verir
	bool foundOperator = false; // operatörlerde karşılaşılan hataları engellemek için kullanılır. örneğin -= geldiğinde Operator(-) yazdırabilmek için
	for (int k = 0; k < strlen(tempString); k++)
		tempString[k] = tolower(tempString[k]);
	for (int i = 0; i < 18; i++) {
		if (strcmp(tempString, keywordsArray[i]) == 0) {
			fprintf(outputFile, "Keyword(%s)\n", tempString);
			found = true;
		}
	}
	if (!found) {

		for (int i = 0; i < 7; i++) {
			if (strcmp(tempString, operatorsArray[i]) == 0) {
				fprintf(outputFile, "Operator(%s)\n", tempString);
				found = true;
			}
		}
		if (!found) {
			char tempOperatorString[2];
			tempOperatorString[0] = tempString[0];
			tempOperatorString[1] = '\0';
			for (int i = 0; i < 7; i++) {
				if (strcmp(tempOperatorString, operatorsArray[i]) == 0) {
					fprintf(outputFile, "Operator(%s)\n", tempOperatorString);
					found = true;
					foundOperator = true;
				}
			}
		}

	}
	if (!found) {
		if (isdigit(tempString[0]))
		{
			if (!error) {
				fprintf(outputFile, "IntConst(%s)\n", tempString);

			}
			found = true;
		}
	}
	if (!found) {
		if (isalpha(tempString[0])) {
			for (int k = 0; k < strlen(tempString); k++)
				tempString[k] = toupper(tempString[k]);
			bool identifierError = false;
			for (int j = 0; j < strlen(tempString); j++) {
				if (!(isalpha(tempString[j]) || isdigit(tempString[j]) || tempString[j] == '_')) {
					identifierError = true;
				}
			}
			if (!identifierError) {
				found = true;
				fprintf(outputFile, "Identifier(%s)\n", tempString);
			}
			else {
				if (!errorPrinted2) {
					fprintf(outputFile, "Identifier is not composed properly.\n");
				}
			}
		}
	}
	if (found) {
		if (foundOperator) {
			tempString[0] = tempString[1];
			tempString[1] = '\0';
			stringCounter = 1;
		}
		else {
			memset(tempString, 0, sizeof(tempString));
			tempString[0] = '\0';
			stringCounter = 0;
			digitCheck = false;
			operatorCheck = false;
			alphaCheck = false;
		}

	}
	else {
		if (!errorPrinted1) {
			fprintf(outputFile, "This token contains an error.\n");
			errorPrinted1 = true;
		}
	}
}
