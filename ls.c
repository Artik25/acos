#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

void look( char *path ) 
{
	DIR *dir = opendir( path );
	struct stat inf;
	struct dirent *entry;
	char newPath[ PATH_MAX + 1 ];
	
	while( ( entry = readdir( dir )) != NULL ) 
	{ 	
		if ( entry->d_name[0] == '.' ) continue;

		strncpy( newPath, path, PATH_MAX );
		strncat( newPath, entry->d_name, PATH_MAX );
		
		if ( !strcmp( path, "." ) )
			strncpy( newPath, entry->d_name, PATH_MAX ); 		
 
		if( stat( newPath, &inf ) < 0 )
		{
			fprintf( stderr, "Cannot read file '%s'\n", newPath );
			exit(1);
		}
		
		if( inf.st_mode & S_IFDIR )
		{
   	               	printf( "%s:\n", entry->d_name );
			strncat( newPath, "/", PATH_MAX );
			look( newPath );
		}

                if( inf.st_mode & S_IFREG )
                       	printf( "----%8s %d %s\n", entry->d_name, inf.st_size, ctime( &inf.st_ctime) );
	}
}
		
int main( int argc, char ** argv ) 
{	
	DIR *dir;
	struct stat inf;
	struct dirent *entry;
	
	if ( argc < 3 )
	{	
		if ( argc == 2 )
		{	
		        if ( !strcmp( argv[1], "-R" ) )
			{
 				look( "." );
				exit(0);
			}

			stat( argv[1], &inf );
			
			if ( inf.st_mode & S_IFREG )
			{
				printf( "FILE   %8s %d %s\n", argv[1], inf.st_size, ctime( &inf.st_ctime) );
				exit(0);
			}
			else
				dir = opendir( argv[1] );
		}
		else
			dir = opendir( "." );
		
		if ( !dir )
		{
			fprintf ( stderr, "Cannot open file '%s'\n", argv[1] );
			exit(1);
		}
		
		while ( ( entry = readdir( dir )) != NULL )
		{
			char path[ PATH_MAX + 1 ];
			strncpy( path, entry->d_name, PATH_MAX );
			if ( argc == 2 )
			{
				strncpy( path, argv[1], PATH_MAX );
				strncat( path, "/", PATH_MAX );
				strncat( path, entry->d_name, PATH_MAX );
			}

			if ( stat( path, &inf ) < 0 )
       			{	
                		fprintf ( stderr, "ls: Cannot open file\n");
                		exit(1);
       			}
			
			if( inf.st_mode & S_IFDIR )
				printf( "DIRECT %8s %d %s\n", entry->d_name, inf.st_size, ctime( &inf.st_ctime) );
			if( inf.st_mode & S_IFREG )
                                printf( "FILE   %8s %d %s\n", entry->d_name, inf.st_size, ctime( &inf.st_ctime) );

		}
	}

	else
	{
		if ( !strcmp( argv[1], "-R" ) ) 
		{
			stat( argv[2], &inf );

                        if ( inf.st_mode & S_IFREG )
                        {
                                printf( stderr, "It's not a directory %s\n", argv[2] );
                                exit(1);
                        }
			
			look( argv[2] );
		}
	}
	exit(0);
}
