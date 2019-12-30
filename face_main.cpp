#include <Titan/sim.h>
#include <iostream>
#include <list>
#include <iterator>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
void make_grid(Simulation *sim);
void make_rows(Simulation *sim);
void make_columns(Simulation *sim);
void set_all_k(Simulation *sim);
void make_muscles(Simulation *sim);
void make_r1(Simulation *sim);
void make_r2(Simulation *sim);
void make_r3(Simulation *sim);
void make_r4(Simulation *sim);
void rest(Simulation *sim);
Container * make_rec_container(int start, Simulation *sim);
Container * make_rec_vert(int start, Simulation *sim);
 

vector <Mass *> mass_vec;
vector <Spring *> spring_vec;
vector <Spring *> spring_vec_2;
vector <Spring *> full_springs;
int dim = 50;     

int main(){
    
   Simulation sim;	
   make_grid(&sim);
   set_all_k(&sim);
   //sim.start();
   make_muscles(&sim);
   rest(&sim);
   // make_recs(50);
}

void make_grid(Simulation *sim){
    
    //for face-on view
    sim->setViewport(Vec(70,0,0), Vec(0, 0, 1), Vec(0, 0, 1));
    //turn off gravity
    sim->setGlobalAcceleration(Vec(0,0,0));

    int low = (dim/2) * -1;
    int high = dim/2;
    int count = 0;

    for(int i = low; i < high; i++){
        for(int j = low; j < high; j++){
	    Mass * x = sim->createMass(Vec(1,j,-i));
	    //fixing the edges 
	    if ( (i == low) || (j == low) || (i == high-1) || (j == high-1) ){
	        x->fix();
            }	    
	    //fixing the corners
	    if( (i == low and j == low) || (i == low and j == high-1) || 
	    		    (j == low and i == high-1) || (i == high-1 and j == high-1) ){
	       x->fix();    
	    }
	    x->damping = .99;
	    mass_vec.push_back(x);
	    count++;
        }

    }
    make_rows(sim);
    make_columns(sim);
}

void make_rows(Simulation *sim){
   
    /** making rows with good indexing **/
    int row = 0;
    for(int i = 0; i < dim; i++){
	for(int j = 0; j < dim-1; j++){ 
	    //scale by row*dim to make all the rows
            Spring * x = sim->createSpring( mass_vec[ j + (row*dim) ], mass_vec[ j + (row*dim) + 1 ] ) ;
	    spring_vec.push_back(x);
	    full_springs.push_back(x);
	}
	row++;
    }
}

void make_columns(Simulation * sim){


    /** making columns with good indexing **/
    int c = 0;
    for (int i = 0; i < dim; i++){
        for(int j = 0; j < mass_vec.size(); j = j + dim){
	    Spring * x = sim->createSpring(mass_vec[i+j], mass_vec[i+j+dim]);
	    spring_vec_2.push_back(x);
	    full_springs.push_back(x);
	    c++;
	}
    }
}

void set_all_k(Simulation *sim){
    
    for (int i = 0; i < full_springs.size(); i++){
         full_springs[i]->_k = 800;
    }
}

void make_muscles(Simulation *sim){
    
    make_r1(sim);
    make_r2(sim);
    make_r3(sim);
    make_r4(sim);    

}

/**
 * Make first rec
 **/

void make_r1(Simulation *sim){
    int start = 12; 
    Container * r1 = make_rec_container(12, sim);
    Container * r1_vert = make_rec_vert(12, sim);
    r1_vert->setRestLengths(2);
}

/**
* Make second rec
**/

void make_r2(Simulation *sim){
    int start = 20; 
    Container * r2 = make_rec_container(20, sim);
    Container * r2_vert = make_rec_vert(20, sim);
    r2_vert->setRestLengths(2);
}

/**
* Make third rec
**/

void make_r3(Simulation *sim){
    int start = 28; 
    Container * r3 = make_rec_container(28, sim);
    Container * r3_vert = make_rec_vert(28, sim);
    r3_vert->setRestLengths(2);
}


/**
* Make fourth rec
**/

