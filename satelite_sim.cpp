//Description:
//Console program that simulates satellite orbit around planet.

//---- Libraries:
#include <iostream>
#include <windows.h>
#include <math.h>
#include <conio.h>

using namespace std;

//---- Constants:

    //- Colors:
enum {
    BLACK             = 0,
    DARKBLUE          = FOREGROUND_BLUE,
    DARKGREEN         = FOREGROUND_GREEN,
    DARKCYAN          = FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARKRED           = FOREGROUND_RED,
    DARKMAGENTA       = FOREGROUND_RED | FOREGROUND_BLUE,
    DARKYELLOW        = FOREGROUND_RED | FOREGROUND_GREEN,
    DARKGRAY          = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    GRAY              = FOREGROUND_INTENSITY,
    BLUE              = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    GREEN             = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CYAN              = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    RED               = FOREGROUND_INTENSITY | FOREGROUND_RED,
    MAGENTA           = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    YELLOW            = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    WHITE             = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

    //Indexes of axes:
    IND_X             = 0,
    IND_Y             = 1
};

    //- Bounding box:

            //Maximum size:
const int BOX_MAX_X = 100;
const int BOX_MAX_Y = 50;
const int BOX_MAX[] = {BOX_MAX_X, BOX_MAX_Y};
            //Minimum size:
const int BOX_MIN_X = 0;
const int BOX_MIN_Y = 1;
const int BOX_MIN[] = {BOX_MIN_Y, BOX_MIN_Y};

    //- Planet dimensions:
const int RADIUS = 10;
const int EDGE_X = BOX_MAX_X/2 - RADIUS ;
const int EDGE_Y = BOX_MAX_Y/2 - RADIUS ;
const float CENTER_POS[] = { EDGE_X + RADIUS, EDGE_Y + RADIUS };
            //Percent of radius with ice:
const float ICE_CAP = 0.5;

    //- Simulation update time:

const int MAX_GRAV = 100;
            //Update time step
const int TIME_INT = 100;

    //- Timed messages:

            //Messages
const string message[] = { "Happy B-DAY", "Success!", "GLORY TO KHORNE!", "FREEDOM" };
            //Color per message
const short COLORS[] = { WHITE, YELLOW, BLUE, GREEN };
            //Time per message
const int TIMES[] = { 20, 40, 50, 60};
const int N_MES = sizeof( message )/sizeof( message[0] );

    //- Data input:
const int LEN_WIPE = 30;



//---- Functions:

    //-- Console manipulation:

void setxy( short x, short y) {
	COORD c = { x , y };
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), c );
}

//-

void setColor( short text_color, short bg_color ) {
    SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), text_color | 16*bg_color );
}

//-

void AddSpace( int len, short x, short y ) {
    setxy(x, y);
    for( int index = 0; index < len; index += 1 ) {
        cout << " ";
    }
    setxy(x, y);
}

    //-- Graphics:

void makeEarth( short edge_x, short edge_y, short radius ) {;
    for( int y = 0; y <= 2*radius; y += 1 ) {
        int width = sqrt( 1 - pow( float(y - radius)/float(radius) , 2 ) ) * radius;
        setxy( edge_x + radius - width, edge_y + y );
        for( int x = 0; x <= 2*width; x += 1 ) {
            int land = rand()%8 ;
            if( land == 0 ) {
                setColor(WHITE, GREEN);
            } else if( land == 1) {
                setColor(WHITE, BLUE);
            }
            if( y < int(radius*ICE_CAP) || y > int(radius*(2 - ICE_CAP)) ) {
                if( land == 2 ) {
                    setColor(WHITE, WHITE);
                }
            } else {
                if ( land == 3 ) {
                    setColor(WHITE, DARKGREEN);
                }
            }
            cout << " ";
        }
    }
    setColor(WHITE, BLACK);
    setxy( 0, 0);
}

    //-- Orbit simulation:

void updateOrbit( float pos_init[], float vel_init[], float coef_res, float grav_scalar ) {

    static float pos[] = { pos_init[IND_X] , pos_init[IND_Y] };
    static float vel[] = { vel_init[IND_X] , vel_init[IND_Y] };

    static float force[] = {0,0};
    static float diff[] = {0,0};
    static bool impact = false;

    for( int index = 0; index < 2; index += 1 ) {
        diff[index] = pos[index] - CENTER_POS[index];
    }

    float force_mag = -grav_scalar/( pow( diff[IND_X] , 2 ) + pow( diff[IND_Y], 2 ) );
    float angle = atan2( diff[IND_Y], diff[IND_X] );

    force[IND_X] = force_mag*cos(angle);
    force[IND_Y] = force_mag*sin(angle);

    for( int index = 0; index < 2 ; index += 1 ) {

        vel[index] += force[index];
        pos[index] += vel[index];

        if( pos[index] >= BOX_MAX[index] ) {
            pos[index] = BOX_MAX[index];
            vel[index] = -vel[index];
        }
        if( pos[index] <= BOX_MIN[index] ) {
            pos[index] = BOX_MIN[index];
            vel[index] = -vel[index];
        }
        diff[index] = pos[index] - CENTER_POS[index];
    }

    float rad = sqrt( pow(diff[IND_X],2) + pow(diff[IND_Y],2) );

    if( rad <= RADIUS+1 ) {
        pos[IND_X] = (RADIUS+1)*cos(angle) + EDGE_X + RADIUS;
        pos[IND_Y] = (RADIUS+1)*sin(angle) + EDGE_Y + RADIUS;
        vel[IND_X] = -vel[IND_X]*coef_res;
        vel[IND_Y] = -vel[IND_Y]*coef_res;
        //-- explosion:
        impact = true;
    }

    setxy( short(pos[IND_X]), short(pos[IND_Y]) );

    if( impact == true ) {
        impact = false;
        setColor( YELLOW, DARKRED );
        cout << "X";
    } else {
        setColor( BLACK, GRAY );
        cout << "O";
        setColor( WHITE, BLACK );
    }

    Sleep( TIME_INT );
    AddSpace( 1 , short(pos[IND_X]) , short(pos[IND_Y]) );
}

    //-- Initial conditions of orbit:

