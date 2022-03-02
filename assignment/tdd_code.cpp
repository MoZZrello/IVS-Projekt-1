//======== Copyright (c) 2021, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Richard Harman <xharma05@stud.fit.vutbr.cz>
// $Date:       $2022-02-18
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Richard Harman
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find/GetHead ...
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "singly linked list", ktera bude splnovat dodane testy
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
    m_pHead = nullptr;
}

PriorityQueue::~PriorityQueue()
{
    Element_t *temp = GetHead();
    while(m_pHead != nullptr){
        m_pHead = m_pHead->pNext;
        delete temp;
        temp = m_pHead;
    }
}

void PriorityQueue::Insert(int value)
{
    Element_t *insert = new Element_t;
    Element_t *storage = new Element_t;
    if(insert != nullptr){
        insert->pNext = nullptr;
        insert->value = value;
    }
    if(m_pHead == nullptr){
        m_pHead = insert;
    }else if(insert->value >= m_pHead->value){
        storage = m_pHead;
        m_pHead = insert;
        insert->pNext = storage;
    }else{
        Element_t *temp = m_pHead;
        while(temp->pNext != nullptr && (temp->pNext->value > insert->value)){
            temp = temp->pNext;
        }
        if(temp->pNext == nullptr){
            temp->pNext = insert;
        }else{
            storage = temp->pNext;
            temp->pNext = insert;
            insert->pNext = storage;
        }
    }
}

/*
 * 1. Ak je koren NULL pointer, nemame co odstranit
 * 2. Odstranenie korena
 * 3. Hladanie prvku ak to nebol koren
 * 4. Odstranenie najdeneho prvku ineho ako koren
 */
bool PriorityQueue::Remove(int value)
{
    Element_t *element = m_pHead;
    Element_t *delete_element;
//-------------------1.----------------------
    if(m_pHead == nullptr){
        return false;
    }
//-------------------2.----------------------
    if(value == element->value){
        m_pHead = element->pNext;
        free(element);
        return true;
    }
//-------------------3.----------------------
    while(element->pNext->value != value){
        if(element->pNext->pNext == nullptr) {
            return false;
        }
        element = element->pNext;
    }
//-------------------4.----------------------
    delete_element = element->pNext;
    element->pNext = element->pNext->pNext;
    free(delete_element);
    return true;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    Element_t *found = m_pHead;
    while(found != nullptr && found->value != value){
        found = found->pNext;
    }
    return found;
}

size_t PriorityQueue::Length()
{
    int length = 0;
    Element_t *temp = m_pHead;
    while(temp != nullptr){
        temp = temp->pNext;
        length++;
    }
	return length;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return m_pHead;
}

/*** Konec souboru tdd_code.cpp ***/