void make_r4(Simulation *sim){
    int start = 36; 
    Container * r4 = make_rec_container(36, sim);
    Container * r4_vert = make_rec_vert(36, sim);
    r4_vert->setRestLengths(2);
}

Container * make_rec_container(int start, Simulation *sim){

    Container * r = sim->createContainer();
    int init = start + ((dim-1) * 14);
    for (int i = init; i < init + 3; i++){
        (spring_vec[  i  ]->_left)->color = Vec(1.0, 1.0, 1.0);
        (spring_vec[  i  ]->_right)->color = Vec(1.0, 1.0, 1.0);
        r->add(spring_vec[  i   ]);
        r->add( spring_vec[  i   ]->_left );
        r->add( spring_vec[  i   ]->_right );	    
    }

    int init2 = start + ((dim-1) * 15);
    for (int i = init2; i < init2 + 3; i++){
	(spring_vec[i]->_left)->color = Vec(1.0, 1.0, 1.0);
        (spring_vec[i]->_right)->color = Vec(1.0, 1.0, 1.0);
        r->add(spring_vec[i]);
        r->add( spring_vec[i]->_left );
        r->add( spring_vec[i]->_right );
 
    }

    return r;

}

Container * make_rec_vert(int start, Simulation *sim){
    
    Container * r = sim->createContainer();   	
    for (int i=start; i < start+4; i++){
        (spring_vec_2[   (i*50) + 14  ]->_left)->color = Vec(1,1,1);
        (spring_vec_2[   (i*50) + 14  ]->_right)->color = Vec(1,1,1);
        r->add(spring_vec_2[ (i*50) + 14 ]);
	(spring_vec_2[ (i*50) + 14 ]->_right)->fix();
        r->add(spring_vec_2[ (i*50) + 14 ]->_left);
        r->add(spring_vec_2[ (i*50) + 14 ]->_right);
    }

    return r;
    
}

