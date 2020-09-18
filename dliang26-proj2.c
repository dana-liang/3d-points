/* CS211 Summer 2020 Project 2
	
   	dliang26-proj2.c
	By Dana Liang, 5/25/2020
        This program maintains a collection of points in 3D space and information 
        about those points (maximums and minimums); this information will be used 
        to test whether a given separate point intersects with this collection of points	

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

/* Point3d data type */
typedef struct 
{
     int ID; 		// serial number for each vertex
     double X, Y, Z; 	// coordinates for the 3D point
     double luminosity; // brightness of vertex, ranges from 0.0 - 1.0
} Point3d;

/* PointCloud data type */
typedef struct 
{
     Point3d * points; 		// array to hold all points in the PointCloud (grow as needed)
     int allocated; 		// keep track of how much space has been allocated in the points array
     int nPoints; 		// keep track of how many points are in the points array
     double luminosity; 	// average luminosity of the points in the PointCloud (0 if 0 points inside)
     double xMin, xMax, yMin, yMax, zMin, zMax; // minimum and maximum values of all points (all 0 if 0 points inside)
} PointCloud;


/***********************
 * FUNCTION PROTOTYPES *
 ***********************/

int addPointToCloud( PointCloud* cloud, Point3d point );
int intersection( PointCloud cloud, Point3d point );

/********************
 *       MAIN       *
 ********************/

