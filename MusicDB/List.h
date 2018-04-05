//	Its a simple realisation of \doubly linked list\ [to make work with Data Base easier]

#ifndef __C__LIST__

	#define __C__LIST__

	#include <stdlib.h>

	//***			public macro

	#define			CMP_EQUAL			(unsigned int	)(0x00000001	)
	#define			CMP_LESS			(unsigned int	)(0x00000002	)
	#define			CMP_GREATER			(unsigned int	)(0x00000003	)

	//***			public types

	typedef 
		void* hsList;

	typedef 
		void* hsListItem;

	typedef
		int (fComparator)(void* fItem, void* lItem);

	typedef	
		fComparator *hfComparator;

	typedef
		void (fDestructor)(void* inItem);

	typedef
		fDestructor *hfDestructor;

	//***			public functions

	//**			List constructor & destructor

	hsList		__ctor_List			(															);
	void		__dtor_List			(hsList inList, hfDestructor destructor						);
	void		__dtor_ListNoRelease(hsList inList												);

	//**			List functions

	void		listSort			(hsList inList, hfComparator comparator						);
	void		insertItemToStart	(hsList inList, void* data									);
	void		insertItemToEnd		(hsList inList, void* data									);
	void		removeItem			(hsList inList, hsListItem	item, hfDestructor destructor	);
	void		removeItemNoRelease	(hsList inList, hsListItem item								);
	hsListItem	getFirst			(hsList inList												);
	hsListItem	getLast				(hsList inList												);
	size_t		getCount			(hsList inList												);

	void*		getItemValue		(hsListItem inItem											);
	hsListItem	getNextItem			(hsListItem inItem											);
	hsListItem	getPrevItem			(hsListItem inItem											);

#endif	//#ifndef __C__LIST__