void rest(Simulation *sim){

    /**
     * Make left eyelid up 
     * */

    Container * e_l_u = sim->createContainer();
    int init9 = 13 + ( (dim-1) * 18 );
    for (int i = init9; i < init9 + 6; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0, 0, 1.0);
        (spring_vec[  i  ]->_right)->color = Vec(0, 0, 1.0);
        e_l_u->add(spring_vec[  i   ]);
        e_l_u->add( spring_vec[  i   ]->_left );
        e_l_u->add( spring_vec[  i   ]->_right );
    }

    vector <int> index_vec;
    index_vec.push_back(12 + ((dim-1) * 19));
    index_vec.push_back(19 + ((dim-1) * 19));
    index_vec.push_back(16 + ((dim-1) * 17));
    index_vec.push_back(15 + ((dim-1) * 17));
    for (int x : index_vec){
        (spring_vec[  x ]->_left)->color = Vec(0, 0, 1.0);
        (spring_vec[  x  ]->_right)->color = Vec(0, 0, 1.0);
        e_l_u->add(spring_vec[  x   ]);
        e_l_u->add( spring_vec[  x   ]->_left );
        e_l_u->add( spring_vec[  x   ]->_right );
    }



    /**
     * Make diagonal rec between left eye/eyebrow
     * */


    /**
     * Make diagonal rec between right eye/eyebrow
     * */


    /**
     * Make right eyelid up
     * */

    Container * e_r_u = sim->createContainer();
    int init10 = 32 + ( (dim-1) * 18 );
    for (int i = init10; i < init10 + 6; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0,0,1.000);
        (spring_vec[  i  ]->_right)->color = Vec(0,0,1.000);
        e_r_u->add(spring_vec[  i   ]);
        e_r_u->add( spring_vec[  i   ]->_left );
        e_r_u->add( spring_vec[  i   ]->_right );
    }

    vector <int> index_vec1;
    index_vec1.push_back(31 + ((dim-1) * 19));
    index_vec1.push_back(38 + ((dim-1) * 19));
    index_vec1.push_back(35 + ((dim-1) * 17));
    index_vec1.push_back(34 + ((dim-1) * 17));
    for (int x : index_vec1){
        (spring_vec[  x ]->_left)->color = Vec(0,0,1.000);
        (spring_vec[  x  ]->_right)->color = Vec(0,0,1.000);
        e_r_u->add(spring_vec[  x   ]);
        e_r_u->add( spring_vec[  x   ]->_left );
        e_r_u->add( spring_vec[  x   ]->_right );
    }


    /**
     * Make left eyelid down 
     * */

    Container * e_l_d = sim->createContainer();
    int init11 = 13 + ( (dim-1) * 21 );
    for (int i = init11; i < init11 + 6; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.194,0.398,0.850);
        (spring_vec[  i  ]->_right)->color = Vec(0.194,0.398,0.850);
        e_l_d->add(spring_vec[  i   ]);
        e_l_d->add( spring_vec[  i   ]->_left );
        e_l_d->add( spring_vec[  i   ]->_right );
    }

    vector <int> index_vec2;
    index_vec2.push_back(12 + ((dim-1) * 20));
    index_vec2.push_back(19 + ((dim-1) * 20));
    index_vec2.push_back(16 + ((dim-1) * 22));
    index_vec2.push_back(15 + ((dim-1) * 22));
    for (int x : index_vec2){
        (spring_vec[  x ]->_left)->color = Vec(0.194,0.398,0.850);
        (spring_vec[  x  ]->_right)->color = Vec(0.194,0.398,0.850);
        e_l_u->add(spring_vec[  x   ]);
        e_l_u->add( spring_vec[  x   ]->_left );
        e_l_u->add( spring_vec[  x   ]->_right );
    }


    /**
     * Make right eyelid down
     * */

    Container * e_r_d = sim->createContainer();
    int init12 = 32 + ( (dim-1) * 21 );
    for (int i = init12; i < init12 + 6; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.194,0.398,0.850);
        (spring_vec[  i  ]->_right)->color = Vec(0.194,0.398,0.850);
        e_r_d->add(spring_vec[  i   ]);
        e_r_d->add( spring_vec[  i   ]->_left );
        e_r_d->add( spring_vec[  i   ]->_right );
    }

    vector <int> index_vec3;
    index_vec3.push_back(31 + ((dim-1) * 20));
    index_vec3.push_back(38 + ((dim-1) * 20));
    index_vec3.push_back(35 + ((dim-1) * 22));
    index_vec3.push_back(34 + ((dim-1) * 22));
    for (int x : index_vec3){
        (spring_vec[  x ]->_left)->color = Vec(0.194,0.398,0.850);
        (spring_vec[  x  ]->_right)->color = Vec(0.194,0.398,0.850);
        e_l_u->add(spring_vec[  x   ]);
        e_l_u->add( spring_vec[  x   ]->_left );
        e_l_u->add( spring_vec[  x   ]->_right );
    }

    /**
     * Make left cheek
     * */

    Container * c_l = sim->createContainer();
    int init13 = 13 + ( (dim-1) * 24 );
    for (int i = init13; i < init13 + 6; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.340,0.850,0.614);
        (spring_vec[  i  ]->_right)->color = Vec(0.340,0.850,0.614);
        c_l->add(spring_vec[  i   ]);
        c_l->add( spring_vec[  i   ]->_left );
        c_l->add( spring_vec[  i   ]->_right );
    }
    int init14 = 13 + ( (dim-1) * 25 );
    for (int i = init14; i < init14 + 6; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.340,0.850,0.614);
        (spring_vec[  i  ]->_right)->color = Vec(0.340,0.850,0.614);
        c_l->add(spring_vec[  i   ]);
        c_l->add( spring_vec[  i   ]->_left );
        c_l->add( spring_vec[  i   ]->_right );
    }
    int init15 = 13 + ( (dim-1) * 26 );
    for (int i = init15; i < init15 + 6; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.340,0.850,0.614);
        (spring_vec[  i  ]->_right)->color = Vec(0.340,0.850,0.614);
        c_l->add(spring_vec[  i   ]);
        c_l->add( spring_vec[  i   ]->_left );
        c_l->add( spring_vec[  i   ]->_right );
    }
    int init16 = 14 + ( (dim-1) * 27 );
    for (int i = init16; i < init16 + 4; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.340,0.850,0.614);
        (spring_vec[  i  ]->_right)->color = Vec(0.340,0.850,0.614);
        c_l->add(spring_vec[  i   ]);
        c_l->add( spring_vec[  i   ]->_left );
        c_l->add( spring_vec[  i   ]->_right );
    }
  
    vector <int> index_vec5;
    index_vec5.push_back(12 + ((dim-1) * 23));
    index_vec5.push_back(19 + ((dim-1) * 23));
    index_vec5.push_back(12 + ((dim-1) * 24));
    index_vec5.push_back(19 + ((dim-1) * 24));
    index_vec5.push_back(16 + ((dim-1) * 28));
    index_vec5.push_back(15 + ((dim-1) * 28));
    for (int x : index_vec5){
        (spring_vec[  x ]->_left)->color = Vec(0.340,0.850,0.614);
        (spring_vec[  x  ]->_right)->color = Vec(0.340,0.850,0.614);
        c_l->add(spring_vec[  x   ]);
        c_l->add( spring_vec[  x   ]->_left );
        c_l->add( spring_vec[  x   ]->_right );
    }

    /**
     * Make right cheek
     * */

    Container * c_r = sim->createContainer();
    int init17 = 32 + ( (dim-1) * 24 );
    for (int i = init17; i < init17 + 6; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.340,0.850,0.614);
        (spring_vec[  i  ]->_right)->color = Vec(0.340,0.850,0.614);
        c_r->add(spring_vec[  i   ]);
        c_r->add( spring_vec[  i   ]->_left );
        c_r->add( spring_vec[  i   ]->_right );
    }
    int init18 = 32 + ( (dim-1) * 25 );
    for (int i = init18; i < init18 + 6; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.340,0.850,0.614);
        (spring_vec[  i  ]->_right)->color = Vec(0.340,0.850,0.614);
        c_r->add(spring_vec[  i   ]);
        c_r->add( spring_vec[  i   ]->_left );
        c_r->add( spring_vec[  i   ]->_right );
    }
    int init19 = 32 + ( (dim-1) * 26 );
    for (int i = init19; i < init19 + 6; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.340,0.850,0.614);
        (spring_vec[  i  ]->_right)->color = Vec(0.340,0.850,0.614);
        c_r->add(spring_vec[  i   ]);
        c_r->add( spring_vec[  i   ]->_left );
        c_r->add( spring_vec[  i   ]->_right );
    }
    int init20 = 33 + ( (dim-1) * 27 );
    for (int i = init20; i < init20 + 4; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.340,0.850,0.614);
        (spring_vec[  i  ]->_right)->color = Vec(0.340,0.850,0.614);
        c_r->add(spring_vec[  i   ]);
        c_r->add( spring_vec[  i   ]->_left );
        c_r->add( spring_vec[  i   ]->_right );
    }
  
    vector <int> index_vec6;
    index_vec6.push_back(31 + ((dim-1) * 23));
    index_vec6.push_back(38 + ((dim-1) * 23));
    index_vec6.push_back(31 + ((dim-1) * 24));
    index_vec6.push_back(38 + ((dim-1) * 24));
    index_vec6.push_back(34 + ((dim-1) * 28));
    index_vec6.push_back(35 + ((dim-1) * 28));
    for (int x : index_vec6){
        (spring_vec[  x ]->_left)->color = Vec(0.340,0.850,0.614);
        (spring_vec[  x  ]->_right)->color = Vec(0.340,0.850,0.614);
        c_r->add(spring_vec[  x   ]);
        c_r->add( spring_vec[  x   ]->_left );
        c_r->add( spring_vec[  x   ]->_right );
    }



    /**
     * Make lower ear rec left
     * */

    /**Container * l_e_r_l = sim.createContainer();
    int j = 31;
    while (j < 34){ 
        int init21 = 8 + ( (dim-1) * j );
        for (int i = init21; i < init21 + 2; i++){
            (spring_vec[  i  ]->_left)->color = Vec(.5, .7, .7);
            (spring_vec[  i  ]->_right)->color = Vec(.5, .7, .7);
            l_e_r_l->add(spring_vec[  i   ]);
            l_e_r_l->add( spring_vec[  i   ]->_left );
            l_e_r_l->add( spring_vec[  i   ]->_right );
	}
	j++;
    }**/
    


    /**
     * Make left nose rec
     * */

    /**
     * Make left nose square
     * */

    /**
     * Make right nose square
     * */

    /**
     * Make right nose rec
     * */

    /**
     * Make lower ear rec right
     * */

    /**Container * l_e_r_r = sim.createContainer();
    int k = 31;
    while (k < 34){ 
        int init22 = 40 + ( (dim-1) * k );
        for (int i = init22; i < init22 + 2; i++){
            (spring_vec[  i  ]->_left)->color = Vec(.5, .7, .7);
            (spring_vec[  i  ]->_right)->color = Vec(.5, .7, .7);
            l_e_r_r->add(spring_vec[  i   ]);
            l_e_r_r->add( spring_vec[  i   ]->_left );
            l_e_r_r->add( spring_vec[  i   ]->_right );
	}
	k++;
    }**/
    
    /**
     * Make lower nose diagonal rec left
     * */

    /**
     * Make lower nose diagonal rec right
     * */

    /**
     * Make left mouth rectangle upright
     * */

    /**
     * Make left mouth rectangle sideways
     * */

    /**
     * Make left mouth rectangle diagonal
     * */

    /**
     * Make upper lip
     * */

    Container * u_lip = sim->createContainer();
    int init23 = 21 + ( (dim-1) * 32 );
    for (int i = init23; i < init23 + 9; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.194,0.398,0.850);
        (spring_vec[  i  ]->_right)->color = Vec(0.194,0.398,0.850);
        u_lip->add(spring_vec[  i   ]);
        u_lip->add( spring_vec[  i   ]->_left );
        u_lip->add( spring_vec[  i   ]->_right );
    }


    Container *u_lip_vert = sim->createContainer();
    for (int i = 21; i < 21 + 10; i++){
        (spring_vec_2[ (i*50) + 32 ]->_left)->color = Vec(.194,.398,.850);
	(spring_vec_2[ (i*50) + 32 ]->_right)->color = Vec(.194,.398,.850);
	u_lip_vert->add( spring_vec_2[ (i*50) + 32] );
        u_lip_vert->add( spring_vec_2[ (i*50) + 32]->_left );
	u_lip_vert->add( spring_vec_2[ (i*50) + 32]->_right );
    }

    //u_lip_vert->setRestLengths(2);


    int init24 = 19 + ( (dim-1) * 33 );
    for (int i = init24; i < init24 + 12; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.194,0.398,0.850);
        (spring_vec[  i  ]->_right)->color = Vec(0.194,0.398,0.850);
        u_lip->add(spring_vec[  i   ]);
        u_lip->add( spring_vec[  i   ]->_left );
        u_lip->add( spring_vec[  i   ]->_right );
    }
    
    vector <int> index_corner;
    vector <int> index_not_corner;
    Container *mouth_corner = sim->createContainer();

    index_corner.push_back(18 + ((dim-1) * 34));

    index_not_corner.push_back(19 + ((dim-1) * 34));
    index_not_corner.push_back(30 + ((dim-1) * 34));

    index_corner.push_back(31 + ((dim-1) * 34));
    //index_vec7.push_back(34 + ((dim-1) * 28));
    //index_vec7.push_back(35 + ((dim-1) * 28));
    for (int x : index_not_corner){
        (spring_vec[  x ]->_left)->color = Vec(0.194,0.398,0.850);
        (spring_vec[  x  ]->_right)->color = Vec(0.194,0.398,0.850);
        u_lip->add(spring_vec[  x   ]);
        u_lip->add( spring_vec[  x   ]->_left );
        u_lip->add( spring_vec[  x   ]->_right );
    }
    for (int x : index_corner){
        //(spring_vec[  x ]->_left)->color = Vec(1,1,1);
        //(spring_vec[  x  ]->_right)->color = Vec(1,1,1);
        u_lip->add(spring_vec[  x   ]);
        u_lip->add( spring_vec[  x   ]->_left );
        u_lip->add( spring_vec[  x   ]->_right );
    }

    for (int i = 16; i < 16+20; i++){
        if( (i == 16)  || (i == 34) ){ 
        //(spring_vec_2[  (i*50) + 33 ]->_left)->color = Vec(1,1,1);
        //(spring_vec_2[  (i*50) + 33  ]->_right)->color = Vec(1,1,1);
        mouth_corner->add(spring_vec_2[  (i*50) + 33   ]);
        (spring_vec_2[ (i*50) + 33 ]->_right)->fix(); 
        mouth_corner->add( spring_vec_2[  (i*50) + 33   ]->_left );
        mouth_corner->add( spring_vec_2[  (i*50) + 33   ]->_right );
	}
    }

    mouth_corner->setRestLengths(8);


    /**
     * Make right mouth diagonal diagonal
     * */

    /**
     * Make right mouth rectangle sidways
     * */

    /**
     * Make right mouth rectangle upright
     * */

    /**
     * Make lower lip
     * */
    
    Container * l_lip = sim->createContainer();
    int init25 = 19 + ( (dim-1) * 37 );
    for (int i = init25; i < init25 + 12; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.194,0.398,0.850);
        (spring_vec[  i  ]->_right)->color = Vec(0.194,0.398,0.850);
        l_lip->add(spring_vec[  i   ]);
        l_lip->add( spring_vec[  i   ]->_left );
        l_lip->add( spring_vec[  i   ]->_right );
    }
    int init26 = 21 + ( (dim-1) * 38 );
    for (int i = init26; i < init26 + 9; i++){
        (spring_vec[  i  ]->_left)->color = Vec(0.194,0.398,0.850);
        (spring_vec[  i  ]->_right)->color = Vec(0.194,0.398,0.850);
        l_lip->add(spring_vec[  i   ]);
        l_lip->add( spring_vec[  i   ]->_left );
        l_lip->add( spring_vec[  i   ]->_right );
    }
    
    vector <int> index_vec9;
    index_vec9.push_back(18 + ((dim-1) * 36));
    index_vec9.push_back(19 + ((dim-1) * 36));
    index_vec9.push_back(30 + ((dim-1) * 36));
    index_vec9.push_back(31 + ((dim-1) * 36));
    //index_vec7.push_back(34 + ((dim-1) * 28));
    //index_vec7.push_back(35 + ((dim-1) * 28));
    for (int x : index_vec9){
        (spring_vec[  x ]->_left)->color = Vec(0.194,0.398,0.850);
        (spring_vec[  x  ]->_right)->color = Vec(0.194,0.398,0.850);
        l_lip->add(spring_vec[  x   ]);
        l_lip->add( spring_vec[  x   ]->_left );
        l_lip->add( spring_vec[  x   ]->_right );
    }

    /**
     * Lower mouth diagonal rectanlge left
     * */

    /**
     * Lower mouth diagonal rectangle right
     * */
    
    /**
     * Make Nose?
     * */

    vector <int> index_vec10;
    index_vec10.push_back(22 + ((dim-1) * 29));
    index_vec10.push_back(23 + ((dim-1) * 30));
    index_vec10.push_back(24 + ((dim-1) * 30));
    index_vec10.push_back(25 + ((dim-1) * 30));
    index_vec10.push_back(26 + ((dim-1) * 30));
    index_vec10.push_back(27 + ((dim-1) * 29));
    for (int x : index_vec10){
        (spring_vec[  x ]->_left)->color = Vec(1, 1, 1);
        (spring_vec[  x  ]->_right)->color = Vec(1, 1, 1);
        l_lip->add(spring_vec[  x   ]);
        l_lip->add( spring_vec[  x   ]->_left );
        l_lip->add( spring_vec[  x   ]->_right );
    }
	    
    sim->start(); 

}
