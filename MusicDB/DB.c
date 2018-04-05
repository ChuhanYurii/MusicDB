#include "DB.h"

//***		local const

const char* __bandTemplate0 =
"\n\
%i :Band\n\
{\n\
  Name : %s,\n\
  Musicians\n\
  [";

const char* __bandTemplate1 =
"  ],\n\
  Albums\n\
  [";

const char* __bandTemplate2 =
"  ]\n\
}";

const char* __musicianTemplate0 =
"\n\
    %i :Musician\n\
    {\n\
      Name : %s,\n\
      StartDate : %02hhu.%02hhu.%hu";

const char* __musicianTemplate1 =
",\n\
      EndDate : %02hhu.%02hhu.%hu";

const char* __musicianTemplate2 =
"\n\
    }";

const char* __albumTemplate		=
"\n\
    %i :%s";

//***		local functions

char*	substr				(char* source, char* str					);
int		bandComparator		(hsBand _fBand, hsBand _sBand				);
int		musicianComparator	(hsMusician fMusician, hsMusician sMusician	);
int		albumComparator		(char* fAlbum, char* sAlbum					);

//***		implementation

void musicianDestructor(hsMusician inMusician)
	{
		free(inMusician->name		);
		
		if(inMusician->endDate)
			free(inMusician->endDate);
	}

void bandDestructor(hsBand band)
	{
		free(band->name);
		__dtor_List(band->albums,		NULL				);
		__dtor_List(band->musicians,	musicianDestructor	);
	}

hsList readMusicDB(char* filePath)
	{
		hFILE	fdb		= fopen(filePath, "rb");
		hsList	outDB	= __ctor_List();
		size_t	dbSize;
		size_t	size;
		size_t	nameSize;

		char* albumName;

		//read bands count
		fread(&dbSize, sizeof(dbSize), 1, fdb);

		while(dbSize > 0)
			{
				hsBand currBand		= malloc(sizeof(sBand));

				currBand->musicians = __ctor_List();
				currBand->albums	= __ctor_List();

				//read name length
				fread(&size, sizeof(size), 1, fdb);

				//read name
				currBand->name = malloc(size + 1);
				fread(currBand->name, size, 1, fdb);
				currBand->name[size] = 0;

				//read musicians count
				fread(&size, sizeof(size), 1, fdb);

				while(size > 0)
					{
						hsMusician currMusician = malloc(sizeof(sMusician));

						//read length of name
						fread(&nameSize, sizeof(nameSize), 1, fdb);

						//read name
						currMusician->name = malloc(nameSize + 1);	//+1 for NULL terminator
						fread(currMusician->name, nameSize, 1, fdb);
						currMusician->name[nameSize] = 0;			//NULL terminator

						fread(&currMusician->startDate, sizeof(currMusician->startDate), 1, fdb);
						
						fread(&nameSize, sizeof(nameSize), 1, fdb);
						
						if(nameSize)
							{
								currMusician->endDate = malloc(sizeof(sDate));
								fread(currMusician->endDate, sizeof(sDate), 1, fdb);
							}
						else
							currMusician->endDate = NULL;

						insertItemToEnd(currBand->musicians, currMusician);

						--size;
					}

				//read albums count
				fread(&size, sizeof(size), 1, fdb);
				

				while(size > 0)
					{
						fread(&nameSize, sizeof(nameSize), 1, fdb);
                        
						albumName = malloc(nameSize + 1);
						fread(albumName, nameSize, 1, fdb);
						albumName[nameSize] = 0;

						insertItemToEnd(currBand->albums, albumName);

						--size;
					}

				--dbSize;

				insertItemToEnd(outDB, currBand);
			}

		fclose(fdb);

		return outDB;
	}

