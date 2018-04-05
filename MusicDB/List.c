#include "List.h"

//***		local macro

#define			LIST(inHandle)		((__hsList			)(inHandle))
#define			ITEM(inHandle)		((__hsListContainer	)(inHandle))

//***		local types

typedef
	 struct __ListContainer
		{
			void*					data;
			struct __ListContainer*	next;
			struct __ListContainer*	prev;
		}

	__sListContainer,
	*__hsListContainer;

typedef
	struct __List
		{
			size_t					count;
			__hsListContainer		first;
			__hsListContainer		last;
		}

	__sList,
	*__hsList;

//***		local functions

void quickSort	(
					hsList inList, 
					size_t leftIndex, 
					__hsListContainer left, 
					size_t rightIndex, 
					__hsListContainer right, 
					hfComparator compare
				);

//***		implementation

hsList __ctor_List()
	{
		__hsList list	= malloc(sizeof(__sList));

		list->count		= 0;
		list->first		= NULL;
		list->last		= NULL;

		return list;
	}

void __dtor_ListNoRelease(hsList inList)
	{
		if(LIST(inList)->first)
			{
				__hsListContainer tmp;

				while(LIST(inList)->first)
					{
						tmp = LIST(inList)->first->next;

						free(LIST(inList)->first		);

						LIST(inList)->first = tmp;
					}
			}

		free(inList);
	}

void __dtor_List(hsList inList, hfDestructor destructor)
	{
		if(LIST(inList)->first)
			{
				__hsListContainer tmp;

				if(destructor)
					{
						while(LIST(inList)->first)
							{
								tmp = LIST(inList)->first->next;

								destructor(LIST(inList)->first->data);
								free(LIST(inList)->first			);

								LIST(inList)->first = tmp;
							}
					}
				else
					{
						while(LIST(inList)->first)
							{
								tmp = LIST(inList)->first->next;

								free(LIST(inList)->first->data	);
								free(LIST(inList)->first		);

								LIST(inList)->first = tmp;
							}
					}
			}

		free(inList);
	}

void quickSort	(
					hsList				inList, 
					size_t				leftIndex, 
					__hsListContainer	left,  
					size_t				rightIndex, 
					__hsListContainer	right,
					hfComparator		compare
				)
	{
		size_t				locLeftIndex	= leftIndex;
		size_t				locRightIndex	= rightIndex;
		__hsListContainer	locLeft			= left;
		__hsListContainer	locRight		= right;
      
		void*	tmp;
		size_t	pivotIndex					= (leftIndex + rightIndex) >> 1;

		__hsListContainer pivot				= LIST(inList)->first;
		while(pivot && pivotIndex > 0)
			{
				pivot = pivot->next;
				--pivotIndex;
			}

		if(!pivot)
			return;

		while(locLeftIndex <= locRightIndex) 
			{
				while(compare(locLeft->data, pivot->data) == CMP_LESS)
					{
						locLeft = locLeft->next;
						++locLeftIndex;
					}

				while(compare(locRight->data, pivot->data) == CMP_GREATER)
					{
						locRight = locRight->prev;
						--locRightIndex;
					}

				if(locLeftIndex < locRightIndex) 
					{
						tmp				= locLeft->data;
						locLeft->data	= locRight->data;
						locRight->data	= tmp;

						locLeft			= locLeft->next;
						++locLeftIndex;

						locRight		= locRight->prev;
						--locRightIndex;
					}
				else
					{
						locLeft			= locLeft->next;
						++locLeftIndex;

						locRight		= locRight->prev;
						--locRightIndex;

						break;
					}
			}

		if(leftIndex < locLeftIndex)
			quickSort(inList, leftIndex, left, locRightIndex, locRight, compare);

		if(locRightIndex < rightIndex)
			quickSort(inList, locLeftIndex, locLeft, rightIndex, right, compare);
	}

void listSort(hsList inList, hfComparator comparator)
	{
		//	You can try some other sorting algorithmn here used basic quick sort.
		quickSort(inList, 0, LIST(inList)->first, LIST(inList)->count - 1, LIST(inList)->last, comparator);
	}

void insertItemToStart(hsList inList, void* data)
	{
		__hsListContainer tmp;
		++LIST(inList)->count;

		tmp = malloc(sizeof(__sListContainer));

		if(LIST(inList)->first)
			{
				LIST(inList)->first->prev = tmp;
				tmp->next = LIST(inList)->first;
			}
		else
			{
				LIST(inList)->last = tmp;
				tmp->next = NULL;
			}

		tmp->prev			= NULL;
		tmp->data			= data;
		LIST(inList)->first = tmp;
	}

void insertItemToEnd(hsList inList, void* data)
	{
		__hsListContainer tmp;
		++LIST(inList)->count;

		tmp = malloc(sizeof(__sListContainer));

		if(LIST(inList)->last)
			{
				LIST(inList)->last->next = tmp;
				tmp->prev = LIST(inList)->last;
			}
		else
			{
				LIST(inList)->first = tmp;
				tmp->prev = NULL;
			}

		tmp->next			= NULL;
		tmp->data			= data;
		LIST(inList)->last	= tmp;
	}

void removeItem(hsList inList, hsListItem item, hfDestructor destructor)
	{
		if(LIST(inList)->count)
			--LIST(inList)->count;

		if(LIST(inList)->first == item)
			LIST(inList)->first = ITEM(item)->next;

		if(LIST(inList)->last == item)
			LIST(inList)->last = ITEM(item)->prev;

		if(ITEM(item)->next)
			ITEM(item)->next->prev = ITEM(item)->prev;

		if(ITEM(item)->prev)
			ITEM(item)->prev->next = ITEM(item)->next;

		if(destructor)
			destructor(ITEM(item)->data);
		else
			free(ITEM(item)->data);

		free(item);
	}

void removeItemNoRelease(hsList inList, hsListItem item)
	{
		if(LIST(inList)->count)
			--LIST(inList)->count;

		if(LIST(inList)->first == item)
			LIST(inList)->first = ITEM(item)->next;

		if(LIST(inList)->last == item)
			LIST(inList)->last = ITEM(item)->prev;

		if(ITEM(item)->next)
			ITEM(item)->next->prev = ITEM(item)->prev;

		if(ITEM(item)->prev)
			ITEM(item)->prev->next = ITEM(item)->next;

		free(item);
	}

hsListItem getFirst(hsList inList)
	{
		return LIST(inList)->first;
	}

hsListItem getLast(hsList inList)
	{
		return LIST(inList)->first;
	}

size_t getCount(hsList inList)
	{
		return LIST(inList)->count;
	}

void* getItemValue(hsListItem inItem)
	{
		return ITEM(inItem)->data;
	}

hsListItem getNextItem(hsListItem inItem)
	{
		return ITEM(inItem)->next;
	}

hsListItem getPrevItem(hsListItem inItem)
	{
		return ITEM(inItem)->prev;
	}