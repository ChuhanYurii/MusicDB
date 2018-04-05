#include "DB.h"

const char* mainMenu =
"\
1)Load Default DataBase\n\
2)Load DataBase.\n\
3)Save DataBase.\n\
4)Select from Bands.\n\
5)Insert into Bands.\n\
6)Update Bands.\n\
7)Delete from Bands.\n\
8)Commit.\n\
Choose action:\
";

#define		DB_RELOAD					0
#define		DB_LOAD						1
#define		DB_SAVE						2
#define		DB_SELECT					3
#define		DB_INSERT					4
#define		DB_UPDATE					5
#define		DB_DELETE					6
#define		DB_COMMIT					7
#define		DB_MAX						8

const char* selectMenu =
"\
1)Select from Bands [*].\n\
2)Select from Bands [*, Order By BandName].\n\
3)Select from Bands [*, where BandName like <>, Order By BandName]\n\
4)Select from Bands [*, where any MusicianName like <>, Order By BandName]\n\
5)Select from Bands [*, where any AlbumName like <>, Order By BandName]\n\
Choose action:\
";

#define		SEL_ALL						0
#define		SEL_ORDER					1
#define		SEL_LIKE_BAND_ORDER			2
#define		SEL_LIKE_MUSICIAN_ORDER		3
#define		SEL_LIKE_ALBUM_ORDER		4
#define		SEL_MAX						5

const char* updateMenu =
"\
1)Update BandName.\n\
2)Add musician.\n\
3)Remove musician.\n\
4)Update musician.\n\
5)Add album.\n\
6)Remove album.\n\
7)Sort musicians.\n\
8)Sort albums.\n\
Choose action:\
";

#define		UPD_NAME					0
#define		UPD_ADD_MUSICIAN			1
#define		UPD_REMOVE_MUSICIAN			2
#define		UPD_UPDATE_MUSICIAN			3
#define		UPD_ADD_ALBUM				4
#define		UPD_REMOVE_ALBUM			5
#define		UPD_SORT_MUSICIANS			6
#define		UPD_SORT_ALBUMS				7
#define		UPD_MAX						8

const char* updateMusicianMenu =
"\
1)Update name.\n\
2)Update start date.\n\
3)Update end date.\n\
Choose action:\
";

#define		MUS_UPDATE_NAME				0
#define		MUS_UPDATE_START_DATE		1
#define		MUS_UPDATE_END_DATE			2
#define		MUS_MAX						3

const char* __bandInputTemplate0 =
"\
Band\n\
{\n\
  Name : \
";

const char* __bandInputTemplate1 =
"\
  Enter musicians count :";

const char* __bandInputTemplate2 =
"\
  Musicians\n\
  [\n\
";

const char* __bandInputTemplate3 =
"\
  ],\n\
  Enter albums count :";

const char* __bandInputTemplate4 =
"\
  Albums\n\
  [\n\
";

const char* __bandInputTemplate5 =
"\
  ]\n\
}\
";

const char* __musicianInputTemplate0 =
"\
    Musician\n\
    {\n\
      Name :\
";

const char* __musicianInputTemplate1 =
"\
      StartDate :";

const char* __musicianInputTemplate2 =
"\
      EndDate :";

const char* __musicianInputTemplate3 =
"\
    }\n";

const char* __albumInputTemplate0 =
"\
    ";

//***		globals

hsList		all;
hsList		query	= NULL;