void writeMusicDB(char* filePath, hsList inDB)
	{
		hFILE	fdb = fopen(filePath, "wb");
		size_t	size;
		hsListItem	currBandItem;

		hsListItem curr;

		//write bands count
		size = getCount(inDB);
		fwrite(&size, sizeof(size), 1, fdb);

		//loop throught bands
		currBandItem = getFirst(inDB);
		while(currBandItem)
			{
				hsBand currBand	= getItemValue(currBandItem);

				//write name length
				size = strlen(currBand->name);
				fwrite(&size, sizeof(size), 1, fdb);

				//write name
				fwrite(currBand->name, size, 1, fdb);

				//write musicians count
				size = getCount(currBand->musicians);
				fwrite(&size, sizeof(size), 1, fdb);

				//loop through musicians
				curr = getFirst(currBand->musicians);
				while(curr)
					{
						hsMusician currMusician = getItemValue(curr);

						//write length of name
						size = strlen(currMusician->name);
						fwrite(&size, sizeof(size), 1, fdb);

						//write name
						fwrite(currMusician->name, size, 1, fdb);

						fwrite(&currMusician->startDate, sizeof(currMusician->startDate	),	1, fdb);

						if(currMusician->endDate)
							{
								size = 1;
								fwrite(&size, sizeof(size), 1, fdb);
								fwrite(currMusician->endDate,	sizeof(sDate	),	1, fdb);
							}
						else
							{
								size = 0;
								fwrite(&size, sizeof(size), 1, fdb);
							}

						curr = getNextItem(curr);
					}

				//write albums count
				size = getCount(currBand->albums);
				fwrite(&size, sizeof(size), 1, fdb);

				//loop through albums
				curr = getFirst(currBand->albums);
				while(curr)
					{
						char* albumName = getItemValue(curr);

						//write length of album name
						size = strlen(albumName);
						fwrite(&size, sizeof(size), 1, fdb);

						//write album name
						fwrite(albumName, size, 1, fdb);

						curr = getNextItem(curr);
					}

				currBandItem	= getNextItem(currBandItem);
			}

		fclose(fdb);
	}

hsBand cloneBand(hsBand inBand)
	{
		hsListItem item;

		hsBand cloneBand	= malloc(sizeof(sBand));

		cloneBand->name		= malloc(strlen(inBand->name) + 1);
		strcpy(cloneBand->name, inBand->name);

		cloneBand->musicians = __ctor_List();

		item = getFirst(inBand->musicians);

		while(item)
			{
				hsMusician musician = getItemValue(item);
				hsMusician insMusician = malloc(sizeof(sMusician));

				insMusician->name = malloc(strlen(musician->name) + 1);
				strcpy(insMusician->name, musician->name);

				insMusician->startDate = musician->startDate;

				if(musician->endDate)
					{
						insMusician->endDate = malloc(sizeof(sDate));
						*insMusician->endDate = *musician->endDate;
					}
				else
					insMusician->endDate = NULL;

				insertItemToEnd(cloneBand->musicians, insMusician);

				item = getNextItem(item);
			}

		cloneBand->albums = __ctor_List();

		item = getFirst(inBand->albums);

		while(item)
			{
				char* album = getItemValue(item);

				char* insAlbum = malloc(strlen(album) + 1);
				strcpy(insAlbum, album);

				insertItemToEnd(cloneBand->albums, insAlbum);

				item = getNextItem(item);
			}

		return cloneBand;
	}

hsList cloneMusicDB(hsList inDB)
	{
		hsList ret = __ctor_List();

		hsListItem band = getFirst(inDB);

		while(band)
			{
				insertItemToEnd(ret, /*cloneBand(*/getItemValue(band)/*)*/);

				band = getNextItem(band);
			}

		return ret;
	}

void releaseMusicDB(hsList inDB)
	{
		__dtor_List(inDB, bandDestructor);
	}

void releaseMusicQuery(hsList inQuery)
	{
		__dtor_ListNoRelease(inQuery);
	}

void sortBands(hsList inDB)
	{
		listSort(inDB, bandComparator);
	}

void sortMusicians(hsList inMusicians)
	{
		listSort(inMusicians, musicianComparator);
	}

void sortAlbums(hsList inAlbums)
	{
		listSort(inAlbums, albumComparator);
	}

//**		con

void printBand(hsBand inBand, size_t index)
	{
		hsListItem item;
		unsigned int locIndex = 0;

		printf(__bandTemplate0, index, inBand->name);
			
		item = getFirst(inBand->musicians);

		while(item)
			{
				hsMusician	musician	= getItemValue(item);

				printf(__musicianTemplate0, locIndex, musician->name, musician->startDate.day, musician->startDate.month, musician->startDate.year);

				if(musician->endDate)
					printf(__musicianTemplate1, musician->endDate->day, musician->endDate->month, musician->endDate->year);

				printf(__musicianTemplate2);

				item = getNextItem(item);
				++locIndex;

				if(item)
					printf(",");
				else
					printf("\n");
			}

		printf(__bandTemplate1);

		item = getFirst(inBand->albums);
		locIndex = 0;

		while(item)
			{
				char* album = getItemValue(item);

				printf(__albumTemplate, locIndex, album);

				item = getNextItem(item);
				++locIndex;

				if(item)
					printf(",");
				else
					printf("\n");
			}

		printf(__bandTemplate2);
	}

