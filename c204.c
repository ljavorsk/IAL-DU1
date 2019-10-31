
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {

  char tmp_stack_top;

  while (stackEmpty(s) != -1) // While the stack isn't empty
  {
    stackTop(s, &tmp_stack_top);  // Get the top of the stack
    stackPop(s);  // Pops the top

    if (tmp_stack_top != '(')
    {
      postExpr[(*postLen)++] = tmp_stack_top; // Writes the current stack's top value
    }
    else  // We have found the and poped '('
    {
      return;
    }
  }
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {
  if (stackEmpty(s) != 0) // When empty stack
  {
    stackPush(s, c);
  }
  else                // Not empty
  {
    char stack_top;
    stackTop(s, &stack_top);  // What's on the top?

    if (stack_top == '(' ||               // If the left bracket is on the top
        ((c == '*' || c == '/') &&                // Or if it's operator with bigger priority
        (stack_top == '+' || stack_top == '-')))  // than current top operator
    {
      stackPush(s, c);
    }
    else    // It's lower or the same priority operator or it's not '('
    {
      stackPop(s);  // Pop the operator from stack
      postExpr[(*postLen)++] = stack_top; // Add it to the array


      // Recursive call
      doOperation(s, c, postExpr, postLen);
    }
  }
}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {

  char *postExpr = (char *) malloc(sizeof(char) * MAX_LEN);
  if (postExpr == NULL)
  {
    return NULL;
  }
  
  unsigned postLen = 0;
  char c; // Temporary char for cleaner code

  tStack s;
  stackInit(&s);

  for (size_t i = 0; (c = infExpr[i]) != '\0'; i++) // Goes through input untill '\0' is found
  {
    if(c == '+' || c == '-' || c == '*' || c == '/')  // Is operator (+-*/)
		{		
			doOperation(&s, c, postExpr, &postLen);
		}
    else if (c == '(')  // If '(' push it to stack
    {
      stackPush(&s, c);
    }
    else if (c == ')')
    {
      untilLeftPar(&s, postExpr, &postLen);  // Put all operator to the postExpr until '(' occur
    }
    else if ((c >= 'a' && c <= 'z') ||  // Is identificator or number
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9'))
    {
      postExpr[postLen++] = c;
    }
    else if (c == '=')
    {
      while (stackEmpty(&s) != -1)  // Until empty stack
      {
        // Take all operators from stack and put them to the array, then pop
        stackTop(&s, &postExpr[postLen++]);
        stackPop(&s);
      }
    }
  }
  
  // End the postfix correctly
  postExpr[postLen++] = '=';
  postExpr[postLen] = '\0';

  return postExpr;
}

/* Konec c204.c */