int main()
	{
		char buffer[500];
		char currentPath[500];
		unsigned int action;

		hsBand insBand;
		size_t count;
		hsMusician	musician;
		char* album;
		hsListItem item;
		hsBand band;
		hsMusician newMusician;
		hsMusician updMusician;
		char* newAlbum;
		//hsListItem item;

		strcpy(currentPath, "default.db");

		all		= __ctor_List();
		query	= NULL;

		action = 0;

		while(action < DB_MAX)
			{
				system("cls");
				printf(mainMenu);

				fseek(stdin, 0, SEEK_SET);	//set to the start of input console buffer
				scanf("%u", &action		);

				switch(--action)
					{
						case DB_LOAD:
							{
								system("cls");
								printf("Enter file path :");
								fseek(stdin, 0, SEEK_SET);
								scanf("%s", currentPath);

								releaseMusicDB(all);

								if(query)
									releaseMusicQuery(query);

								query = NULL;
								all = readMusicDB(currentPath);

								break;
							}
						case DB_SAVE:
							{
								system("cls");
								printf("Enter file path :");
								fseek(stdin, 0, SEEK_SET);
								scanf("%s", currentPath);

								writeMusicDB(currentPath, all);

								break;
							}
						case DB_SELECT:
							{
								system("cls");
								printf(selectMenu);
								fseek(stdin, 0, SEEK_SET);
								scanf("%i", &action);
								system("cls");

								switch(--action)
									{
										case SEL_ALL:
											{
												if(query)
													{
														releaseMusicQuery(query);
														query = NULL;
													}

												printAllBands(all);

												system("pause");
												break;
											}
										case SEL_ORDER:
											{
												if(query)
													releaseMusicQuery(query);

												query = cloneMusicDB(all);
												sortBands(query);
												printAllBands(query);

												system("pause");
												break;
											}
										case SEL_LIKE_BAND_ORDER:
											{
												printf("Enter search substring:");
												fseek(stdin, 0, SEEK_SET);
												scanf("%s", buffer);

												system("cls");

												if(query)
													releaseMusicQuery(query);

												query = findByBandNameSubString(all, buffer);
												sortBands(query);
												printAllBands(query);
												
												system("pause");
												break;
											}
										case SEL_LIKE_MUSICIAN_ORDER:
											{
												printf("Enter search substring:");
												fseek(stdin, 0, SEEK_SET);
												scanf("%s", buffer);

												system("cls");

												if(query)
													releaseMusicQuery(query);

												query = findByMusicianNameSubString(all, buffer);
												sortBands(query);
												printAllBands(query);
												
												system("pause");
												break;
											}
										case SEL_LIKE_ALBUM_ORDER:
											{
												printf("Enter search substring:");
												fseek(stdin, 0, SEEK_SET);
												scanf("%s", buffer);

												system("cls");

												if(query)
													releaseMusicQuery(query);

												query = findByAlbumNameSubString(all, buffer);
												sortBands(query);
												printAllBands(query);
												
												system("pause");
												break;
											}
									}

								action = 0;

								break;
							}
						case DB_INSERT:
							{
								system("cls");

								insBand = malloc(sizeof(sBand));

								printf(__bandInputTemplate0);
								fseek(stdin, 0, SEEK_SET);
								scanf("%s", buffer);

								if(!*buffer)
									{
										free(insBand);
										break;
									}

								insBand->name = malloc(strlen(buffer) + 1);
								strcpy(insBand->name, buffer);							

								printf(__bandInputTemplate1);
								fseek(stdin, 0, SEEK_SET);
								scanf("%i", &count);
								printf(__bandInputTemplate2);

								insBand->musicians = __ctor_List();								

								while(count > 0)
									{
										musician = malloc(sizeof(sMusician));

										printf(__musicianInputTemplate0);
										fseek(stdin, 0, SEEK_SET);
										scanf("%s", buffer);

										musician->name = malloc(strlen(buffer) + 1);
										strcpy(musician->name, buffer);

										printf(__musicianInputTemplate1);
										fseek(stdin, 0, SEEK_SET);
										scanf("%2hhu.%2hhu.%hu", &musician->startDate.day, &musician->startDate.month, &musician->startDate.year);

										musician->endDate = malloc(sizeof(sDate));

										printf(__musicianInputTemplate2);
										fseek(stdin, 0, SEEK_SET);
										scanf("%2hhu.%2hhu.%hu", &(musician->endDate->day), &(musician->endDate->month), &(musician->endDate->year));

										printf(__musicianInputTemplate3);

										insertItemToEnd(insBand->musicians, musician);

										--count;
									}

								printf(__bandInputTemplate3);
								fseek(stdin, 0, SEEK_SET);
								scanf("%i", &count);
								printf(__bandInputTemplate4);

								insBand->albums = __ctor_List();							

								while(count > 0)
									{
										printf(__albumInputTemplate0);
										fseek(stdin, 0, SEEK_SET);
										scanf("%s", buffer);

										if(!*buffer)
											break;

										album = malloc(strlen(buffer) + 1);
										strcpy(album, buffer);

										insertItemToEnd(insBand->albums, album);

										--count;
									}

								printf(__bandInputTemplate5);

								insertItemToEnd(all, insBand);

								system("pause");

								break;
							}
						case DB_UPDATE:
							{
								size_t index;
								size_t currIndex;

								system("cls");
								printf("Enter band index:");
								fseek(stdin, 0, SEEK_SET);
								scanf("%u", &index);
								currIndex = index;						

								if(query)
									item = getFirst(query);
								else
									item = getFirst(all);

								while(index > 0)
									{
										item = getNextItem(item);
										--index;
									}

								if(!item)
									break;

								band = getItemValue(item);

								system("cls");
								printBand(band, currIndex);
								printf("\n");
								printf(updateMenu);
								fseek(stdin, 0, SEEK_SET);
								scanf("%u", &action);

								switch(--action)
									{
										case UPD_NAME:
											{
												system("cls");
												printBand(band, currIndex);

												printf("\nEnter new name:");
												fseek(stdin, 0, SEEK_SET);
												scanf("%s", buffer);

												free(band->name);

												band->name = malloc(strlen(buffer) + 1);
												strcpy(band->name, buffer);

												system("cls");
												printBand(band, currIndex);
												system("pause");

												break;
											}
										case UPD_ADD_MUSICIAN:
											{
												system("cls");

												newMusician = malloc(sizeof(sMusician));

												printf(__musicianInputTemplate0);
												fseek(stdin, 0, SEEK_SET);
												scanf("%s", buffer);

												newMusician->name = malloc(strlen(buffer) + 1);
												strcpy(newMusician->name, buffer);

												printf(__musicianInputTemplate1);
												fseek(stdin, 0, SEEK_SET);
												scanf("%2hhu.%2hhu.%hu", &newMusician->startDate.day, &newMusician->startDate.month, &newMusician->startDate.year);

												printf(__musicianInputTemplate2);
												newMusician->endDate = malloc(sizeof(sDate));

												fseek(stdin, 0, SEEK_SET);
												scanf("%2hhu.%2hhu.%hu", &newMusician->endDate->day, &newMusician->endDate->month, &newMusician->endDate->year);

												printf(__musicianInputTemplate3);

												insertItemToEnd(band->musicians, newMusician);
												system("pause");

												system("cls");
												printBand(band, currIndex);
												system("pause");

												break;
											}
										case UPD_REMOVE_MUSICIAN:
											{
												system("cls");
												printf("Enter musician index:");
												fseek(stdin, 0, SEEK_SET);
												scanf("%u", &index);

												item = getFirst(band->musicians);

												while(item && index > 0)
													{
														item = getNextItem(item);
														--index;
													}

												if(!item)
													break;
												
												removeMusician(band->musicians, item);

												system("cls");
												printBand(band, currIndex);
												system("pause");

												break;
											}
										case UPD_UPDATE_MUSICIAN:
											{
												system("cls");
												printf("Enter musician index:");
												fseek(stdin, 0, SEEK_SET);
												scanf("%u", &index);

												item = getFirst(band->musicians);

												while(item && index > 0)
													{
														item = getNextItem(item);
														--index;
													}

												if(!item)
													break;

												updMusician = getItemValue(item);

												system("cls");
												printf(updateMusicianMenu);
												fseek(stdin, 0, SEEK_SET);
												scanf("%u", &action);

												switch(--action)
													{
														case MUS_UPDATE_NAME:
															{
																system("cls");
																printf("Enter new name:");
																fseek(stdin, 0, SEEK_SET);
																scanf("%s", buffer);

																free(updMusician->name);
																updMusician->name = malloc(strlen(buffer) + 1);
																strcpy(updMusician->name, buffer);

																break;
															}
														case MUS_UPDATE_START_DATE:
															{
																system("cls");
																printf("Enter new start date:");
																fseek(stdin, 0, SEEK_SET);
																scanf("%2hhu.%2hhu.%hu", &updMusician->startDate.day, &updMusician->startDate.month, &updMusician->startDate.year);

																break;
															}
														case MUS_UPDATE_END_DATE:
															{
																if(!updMusician->endDate)
																	updMusician->endDate = malloc(sizeof(sDate));

																system("cls");
																printf("Enter new end date:");
																fseek(stdin, 0, SEEK_SET);
																scanf("%2hhu.%2hhu.%hu", &updMusician->endDate->day, &updMusician->endDate->month, &updMusician->endDate->year);
															}
													}

												system("cls");
												printBand(band, currIndex);
												system("pause");

												break;
											}
										case UPD_ADD_ALBUM:
											{
												system("cls");
												printBand(band, currIndex);

												printf("\nEnter new album name:");
												fseek(stdin, 0, SEEK_SET);
												scanf("%s", buffer);

												newAlbum = malloc(strlen(buffer) + 1);
												strcpy(newAlbum, buffer);

												insertItemToEnd(band->albums, newAlbum);

												system("cls");
												printBand(band, currIndex);
												system("pause");

												break;
											}
										case UPD_REMOVE_ALBUM:
											{
												system("cls");
												printf("Enter album index:");
												fseek(stdin, 0, SEEK_SET);
												scanf("%u", &index);

												item = getFirst(band->albums);

												while(item && index > 0)
													{
														item = getNextItem(item);
														--index;
													}

												if(item)
													removeAlbum(band->albums, item);

												system("cls");
												printBand(band, currIndex);
												system("pause");
											}
										case UPD_SORT_MUSICIANS:
											{
												sortMusicians(band->musicians);

												system("cls");
												printBand(band, currIndex);
												system("pause");

												break;
											}
										case UPD_SORT_ALBUMS:
											{
												sortAlbums(band->albums);

												system("cls");
												printBand(band, currIndex);
												system("pause");
											}
									}

								action = 0;

								break;
							}
						case DB_DELETE:
							{
								size_t index;

								system("cls");
								printf("Enter band index:");
								fseek(stdin, 0, SEEK_SET);
								scanf("%u", &index);						

								if(query)
									{
										item = getFirst(query);

										while(item && index > 0)
											{
												item = getNextItem(item);

												--index;
											}

										if(item)
											{
												hsBand		band	= getItemValue(item);
												hsListItem	allItem = getFirst(all);

												while(allItem && getItemValue(allItem) != band)
													allItem = getNextItem(allItem);

												if(allItem)
													removeBand(all, allItem);

												removeItemNoRelease(query, item);
											}
									}
								else
									{
										item = getFirst(all);

										while(item && index > 0)
											{
												item = getNextItem(item);

												--index;
											}

										if(item)
											removeBand(all, item);
									}

								system("cls");
								printf("Item has been removed!");
								system("pause");

								break;
							}
						case DB_COMMIT:
							{
								writeMusicDB(currentPath, all);

								system("cls");
								printf("Changes committed to hard drive!\n");
								system("pause");

								break;
							}
						case DB_RELOAD:
							{
								hFILE file;

								if(file = fopen(currentPath, "rb")) //if file exists we can reload it
									{
										fclose(file);
										releaseMusicDB(all);

										if(query)
											releaseMusicQuery(query);

										query = NULL;
										all = readMusicDB(currentPath);
									}
							}
					}
			}

		releaseMusicDB(all);
		
		if(query)
			releaseMusicQuery(query);

		return 0;
	}