hsListItem printBandByItem(hsListItem inItem, size_t index)
	{
		printBand(getItemValue(inItem), index);

		if(inItem = getNextItem(inItem))
			printf(",");
		else
			printf("\n");

		return inItem;
	}

void printAllBands(hsList inDB)
	{
		hsListItem		item	= getFirst(inDB);
		unsigned int	index	= 0;

		while(item)
			{
				item = printBandByItem(item, index);
				++index;
			}
	}

//***			queries

void removeBand(hsList inDB, hsListItem band)
	{
		removeItem(inDB, band, bandDestructor);
	}

void removeMusician(hsList inMusicians, hsListItem musician)
	{
		removeItem(inMusicians, musician, musicianDestructor);
	}

void removeAlbum(hsList inAlbums, hsListItem album)
	{
		removeItem(inAlbums, album, NULL);
	}

void* findByIndex(hsList inDB, size_t index)
	{
		hsListItem item = getFirst(inDB);

		while(item && index > 0)
			{
				item = getNextItem(item);
				--index;
			}

		if(item)
			return getItemValue(item);

		return NULL;
	}

hsList findByBandNameSubString(hsList inDB, char* subString)
	{
		hsList		query = __ctor_List();
		hsListItem	item = getFirst(inDB);
		hsBand		band;

		while(item)
			{
				band = getItemValue(item);

				if(substr(band->name, subString))
					insertItemToEnd(query, band);

				item = getNextItem(item);
			}

		return query;
	}

hsList findByMusicianNameSubString(hsList inDB, char* subString)
	{
		hsList		query	= __ctor_List();
		hsListItem	item	= getFirst(inDB);
		hsBand		band;
		hsListItem	mItem;
		hsMusician	musician;

		while(item)
			{
				band = getItemValue(item);

				mItem = getFirst(band->musicians);

				while(mItem)
					{
						musician = getItemValue(mItem);

						if(substr(musician->name, subString))
							{
								insertItemToEnd(query, band);
								break;
							}

						mItem = getNextItem(mItem);
					}

				item = getNextItem(item);
			}

		return query;
	}

hsList findByAlbumNameSubString(hsList inDB, char* subString)
	{
		hsList		query	= __ctor_List();
		hsListItem	item	= getFirst(inDB);
		hsBand		band;
		hsListItem	aItem;

		while(item)
			{
				band = getItemValue(item);

				aItem = getFirst(band->albums);

				while(aItem)
					{
						if(substr(getItemValue(aItem), subString))
							{
								insertItemToEnd(query, band);
								break;
							}

						aItem = getNextItem(aItem);
					}

				item = getNextItem(item);
			}

		return query;
	}

char* substr(char* source, char* str)
	{
		char* baseStr;
		char* baseSource;

		while(*source)
			{
				baseStr		= str;
				baseSource	= source;

				while(*baseStr == *source && *baseStr)
					{
						++baseStr;
						++source;
					}

				if(!*baseStr)
					return baseSource;

				++source;
			}

		return NULL;
	}

int	bandComparator(hsBand _fBand, hsBand _sBand)
	{
		int result = strcmp(_fBand->name, _sBand->name);

		if(result < 0)
			return CMP_LESS;

		if(result > 0)
			return CMP_GREATER;

		return CMP_EQUAL;
	}

int	musicianComparator(hsMusician fMusician, hsMusician sMusician)
	{
		int result = strcmp(fMusician->name, sMusician->name);

		if(result < 0)
			return CMP_LESS;

		if(result > 0)
			return CMP_GREATER;

		return CMP_EQUAL;
	}

int	albumComparator(char* fAlbum, char* sAlbum)
	{
		int result = strcmp(fAlbum, sAlbum);

		if(result < 0)
			return CMP_LESS;

		if(result > 0)
			return CMP_GREATER;

		return CMP_EQUAL;
	}