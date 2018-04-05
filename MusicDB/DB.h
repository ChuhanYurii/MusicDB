//	Database structures and functions\
	declared here

#ifndef __MUSIC_DB__

	#define __MUSIC_DB__

	#include <stdio.h>
	#include <time.h>
	#include <string.h>
	#include "List.h"

	//***			types

	typedef 
		FILE		*hFILE;

	typedef
		struct Date
			{
				unsigned char	day;
				unsigned char	month;
				unsigned short	year;
			}

		sDate,
		*hsDate;

	typedef
		struct Musician
			{
				char*		name;
				sDate		startDate;
				hsDate		endDate;
			}

		sMusician,
		*hsMusician;

	typedef
		struct Band
			{
				char*		name;
				hsList		musicians;
				hsList		albums;
			}

		sBand,
		*hsBand;

	//***			functions

	//**			db

	hsList		readMusicDB						(char* filePath								);
	void		writeMusicDB					(char* filePath, hsList inDB				);
	void		releaseMusicDB					(hsList inDB								);
	void		releaseMusicQuery				(hsList inQuery								);
	hsList		cloneMusicDB					(hsList inDB								);
	hsBand		cloneBand						(hsBand inBand								);

	void		sortBands						(hsList inDB								);
	void		sortMusicians					(hsList inMusicians							);
	void		sortAlbums						(hsList inAlbums							);

	//**			queries

	void		removeBand						(hsList inDB, hsListItem band				);
	void		removeMusician					(hsList inMusicians, hsListItem musician	);
	void		removeAlbum						(hsList inAlbums, hsListItem album			);

	hsList		findByBandNameSubString			(hsList inDB,	char* subString				);
	hsList		findByMusicianNameSubString		(hsList inDB,	char* subString				);
	hsList		findByAlbumNameSubString		(hsList inDB,	char* subString				);
	void*		findByIndex						(hsList inDB,	size_t index				);

	//**			console

	void		printAllBands					(hsList inDB								);
	hsListItem	printBandByItem					(hsListItem inItem, size_t index			);
	void		printBand						(hsBand inBand, size_t index				);

#endif //#ifndef __MUSIC_DB__