void getInputs( float pos_init[], float vel_init[], float *coef_res, float *grav_scalar ) {

    bool check;

    setxy(0,1);

    setColor(CYAN, BLACK);
    cout << "Initial position:" << endl;
    setColor(WHITE, BLACK);

    for( int index = 0; index < 2; index += 1) {
        check = false;
        char axis[] = {'X','Y'};
        do {
            cout << axis[index] << "= ";
            cin >> pos_init[index];
            if( pos_init[index] < BOX_MIN[index] || pos_init[index] > BOX_MAX[index] ) {
                AddSpace( LEN_WIPE , 0, 2 + index );
            } else {
                check = true;
            }
        } while ( check == false );
    }
    //-
    setColor(CYAN, BLACK);
    cout << "Initial velocity:" << endl;
    setColor(WHITE, BLACK);

    for( int index = 0; index < 2; index += 1) {
        check = false;
        char axis[] = {'x', 'y'};
        do {
            cout << "V" << axis[index] << "= ";
            cin >> vel_init[index];
            if( abs(vel_init[index]) > BOX_MAX[index] ) {
                AddSpace( LEN_WIPE , 0, 5 + index );
            } else {
                check = true;
            }
        } while ( check == false );
    }
    //-
    check = false;

    setColor(CYAN, BLACK);
    cout << "Restitution coefficient: " << endl;
    setColor(WHITE, BLACK);

    do {
        cout << "e = ";
        cin >> *coef_res;
        if( *coef_res < 0 || *coef_res > 1 ) {
            AddSpace( LEN_WIPE, 0, 8 );
        } else {
            check = true;
        }
    } while( check == false );

    //-
    check = false;

    setColor(CYAN, BLACK);
    cout << "Gravity strength: " << endl;
    setColor(WHITE, BLACK);

    do {
        cout << "g = ";
        cin >> *grav_scalar;
        if( *grav_scalar < 0 || *grav_scalar > MAX_GRAV ) {
            AddSpace( LEN_WIPE, 0, 10 );
        } else {
            check = true;
        }
    } while( check == false );

    //-
    setColor(RED, BLACK);
    cout << "Press key to begin...";
    setColor(WHITE, BLACK);

    getch();
    for( int index = 1; index <= 11; index += 1) {
        AddSpace( LEN_WIPE , 0, index);
    }
}

    //-- Messages at the center of planet:

void TimedMessage( int timer ) {

    static bool on[N_MES] = {false};

    for( int index = 0; index < N_MES; index += 1 ) {
        if( timer > TIMES[index] && on[index] == false ) {
            on[index] = true;

            setColor( WHITE, BLUE );
            if( index > 0 ) {
                AddSpace( message[index-1].length() , CENTER_POS[IND_X] - message[index-1].length()/2, CENTER_POS[IND_Y] );
            }

            setColor( COLORS[index], RED );
            setxy( CENTER_POS[IND_X] - message[index].length()/2, CENTER_POS[IND_Y] );
            cout << message[index];
        }
    }
    setColor( WHITE, BLACK );
}

    //-- Initial message:

void titleHeader( void ) {
    string tittle = "Meteor simulator!";
    setColor( WHITE, RED );
    cout << tittle;

    setxy( tittle.length() + 5, 0 );
    setColor( GREEN, BLACK );
    cout << "This was a motherfucker to make. Hope you enjoy it! try getting into orbit :)";
    setColor( WHITE, BLACK);
}


//---- Main function:

int main() {

    float pos_init[] = {0, 0};
    float vel_init[] = {0,0};
    float coef_res = 0;
    float grav_scalar = 0;

    titleHeader();

    makeEarth( EDGE_X, EDGE_Y, RADIUS);

    getInputs( pos_init, vel_init, &coef_res, &grav_scalar );

    int timer = 0;

    while( true ) {

        updateOrbit( pos_init, vel_init, coef_res, grav_scalar );
        TimedMessage(timer);
        //-
        timer += 1;
    }
    return 0;
}
