/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos/zero>
 * Copyright (C) 2011-2016 Nostalrius <https://nostalrius.org>
 * Copyright (C) 2016-2017 Elysium Project <https://github.com/elysium-project>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Common.h"

//============================================
class LinkedListHead;

class LinkedListElement
{
    private:

        friend class LinkedListHead;

        LinkedListElement* iNext;
        LinkedListElement* iPrev;

    public:

        LinkedListElement() : iNext(nullptr), iPrev(nullptr) { }
        ~LinkedListElement() { delink(); }

        bool hasNext() const  { return (iNext && iNext->iNext != nullptr); }
        bool hasPrev() const  { return (iPrev && iPrev->iPrev != nullptr); }
        bool isInList() const { return (iNext != nullptr && iPrev != nullptr); }

        LinkedListElement      * next()       { return hasNext() ? iNext : nullptr; }
        LinkedListElement const* next() const { return hasNext() ? iNext : nullptr; }
        LinkedListElement      * prev()       { return hasPrev() ? iPrev : nullptr; }
        LinkedListElement const* prev() const { return hasPrev() ? iPrev : nullptr; }

        LinkedListElement      * nocheck_next()       { return iNext; }
        LinkedListElement const* nocheck_next() const { return iNext; }
        LinkedListElement      * nocheck_prev()       { return iPrev; }
        LinkedListElement const* nocheck_prev() const { return iPrev; }

        void delink()
        {
            if (isInList())
            {
                iNext->iPrev = iPrev;
                iPrev->iNext = iNext;
                iNext = nullptr;
                iPrev = nullptr;
            }
        }

        void insertBefore(LinkedListElement* pElem)
        {
            pElem->iNext = this;
            pElem->iPrev = iPrev;
            iPrev->iNext = pElem;
            iPrev = pElem;
        }

        void insertAfter(LinkedListElement* pElem)
        {
            pElem->iPrev = this;
            pElem->iNext = iNext;
            iNext->iPrev = pElem;
            iNext = pElem;
        }
};

//============================================

class LinkedListHead
{
    private:

        LinkedListElement iFirst;
        LinkedListElement iLast;
        uint32 iSize;

    public:

        LinkedListHead()
        {
            // create empty list

            iFirst.iNext = &iLast;
            iLast.iPrev = &iFirst;
            iSize = 0;
        }

        bool isEmpty() const { return (!iFirst.iNext->isInList()); }

        LinkedListElement      * getFirst()       { return (isEmpty() ? nullptr : iFirst.iNext); }
        LinkedListElement const* getFirst() const { return (isEmpty() ? nullptr : iFirst.iNext); }

        LinkedListElement      * getLast()        { return (isEmpty() ? nullptr : iLast.iPrev); }
        LinkedListElement const* getLast() const  { return (isEmpty() ? nullptr : iLast.iPrev); }

        void insertFirst(LinkedListElement* pElem)
        {
            iFirst.insertAfter(pElem);
        }

        void insertLast(LinkedListElement* pElem)
        {
            iLast.insertBefore(pElem);
        }

        uint32 getSize() const
        {
            if (!iSize)
            {
                uint32 result = 0;
                LinkedListElement const* e = getFirst();

                while(e)
                {
                    ++result;
                    e = e->next();
                }

                return result;
            }
            else
                return iSize;
        }

        void incSize() { ++iSize; }
        void decSize() { --iSize; }

        template<class _Ty>
        class Iterator
        {
            public:

                typedef std::bidirectional_iterator_tag iterator_category;
                typedef _Ty value_type;
                typedef ptrdiff_t difference_type;
                typedef ptrdiff_t distance_type;
                typedef _Ty* pointer;
                typedef _Ty const* const_pointer;
                typedef _Ty& reference;
                typedef _Ty const& const_reference;


                Iterator()
                    : _Ptr(nullptr)
                {                                           // construct with null node pointer
                }

                Iterator(LinkedListElement const* _Pnode)
                    : _Ptr((LinkedListElement*)_Pnode)
                {                                           // construct with node pointer _Pnode
                }

                Iterator& operator=(Iterator const &_Right)
                {
                    _Ptr = _Right._Ptr;
                    return (*this);
                }

                Iterator& operator=(const_pointer const &_Right)
                {
                    _Ptr = (pointer)_Right;
                    return (*this);
                }

                reference operator*()
                {                                           // return designated value
                    return *((pointer)_Ptr);
                }

                pointer operator->()
                {                                           // return pointer to class object
                    return (pointer)_Ptr;
                }

                Iterator& operator++()
                {                                           // preincrement
                    _Ptr = _Ptr->next();
                    return (*this);
                }

                Iterator operator++(int)
                {                                           // postincrement
                    iterator _Tmp = *this;
                    ++*this;
                    return (_Tmp);
                }

                Iterator& operator--()
                {                                           // predecrement
                    _Ptr = _Ptr->prev();
                    return (*this);
                }

                Iterator operator--(int)
                {                                           // postdecrement
                    iterator _Tmp = *this;
                    --*this;
                    return (_Tmp);
                }

                bool operator==(Iterator const &_Right) const
                {                                           // test for iterator equality
                    return (_Ptr == _Right._Ptr);
                }

                bool operator!=(Iterator const &_Right) const
                {                                           // test for iterator inequality
                    return (!(*this == _Right));
                }

                bool operator==(pointer const &_Right) const
                {                                           // test for pointer equality
                    return (_Ptr != _Right);
                }

                bool operator!=(pointer const &_Right) const
                {                                           // test for pointer equality
                    return (!(*this == _Right));
                }

                bool operator==(const_reference _Right) const
                {                                           // test for reference equality
                    return (_Ptr == &_Right);
                }

                bool operator!=(const_reference _Right) const
                {                                           // test for reference equality
                    return (_Ptr != &_Right);
                }

                pointer _Mynode()
                {                                           // return node pointer
                    return (pointer)(_Ptr);
                }

            protected:

                LinkedListElement* _Ptr;                               // pointer to node
        };

        typedef Iterator<LinkedListElement> iterator;
};

//============================================

#endif