int main(int argc, char const *argv[])
{

    /* Global variable debugMode: if -d argument is given, additional information should be printed out as the program runs. */
    int debugMode;
    for ( int i = 0; i < argc; i++ )
    {
         if ( strcmp ("-d", argv[i]) == 0 )
		 debugMode = TRUE;
	 else
		 debugMode = FALSE;
    }
    
    /* Allocate space for a PointCloud, and an array of Point3d inside PointCloud */
    PointCloud *cloud;
    cloud = (PointCloud*) malloc ( sizeof(PointCloud) );

    cloud->allocated = 100;
    cloud->points = (Point3d*) malloc ( cloud->allocated * sizeof(Point3d) );

    Point3d point;
    
    printf("Enter a list of X, Y, and Z values followed by a luminosity value. Terminate your list with a negative luminosity value:\n");
    scanf("%lf", & (point.X) );
    scanf("%lf", & (point.Y) );
    scanf("%lf", & (point.Z) );
    scanf("%lf", & (point.luminosity) );
 
    /* Read data for a series of points from standard input until a negative luminosity is encountered */
    int i = 0; /* Keep track of array indices and size */
    while ( point.luminosity >= 0 )
    {
	point.ID = i;

	addPointToCloud( cloud, point );

	i++;

	/* Double the array space if more points are entered than initially allocated */
	if ( (i+1) == cloud->allocated )
	{
	    Point3d* temp = cloud->points;
	    cloud->points = (Point3d*) malloc ( i * 2 * sizeof(Point3d) );
	    for ( int j = 0; j < i; j++ )
		    cloud->points[j] = temp[j];
	    free(temp);
	    cloud->allocated = cloud->allocated * 2;
	}
	
	/* get next values */
	scanf("%lf", & (point.X) );
	scanf("%lf", & (point.Y) );
	scanf("%lf", & (point.Z) );
	scanf("%lf", & (point.luminosity) );
    }

    cloud->nPoints = i;

    /* Debug: check if all values are read into the cloud->points array correctly */
    if ( debugMode == TRUE )
    {
        for ( int k = 0; k < i; k++ )
		printf("Point %d: (%.2f, %.2f, %.2f)		Lum: %.2f\n", cloud->points[k].ID, cloud->points[k].X, 
				cloud->points[k].Y, cloud->points[k].Z, cloud->points[k].luminosity);
	printf("Total # of Points: %d\n", cloud->nPoints);
    }
    

    /* Determine the upper and lower bounds of the bounding box and the average luminosity of the point cloud.
     * Store these in the PointCloud structure.*/
    /* Report the bounds of the Point Cloud, the average luminosity, the number of points in the Point Cloud */
    
    cloud->xMin = cloud->points[0].X;
    cloud->xMax = cloud->points[0].X;
    cloud->yMin = cloud->points[0].Y;
    cloud->yMax = cloud->points[0].Y;
    cloud->zMin = cloud->points[0].Z;
    cloud->zMax = cloud->points[0].Z;
    cloud->luminosity = 0;
    
    for ( i = 0; i < cloud->nPoints; i++ )
    {
        if ( cloud->points[i].X > cloud->xMax )
            cloud->xMax = cloud->points[i].X;

        if ( cloud->points[i].X < cloud->xMin )
	        cloud->xMin = cloud->points[i].X;

        if ( cloud->points[i].Y > cloud->yMax )
            cloud->yMax = cloud->points[i].Y;

        if ( cloud->points[i].Y < cloud->yMin )
	        cloud->yMin = cloud->points[i].Y;

        if ( cloud->points[i].Z > cloud->zMax )
            cloud->zMax = cloud->points[i].Z;
            
        if ( cloud->points[i].Z < cloud->zMin )
	        cloud->zMin = cloud->points[i].Z;

	cloud->luminosity = cloud->luminosity + cloud->points[i].luminosity;
    }

    cloud->luminosity = cloud->luminosity / (double)cloud->nPoints; 

    printf("\nBounding Box:\n");
    printf("X:	Lower: %.2f	Upper: %.2f\n", cloud->xMin, cloud->xMax);
    printf("Y:	Lower: %.2f	Upper: %.2f\n", cloud->yMin, cloud->yMax);
    printf("Z:	Lower: %.2f	Upper: %.2f\n", cloud->zMin, cloud->zMax);
    printf("Average Luminosity (nearest hundredth): %.2f\n", cloud->luminosity);

    /* Read data for a second set of points and for each point, 
     * determine whether this point intersects with the PointCloud or not.*/
    /* Read data until a negative luminosity is encountered */

    printf("\nEnter a list of X, Y, and Z values followed by a luminosity value to check if it intersects with the bounding box.\n");
    printf("Terminate your list with a negative luminosity value:\n");
    scanf("%lf", &point.X);
    scanf("%lf", &point.Y);
    scanf("%lf", &point.Z);
    scanf("%lf", &point.luminosity);

    while ( point.luminosity >= 0 )
    {
        if ( intersection ( *cloud, point ) ) 
		printf("(%.2f, %.2f, %.2f) intersects with the PointCloud.\n", point.X, point.Y, point.Z);
	else
		printf("(%.2f, %.2f, %.2f) does not intersect with the PointCloud.\n", point.X, point.Y, point.Z);

	/* Get next values */
	scanf("%lf", &point.X);
        scanf("%lf", &point.Y);
        scanf("%lf", &point.Z);
        scanf("%lf", &point.luminosity);
    }

    /* Free all dynamically allocated memory before the program ends. */
    free(cloud);
    free(cloud->points);

    return 0;

}

/************************
 * FUNCTION DEFINITIONS *
 ************************/

/* Returns 0 if the point is successfully added to the cloud or -1 otherwise. */
int addPointToCloud( PointCloud* cloud, Point3d point )
{
    if ( point.ID < cloud->allocated )
    {
        cloud->points[point.ID] = point;
	return 0;
    }
    
    else
        return -1;
}

/* Returns 0 ( false ) if the cloud has no points, otherwise check if the point's coordinates are
   within or equal to the bounds of the cloud.
   Return 1 ( true ) if there is an intersection and false otherwise */
int intersection ( PointCloud cloud, Point3d point )
{
    if ( point.X <= cloud.xMax && point.X >= cloud.xMin &&
         point.Y <= cloud.yMax && point.Y >= cloud.yMin &&
         point.Z <= cloud.zMax && point.Z >= cloud.zMin )
	    return 1;
    else
	    return 0;
}
