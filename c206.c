
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

    // If the list is empty don't do anything
    if (L->First == NULL)
        return;
    
    while (L->First->rptr != NULL)
    {
        tDLElemPtr tmp_elem = L->First->rptr;
        free(L->First);
        L->First = tmp_elem;
    }

    // Free the last element
    free(L->First);
    // Tell the list that it's empty
    L->First = NULL;
    L->Last = NULL;
    L->Act = NULL;
    return;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
    tDLElemPtr first_elem = (tDLElemPtr) malloc(sizeof(struct tDLElem));
    if (first_elem == NULL)
    {
        DLError();
        return;
    }
    
    first_elem->data = val;
    first_elem->lptr = NULL;
    first_elem->rptr = L->First;

    if (L->First != NULL)
    {
        L->First->lptr = first_elem;
    }
    else
    {
        L->Last = first_elem;
    }
    L->First = first_elem;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	

	tDLElemPtr first_elem = (tDLElemPtr) malloc(sizeof(struct tDLElem));
    if (first_elem == NULL)
    {
        DLError();
        return;
    }
    
    first_elem->data = val;
    first_elem->rptr = NULL;
    first_elem->lptr = L->Last;

    if (L->Last != NULL)
    {
        L->Last->rptr = first_elem;
    }
    else
    {
        L->First = first_elem;
    }
    L->Last = first_elem;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
    L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	
	if (L->First == NULL)
    {
        DLError();
        return;
    }

    *val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	
    if (L->First == NULL)
    {
        DLError();
        return;
    }

    *val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	
    if (L->First == NULL)
    {
        return;
    }
    tDLElemPtr elem_to_delete = L->First;
    // First was active = deactivated
    if (L->Act == L->First)
    {
        L->Act = NULL;
    }
    // Lonely one
    if (L->First->rptr == NULL)
    {
        L->First = NULL;
        L->Last = NULL;
    }
    else
    {
        L->First = L->First->rptr;
        L->First->lptr = NULL;
    }
    free(elem_to_delete);
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	
	if (L->Last == NULL)
    {
        return;
    }
    tDLElemPtr elem_to_delete = L->Last;
    // First was active = deactivated
    if (L->Act == L->Last)
    {
        L->Act = NULL;
    }
    // Lonely one
    if (L->Last->lptr == NULL)
    {
        L->First = NULL;
        L->Last = NULL;
    }
    else
    {
        L->Last = L->Last->lptr;
        L->Last->rptr = NULL;
    }
    free(elem_to_delete);
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	
    if (L->Act == NULL || L->Act == L->Last)
    {
        return;
    }
    if (L->Act->rptr != NULL)
    {
        tDLElemPtr elem_to_delete = L->Act->rptr;
        L->Act->rptr = elem_to_delete->rptr;
        if (elem_to_delete->rptr == NULL)
        {
            L->Last = L->Act;
        }
        else
        {
            elem_to_delete->rptr->lptr = L->Act;
        }
        free(elem_to_delete);
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	
    if (L->Act == NULL || L->Act == L->First)
    {
        return;
    }

    if (L->Act->lptr != NULL)
    {
        
        tDLElemPtr elem_to_delete = L->Act->lptr;
        L->Act->lptr = elem_to_delete->lptr;
        if (elem_to_delete->lptr == NULL)
        {
            L->First = L->Act;
        }
        else
        {
            elem_to_delete->lptr->rptr = L->Act;   
        }
        free(elem_to_delete);
    }	
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	if (L->Act == NULL)
    {
        return;
    }

    tDLElemPtr elem_to_add = (tDLElemPtr) malloc(sizeof(struct tDLElem));

    if (elem_to_add == NULL)
    {
        DLError();
        return;
    }

    elem_to_add->data = val;
    elem_to_add->lptr = L->Act;
    elem_to_add->rptr = L->Act->rptr;
    L->Act->rptr = elem_to_add;

    if (L->Act == L->Last)
    {
        L->Last = elem_to_add;
    }
    else
    {
        elem_to_add->rptr->lptr = elem_to_add;
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
    if (L->Act == NULL)
    {
        return;
    }

    tDLElemPtr elem_to_add = (tDLElemPtr) malloc(sizeof(struct tDLElem));

    if (elem_to_add == NULL)
    {
        DLError();
        return;
    }

    elem_to_add->data = val;
    elem_to_add->rptr = L->Act;
    elem_to_add->lptr = L->Act->rptr;
    L->Act->lptr = elem_to_add;

    if (L->Act == L->First)
    {
        L->First = elem_to_add;
    }
    else
    {
        elem_to_add->lptr->rptr = elem_to_add;
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
		
	if (L->Act == NULL)
    {
        DLError();
        return;
    }

    *val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
    if (L->Act == NULL)
    {
        return;
    }

    L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
    if (L->Act == NULL)
    {
        return;
    }

    L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	
    if (L->Act == NULL)
    {
        return;
    }

    L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	
	return (L->Act != NULL) ? 1 : 0;
}

/* Konec c206.c*/
