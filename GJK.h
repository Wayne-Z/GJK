#pragma once

//Kevin's implementation of the Gilbert-Johnson-Keerthi intersection algorithm
//Most useful references (Huge thanks to all the authors):

// "Implementing GJK" by Casey Muratori:
// The best description of the algorithm from the ground up
// https://www.youtube.com/watch?v=Qupqu1xe7Io

// "Implementing a GJK Intersection Query" by Phill Djonov
// Interesting tips for implementing the algorithm
// http://vec3.ca/gjk/implementation/

// "GJK Algorithm 3D by Sergiu Craitoiu
// Has nice diagrams to visualise the tetrahedral case
// http://in2gpu.com/2014/05/18/gjk-algorithm-3d/

vec3 support(vec3 points[], int num_points, vec3 dir);
bool gjk(Collider coll1, Collider coll2);
void update_simplex3(vec3 &a, vec3 &b, vec3 &c, vec3 &d, int &i, vec3 &search_dir);
bool update_simplex4(vec3 &a, vec3 &b, vec3 &c, vec3 &d, int &i, vec3 &search_dir);

bool gjk(Collider coll1, Collider coll2){
    vec3 a, b, c, d; //Simple just a set of points (a is always most recently added)
    vec3 search_dir = coll1.pos - coll2.pos; //initial search direction between colliders

    //Get initial point for simplex
    c = support(coll1, search_dir) - support(coll2, -search_dir);
    search_dir = -c; //search in direction of origin

    //Get second point for a line segment simplex
    b = support(coll1, search_dir) - support(coll2, -search_dir);
    if(dot(b, search_dir)<0) return false; //we didn't reach the origin, won't enclose it
    search_dir = cross(cross(c-b,-b),c-b); //search normal to line segment

    int i = 2; //int to keep track of dimensionality of simplex

    for(;;){
        a = support(coll1, search_dir) - support(coll2, -search_dir);
        if(dot(a, search_dir)<0) return false; //we didn't reach the origin, won't enclose it
        i++;

        if(i==3){
            update_simplex3(a,b,c,d,i,search_dir);
        }
        else if(i==4){
            if(update_simplex4(a,b,c,d,i,search_dir)) return true;
        }
        else { //Should never get here
            printf("WARNING GJK: i = %d\n", i);
            getchar(); //Pause program so we can see how we ended up here!
        }
    }
}

//Triangle case
void update_simplex3(vec3 &a, vec3 &b, vec3 &c, vec3 &d, int &i, vec3 &search_dir){
    /*  
    // Required winding order:
    //  b
    //  | \
    //  |   \
    //  |     \ a
    //  |     /
    //  |   /
    //  | /
    //  c
    */
    vec3 n = cross(b-a, c-a); //triangle's normal
    vec3 AO = a*(-1); //direction to origin

    //Determine which facet is closest to origin, make that new simplex

    i = 2; //hoisting this just cause
    //Closest to edge AB
    if(dot(cross(b-a, n), AO)>0){
        c = a;
        //i = 2;
        search dir = cross(cross(b-a, AO), b-a);
        return;
    }
    //Closest to edge AC
    if(dot(cross(n, c-a), AO)>0){
        b = a;
        //i = 2;
        search dir = cross(cross(c-a, AO), c-a);
        return;
    }
    
    i = 3; //hoisting this just cause
    //Above triangle
    if(dot(n, AO)>0){
        d = c;
        c = b;
        b = a;
        //i = 3;
        search_dir = n;
        return;
    }
    //else
    //Below triangle
    d = b;
    b = a;
    //i = 3;
    search_dir = -n;
    return;
}

//Tetrahedral case
bool update_simplex4(vec3 &a, vec3 &b, vec3 &c, vec3 &d, int &i, vec3 &search_dir){
    // a is peak/tip of pyramid, BCD is the base (counterclockwise winding order)

    //Get normals of three new faces
    vec3 ABC = cross(b-a, c-a);
    vec3 ACD = cross(c-a, d-a);
    vec3 ADB = cross(d-a, b-a);

    vec3 AO = a*(-1); //dir to origin

    //Plane-test origin with 3 faces
    bool d1 = (dot(ABC, AO)>0);
    bool d2 = (dot(ACD, AO)>0);
    bool d3 = (dot(ADB, AO)>0);

    if(!d1 && !d2 && !d3)) return true;

    //Determine which facet is closest to origin, make that new simplex
    
    return false;
}
